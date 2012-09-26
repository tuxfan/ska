/*----------------------------------------------------------------------------*
 * Copyright (c) 2012 Los Alamos National Security, LLC
 * All rights reserved
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
 * Simulator class.
 *----------------------------------------------------------------------------*/

#ifndef Simulator_hh
#define Simulator_hh

#include <string>
#include <cstring>
#include <vector>
#include <list>

#include <llvm/Module.h>
#include <llvm/Constants.h>
#include <llvm/Function.h>
#include <llvm/Instruction.h>
#include <llvm/Instructions.h>
#include <llvm/LLVMContext.h>
#include <llvm/ADT/APInt.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/IRReader.h>
#include <llvm/Support/InstIterator.h>

#include <FileIO.hh>
#include <Decode.hh>
#include <Instruction.hh>
#include <MachineState.hh>
#include <Statistics.hh>

#include <Dependency.hh>

#if defined(HAVE_GRAPHVIZ)
#include <Graphviz.hh>
#endif

#include <OpCodes.hh>
#include <OpTypes.hh>
#include <Core.hh>
#include <Utils.hh>

namespace ska {

#ifndef SKA_VERSION_TAG
#define SKA_VERSION_TAG "undefined"
#endif

/*----------------------------------------------------------------------------*
 * Simulator class.
 *----------------------------------------------------------------------------*/

class simulator_t
{
public:

	/*-------------------------------------------------------------------------*
	 * Public types
	 *-------------------------------------------------------------------------*/

	typedef std::map<llvm::Value *, instruction_t *> instruction_map_t;
	typedef std::map<llvm::Value *, dependency_t *> dependency_map_t;

	typedef std::vector<instruction_t *> instruction_vector_t;
	typedef std::list<instruction_t *> instruction_list_t;

	/*-------------------------------------------------------------------------*
	 * Constructor
	 *-------------------------------------------------------------------------*/

	simulator_t(const char * ir_file);

	/*-------------------------------------------------------------------------*
	 * Destructor
	 *-------------------------------------------------------------------------*/

	~simulator_t();

	/*-------------------------------------------------------------------------*
	 * Run the simulator on a set of LLVM instructions.
	 *-------------------------------------------------------------------------*/

	void process(llvm::inst_iterator begin, llvm::inst_iterator end,
		const dependency_map_t & dmap, instruction_vector_t & retired);

private:

	/*-------------------------------------------------------------------------*
	 * Update statistic based on the execution of 'instruction'.
	 *-------------------------------------------------------------------------*/

	void update_opcount(llvm::Instruction * instruction);

	/*-------------------------------------------------------------------------*
	 * Compute the number of bytes associated with a given llvm::Type.  This
	 * is used to compute load and store sizes.
	 *-------------------------------------------------------------------------*/

	size_t bytes(llvm::Type * type);

	// private data members
	llvm::SMDiagnostic llvm_err_;
	llvm::LLVMContext llvm_context_;
	llvm::Module * llvm_module_;

	core_t * core_;
}; // class simulator_t

/*----------------------------------------------------------------------------*
 * Simulator class.
 *----------------------------------------------------------------------------*/

simulator_t::simulator_t(const char * ir_file)
	: llvm_module_(nullptr), core_(nullptr)
{
	/*-------------------------------------------------------------------------*
	 * Write header information.
	 *-------------------------------------------------------------------------*/

	std::ostream & output = file_io_t::instance().out_stream();
	output << "#---------------------------------------" <<
		"---------------------------------------#" << std::endl;
	output << "# Static Kernel Analyzer (SKA)" << std::endl;
	output << "# Header Section" << std::endl;
	output << "#---------------------------------------" <<
		"---------------------------------------#" << std::endl;
	output << "KEYWORD_SKA_VERSION " <<
		DEFINE_TO_STRING(SKA_VERSION) << std::endl;

	std::string architecture;
	parameters_t & arch = parameters_t::instance();
	arch.getval(architecture, "name");
	output << "KEYWORD_ARCHITECTURE " << architecture <<
		std::endl << std::endl;
	
	/*-------------------------------------------------------------------------*
	 * Start log information.
	 *-------------------------------------------------------------------------*/

	std::ostream & log = file_io_t::instance().log_stream();
	log << "#---------------------------------------" <<
		"---------------------------------------#" << std::endl;
	log << "# Static Kernel Analyzer (SKA) Log Output" << std::endl;
	log << "#---------------------------------------" <<
		"---------------------------------------#" << std::endl;

	/*-------------------------------------------------------------------------*
	 * Initialize core.
	 *-------------------------------------------------------------------------*/
	
	size_t max_issue;
	arch.getval(max_issue, "core::max_issue");

	log << " --- Creating Core ---" << std::endl;
	core_ = new core_t(max_issue);

	size_t lus;
	arch.getval(lus, "lus");

	for(size_t i(0); i<lus; ++i) {
		lu_t * lu = new lu_t(i);

		// parse instructions that this LU can handle
		char key[256];
		std::string tmp;

		sprintf(key, "lu::%d::instructions", int(i));
		arch.getval(tmp, key);

		char * ops = new char[tmp.size()+1];
		strcpy(ops, tmp.c_str());

		log << " LU " << i << std::endl;
		log << " ops handled: " << ops << std::endl;

		char * tok = strtok(ops, " ");

		while(tok != nullptr) {
			lu->add_op(code_map[tok]);
			tok = strtok(nullptr, " ");
		} // while

		delete[] ops;
		tok = nullptr;

		// parse types that this LU can handle
		sprintf(key, "lu::%d::types", int(i));
		arch.getval(tmp, key);

		char * types = new char[tmp.size()+1];
		strcpy(types, tmp.c_str());

		log << " types handled: " << types << std::endl << std::endl;

		tok = strtok(types, " ");

		while(tok != nullptr) {
			lu->add_type(type_map[tok]);
			tok = strtok(nullptr, " ");
		} // while

		delete[] types;

		// add unit to the core
		//core.add_unit(lu);
		core_->add_unit(lu);
	} // for

	/*-------------------------------------------------------------------------*
	 * Get parsed LLVM IR.
	 *-------------------------------------------------------------------------*/

	log << " --- Reading IR File ---" << std::endl;
	log << " " << ir_file << std::endl << std::endl;
	llvm_module_ = ParseIRFile(ir_file, llvm_err_, llvm_context_);

	if(llvm_module_ == nullptr) {
		ExitOnError("LLVM parse failed on " << ir_file << std::endl <<
			llvm_err_.getMessage(), ska::LLVMError);
	} // if

	/*-------------------------------------------------------------------------*
	 * Visit functions.
	 *-------------------------------------------------------------------------*/

	instruction_vector_t retired;
	machine_state_t & machine = machine_state_t::instance();
	statistics_t & stats = statistics_t::instance();

	log << " --- Processing Module ---" << std::endl;
	for(llvm::Module::iterator fita = llvm_module_->begin();
		fita != llvm_module_->end(); ++fita) {

		std::string fname = try_demangle_and_strip(fita->getName().str());
		log << " Function " << fname << std::endl;

		// skip degnerate functions and declarations
		if(inst_begin(fita) == inst_end(fita)) {
			continue;
		} // if

#if defined(HAVE_GRAPHVIZ)
		graphviz_t & graph = graphviz_t::instance();					
		graph.clear();
#endif

		/*----------------------------------------------------------------------*
		 * Create the dependency map for this module and all instructions.
		 * This will allow us to lookup instructions by llvm::Value when
		 * we need to add dependency information.
		 *----------------------------------------------------------------------*/

		log << "  creating dependency map" << std::endl;
		dependency_map_t dmap;

		// function arguments
		llvm::Function::ArgumentListType & args = fita->getArgumentList();
		for(auto aita = args.begin(); aita != args.end(); ++aita) {
			dmap[&*aita] = new dependency_t(aita->getName().str());
		} // for

		// instructions
		for(auto iita = inst_begin(fita); iita != inst_end(fita); ++iita) {
			dmap[&*iita] = new instruction_t(decode(&*iita));
		} // for

		/*----------------------------------------------------------------------*
		 * Add dependency information
		 *----------------------------------------------------------------------*/

		for(auto iita = inst_begin(fita); iita != inst_end(fita); ++iita) {
			auto dita = dmap.find(&*iita);

			if(dita == dmap.end()) {
				Warn("Sanity check failed: did not find instruction");
			} // if

			dependency_t * dep = dita->second;

			for(unsigned i(0); i<iita->getNumOperands(); ++i) {
				auto op = dmap.find(iita->getOperand(i));
				if(op != dmap.end()) {
					dep->add_dependency(op->second);
#if defined(HAVE_GRAPHVIZ)
					graph.add_edge(dep->agnode(), op->second->agnode());
#endif
				} // if
			} // for
		} // for

		/*----------------------------------------------------------------------*
		 * Update tree properties
		 *----------------------------------------------------------------------*/

		log << "  updating tree properties" << std::endl;
		for(auto iita = inst_begin(fita); iita != inst_end(fita); ++iita) {
			auto dita = dmap.find(&*iita);

			if(dita == dmap.end()) {
				Warn("Sanity check failed: did not find instruction");
			} // if

			dita->second->update_tree_properties();
		} // for

		// prune the tree
		for(auto dita = dmap.begin(); dita != dmap.end(); ++dita) {
			dependency_t * dep = dita->second;
			char buffer[1024];
			sprintf(buffer, "(%d)", int(dep->strahler_number()));
			std::string label(dep->name() + buffer);
			dep->set_label(label.c_str());
#if 0
			if(!dep->in_tree()) {
				dep->prune();
			} // if
#endif
		} // for

#if defined(HAVE_GRAPHVIZ)
		graph.write(fname + ".gv");
#endif

		output << "#---------------------------------------" <<
			"---------------------------------------#" << std::endl;
		output << "# Module Section: " << fname << std::endl;
		output << "#---------------------------------------" <<
			"---------------------------------------#" << std::endl;
		output << "BEGIN_MODULE" << std::endl;
		output << "KEYWORD_NAME " << fname << std::endl;

		/*----------------------------------------------------------------------* 
		 * Simulate instruction execution.
		 *----------------------------------------------------------------------*/

		log << "  beginning simulation" << std::endl;
		process(inst_begin(fita), inst_end(fita), dmap, retired);

		log << "  writing output" << std::endl;
		output << "# Primitive Statistics" << std::endl;
		output << "KEYWORD_INSTRUCTIONS " << retired.size() << std::endl;
		output << "KEYWORD_CYCLES " << machine.current() << std::endl;

		// count stall cycles
		size_t stalls(0);
		for(auto rita = retired.begin(); rita != retired.end(); ++rita) {
			stalls += (*rita)->stalls();
		} // for

		output << "KEYWORD_STALLS " << stalls << std::endl;

		output << "KEYWORD_STACK_ALLOCATIONS " <<
			stats["allocas"] << std::endl;
		output << "KEYWORD_STACK_ALLOCATION_BYTES " <<
			stats["alloca bytes"] << std::endl;
		output << "KEYWORD_FLOPS " << stats["flops"] << std::endl;

		output << "KEYWORD_LOADS " << stats["loads"] << std::endl;
		output << "KEYWORD_LOAD_BYTES " << stats["load bytes"] << std::endl;
		output << "KEYWORD_STORES " << stats["stores"] << std::endl;
		output << "KEYWORD_STORE_BYTES " << stats["store bytes"] << std::endl;
		output << "# Derived Statistics" << std::endl;
		output << "KEYWORD_CYCLES_PER_INSTRUCTION " <<
			retired.size()/double(machine.current()) << std::endl;

		double balance = stats["load bytes"] == 0 ? 0.0 :
			stats["flops"]/double(stats["load bytes"]);
		output << "KEYWORD_BALANCE " << balance << std::endl;
		output << "KEYWORD_STRAHLER " << stats["strahler"] << std::endl;
		output << "KEYWORD_LEWIN " <<
			double(retired.size())/stats["depth"] << std::endl;
		output << "KEYWORD_DEPTH " << stats["depth"] << std::endl;
		output << "KEYWORD_BETA " <<
			double(stats["strahler"])/stats["depth"] << std::endl;
		output << "# Pipeline" << std::endl;
		output << "BEGIN_INSTRUCTION_STREAM" << std::endl;

		for(auto out = retired.begin(); out != retired.end(); ++out) {
			output << (*out)->string() << std::endl;
		} // for

		output << "END_INSTRUCTION_STREAM" << std::endl;
		output << "END_MODULE" << std::endl;
	} // for
} // simulator_t::simulator_t

simulator_t::~simulator_t()
{
	delete core_;
} // simulator_t::~simulator_t

void simulator_t::process(llvm::inst_iterator begin, llvm::inst_iterator end,
	const dependency_map_t & dmap, instruction_vector_t & retired) {
	machine_state_t & machine = machine_state_t::instance();
	statistics_t & stats = statistics_t::instance();
	std::ostream & log = file_io_t::instance().log_stream();

	// clear states
	machine.clear();
	stats.clear();
	retired.clear();

	/*-------------------------------------------------------------------------*
	 * Visit instructions.
	 *-------------------------------------------------------------------------*/

	instruction_t * inst(nullptr);
	instruction_list_t active;
	auto iita = begin;
	while(iita != end || active.size() > 0) {
		size_t issued(0);
		bool issue(true);
		std::vector<instruction_t *> cycle_issue;

		while(iita != end && issue && issued < core_->max_issue()) {

			/*-------------------------------------------------------------------*
			 * Create instruction and add dependencies
			 *-------------------------------------------------------------------*/

			if(inst == nullptr) {

				//inst = new instruction_t(decode(&*iita));
				auto dita(dmap.find(&*iita));

				if(dita == dmap.end()) {
					ExitOnError("Instruction not found", ska::AssertionFailed);
				} // if

				inst = dynamic_cast<instruction_t *>(dita->second);

				/*----------------------------------------------------------------*
				 * Keep track of branching complexity and depth.
				 *----------------------------------------------------------------*/

				stats["strahler"] = std::max(stats["strahler"],
					inst->strahler_number());
				stats["depth"] = std::max(stats["depth"],
					inst->depth());

				/*----------------------------------------------------------------*
				 * If an instruction that was previously issed and stalled
				 * is now ready to execute, we can try for multiple-issue.
				 *----------------------------------------------------------------*/

				if(issued == 0) {
					// Check for pending instructions from previous cycles
					// NOTE: This has to happen before the current instruciton
					// is added to active!
					for(auto a = active.begin(); a != active.end(); ++a) {
						if((*a)->state() == instruction_t::stalled &&
							(*a)->ready()) {
							// reset state to staging
							(*a)->set_state(instruction_t::staging);

							// add to instructions issued this cycle
							cycle_issue.push_back(*a);

							// update count
							++issued;
						} // if
					} // for
				} // if
			} // if

			/*-------------------------------------------------------------------*
			 * Check for dependencies of this instruction
			 *
			 * If the currently queued instruction has any dependencies
			 * that have not retired, multiple issue cannot happen.
			 *-------------------------------------------------------------------*/

			issue = inst->ready();

			/*-------------------------------------------------------------------*
			 * Check for dependencies within this issue
			 *
			 * If the current instruction depends on any other instructions
			 * that are to be issued this cycle, multiple issue cannot
			 * happen because the current instruction will immediately
			 * stall.
			 *-------------------------------------------------------------------*/

			bool cycle_dependency(false);
			if(issued > 0) {
				for(auto cita = cycle_issue.begin();
					cita != cycle_issue.end(); ++cita) {
					for(auto dita = inst->dependencies().begin();
						dita != inst->dependencies().end(); ++dita) {
							if(*dita == *cita) {
								cycle_dependency = true;
								break;
							} // if
					} // for

					if(cycle_dependency) {
						break;
					} // if
				} // for
			} // if

			/*-------------------------------------------------------------------*
			 * Check for stalls
			 *
			 * If any instruction is stalled, no new instructions
			 * can be issued.
			 *-------------------------------------------------------------------*/

			bool cycle_stall(false);
			if(!cycle_dependency) {
				for(auto a = active.begin(); /* (*a) != inst && */
					a != active.end(); ++a) {
					// check for any type of stall from an active instruction
					if((*a)->state() == instruction_t::stalled) {
						cycle_stall = true;
						break;
					} // if
				} // for
			} // if

			/*-------------------------------------------------------------------*
			 * Check to see if this instruction will stall on issue.
			 *
			 * The previous check will not catch this because the
			 * instruction hasn't yet been advanced.
			 *-------------------------------------------------------------------*/

			if(issued > 0 && !issue) {
				cycle_stall = true;
			} // if

			/*-------------------------------------------------------------------*
			 * Try to issue
			 *-------------------------------------------------------------------*/

			int32_t id = core_->accept(inst);
			if(!cycle_dependency && !cycle_stall && id >= 0) {

				log << "  issued " << inst->name() <<
					"(" << machine.current() << ")" << std::endl;

				/*#################################################################
				 ##################################################################
				 # Everything that makes it to this point actually gets
				 # issued and executed.
				 ##################################################################
				 *################################################################*/
			
				/*----------------------------------------------------------------*
				 * Update statistics
				 *----------------------------------------------------------------*/

				update_opcount(&*iita);
			
				/*----------------------------------------------------------------*
				 * Add instruction to active list
				 *----------------------------------------------------------------*/

				active.push_back(inst);

				/*----------------------------------------------------------------*
				 * Add instruction to this issue
				 *----------------------------------------------------------------*/
			
				cycle_issue.push_back(inst);

				/*----------------------------------------------------------------*
				 * Add instruction to processed instructions.
				 *----------------------------------------------------------------*/

				retired.push_back(inst);

				/*----------------------------------------------------------------*
				 * Advance LLVM instruction stream.
				 *----------------------------------------------------------------*/

				++issued;
				++iita;
				inst = nullptr;
			}
			else {
				// remove current instruction from active list
				for(auto a = active.begin(); a != active.end(); ++a) {
					if((*a) == inst) {
						active.erase(a);
						break;
					} // if
				} // for

				// cleanup failed multiple issue attempt
				if(cycle_stall || issued > 0) {
					// free the ALU for the current instruction
					if(id != -1) {
						core_->release(id);
					} // if

					// reset instruction
					inst = nullptr;
				} // if

				issue = false;
				continue;
			} // if

			// if multiple issue was possible, update the affected
			// instruction states
			if(issued > 1) {
				for(auto cita = cycle_issue.begin();
					cita != cycle_issue.end(); ++cita) {
					(*cita)->set_multiple(issued);
				} // for
			} // if
		} // while

		// update executing instructions
		auto a = active.begin();
		while(a != active.end()) {
			(*a)->advance();

			if((*a)->state() == instruction_t::retired) {
				active.erase(a++);
			}
			else {
				++a;
			} // if
		} // while

		// set state for next cycle
		core_->advance();
	} // while
} // simulator_t::process

void simulator_t::update_opcount(llvm::Instruction * instruction) {
	statistics_t & stats = statistics_t::instance();

	switch(instruction->getOpcode()) {
		case llvm::Instruction::Ret:
			break;
		case llvm::Instruction::Br:
			break;
		case llvm::Instruction::Switch:
			break;
		case llvm::Instruction::IndirectBr:
			break;
		case llvm::Instruction::Invoke:
			break;
		case llvm::Instruction::Resume:
			break;
		case llvm::Instruction::Unreachable:
			break;
		case llvm::Instruction::Add:
			break;
		case llvm::Instruction::FAdd:
			stats["flops"]++;
			break;
		case llvm::Instruction::Sub:
			break;
		case llvm::Instruction::FSub:
			stats["flops"]++;
			break;
		case llvm::Instruction::Mul:
			break;
		case llvm::Instruction::FMul:
			stats["flops"]++;
			break;
		case llvm::Instruction::UDiv:
			break;
		case llvm::Instruction::SDiv:
			break;
		case llvm::Instruction::FDiv:
			stats["flops"]++;
			break;
		case llvm::Instruction::URem:
			break;
		case llvm::Instruction::SRem:
			break;
		case llvm::Instruction::FRem:
			stats["flops"]++;
			break;
		case llvm::Instruction::Shl:
		case llvm::Instruction::LShr:
		case llvm::Instruction::AShr:
		case llvm::Instruction::And:
		case llvm::Instruction::Or:
		case llvm::Instruction::Xor:
			break;
		case llvm::Instruction::Alloca:
			{
			stats["allocas"]++;
			llvm::AllocaInst * ainst =
				llvm::cast<llvm::AllocaInst>(instruction);
			stats["alloca bytes"] += bytes(ainst->getType());
			break;
			} // scope
		case llvm::Instruction::Load:
			{
			stats["loads"]++;
			llvm::Value * value = instruction->getOperand(0);
			stats["load bytes"] += bytes(value->getType());
			break;
			} // scope
		case llvm::Instruction::Store:
			{
			stats["stores"]++;
			llvm::Value * value = instruction->getOperand(0);
			stats["store bytes"] += bytes(value->getType());
			break;
			} // scope
		case llvm::Instruction::GetElementPtr:
			break;
		case llvm::Instruction::Fence:
			break;
		case llvm::Instruction::AtomicCmpXchg:
			break;
		case llvm::Instruction::AtomicRMW:
			break;
		case llvm::Instruction::Trunc:
			break;
		case llvm::Instruction::ZExt:
			break;
		case llvm::Instruction::SExt:
			break;
		case llvm::Instruction::FPToUI:
			break;
		case llvm::Instruction::FPToSI:
			break;
		case llvm::Instruction::UIToFP:
			break;
		case llvm::Instruction::SIToFP:
			break;
		case llvm::Instruction::FPTrunc:
			break;
		case llvm::Instruction::FPExt:
			break;
		case llvm::Instruction::PtrToInt:
			break;
		case llvm::Instruction::IntToPtr:
			break;
		case llvm::Instruction::BitCast:
			break;
		case llvm::Instruction::ICmp:
			break;
		case llvm::Instruction::FCmp:
			break;
		case llvm::Instruction::PHI:
			break;
		case llvm::Instruction::Call:
			break;
		case llvm::Instruction::Select:
			break;
		case llvm::Instruction::UserOp1:
			break;
		case llvm::Instruction::UserOp2:
			break;
		case llvm::Instruction::VAArg:
			break;
		case llvm::Instruction::ExtractElement:
			break;
		case llvm::Instruction::InsertElement:
			break;
		case llvm::Instruction::ShuffleVector:
			break;
		case llvm::Instruction::ExtractValue:
			break;
		case llvm::Instruction::InsertValue:
			break;
		case llvm::Instruction::LandingPad:
			break;
		default:
			{
				ExitOnError("Unhandled Instruction", ska::UnknownCase);
			} // scope
	} // switch
} // simulator_t::update_opcount

size_t simulator_t::bytes(llvm::Type * type) {
	switch(type->getTypeID()) {

		case llvm::Type::VoidTyID:
			return sizeof(nullptr);

		case llvm::Type::HalfTyID:
			break;

		case llvm::Type::FloatTyID:
			return 4;

		case llvm::Type::DoubleTyID:
			return 8;

		case llvm::Type::X86_FP80TyID:
			break;

		case llvm::Type::FP128TyID:
			break;

		case llvm::Type::PPC_FP128TyID:
			break;

		case llvm::Type::LabelTyID:
			break;

		case llvm::Type::MetadataTyID:
			break;

		case llvm::Type::X86_MMXTyID:
			break;

		case llvm::Type::IntegerTyID:
			break;

		case llvm::Type::FunctionTyID:
			break;

		case llvm::Type::StructTyID:
			break;

		case llvm::Type::ArrayTyID:
			return type->getArrayNumElements() *
				bytes(type->getArrayElementType());

		case llvm::Type::PointerTyID:
			return bytes(type->getPointerElementType());

		case llvm::Type::VectorTyID:
			break;

		case llvm::Type::NumTypeIDs:
			break;

		default:
			break;
	} // switch

	return 0;
} // simulator_t::bytes

} // namespace ska

#endif // Simulator_hh

/*----------------------------------------------------------------------------*
 * Local Variables: 
 * mode:c++
 * c-basic-offset:3
 * indent-tabs-mode:t
 * tab-width:3
 * End:
 *
 * vim: set ts=3 :
 *----------------------------------------------------------------------------*/
