#ifndef Decode_h
#define Decode_h

#if defined(USE_MANGLED_CALL_NAMES)
	#include <cxxabi.h>
#endif

#include <llvm/IR/Instruction.h>

#include "../params/Parameters.h"
#include "../arch/Instruction.h"
#include "../utils/x86CallMap.h"

namespace ska {

/*----------------------------------------------------------------------------*
 * Decode an llvm::Instruction.  This gets property information about
 * the instruction (latency, issue latency, opcode, optype) that is used
 * to contruct a instruction_t instance.
 *----------------------------------------------------------------------------*/

instruction_properties_t decode(llvm::Instruction * instruction) {
	parameters_t & arch = parameters_t::instance();
	instruction_properties_t properties;

	unsigned opcode = instruction->getOpcode();
	unsigned optype = instruction->getType()->getTypeID();

	std::string str;
	llvm::raw_string_ostream rso(str);
	rso << *instruction;
	properties.ir = rso.str();

	Debug(rso.str());

	properties.opcode = opcode;
	properties.optype = optype;
	
	// FIXME: DOES THIS NEED TO BE HERE?
	if(!is_call_op(opcode)) {
		properties.name = code_name(opcode);
	} // if

	switch(opcode) {

		/*----------------------------------------------------------------------*
		 * Terminator operators
		 *----------------------------------------------------------------------*/

		case llvm::Instruction::Ret:
                        switch(optype){
                            case llvm::Type::IntegerTyID:
			          arch.getval(properties.latency, "latency::ret::int");
			          arch.getval(properties.reciprocal, "reciprocal::ret::int");
                                  break;
                            default :
			          arch.getval(properties.latency, "latency::ret");
			          arch.getval(properties.reciprocal, "reciprocal::ret");
                        }
		        break;

		case llvm::Instruction::Br:
			arch.getval(properties.latency, "latency::br");
			arch.getval(properties.reciprocal, "reciprocal::br");
			break;

		case llvm::Instruction::Switch:
			arch.getval(properties.latency, "latency::switch");
			arch.getval(properties.reciprocal, "reciprocal::switch");
			break;

		case llvm::Instruction::IndirectBr:
			arch.getval(properties.latency, "latency::indirectbr");
			arch.getval(properties.reciprocal, "reciprocal::indirectbr");
			break;

		case llvm::Instruction::Invoke:
			arch.getval(properties.latency, "latency::invoke");
			arch.getval(properties.reciprocal, "reciprocal::invoke");
			break;

		case llvm::Instruction::Resume:
			arch.getval(properties.latency, "latency::resume");
			arch.getval(properties.reciprocal, "reciprocal::resume");
			break;

		case llvm::Instruction::Unreachable:
			arch.getval(properties.latency, "latency::unreachable");
			arch.getval(properties.reciprocal, "reciprocal::unreachable");
			break;

		/*----------------------------------------------------------------------*
		 * Binary operators
		 *----------------------------------------------------------------------*/

		case llvm::Instruction::Add:
                        switch(optype){ 
                                case llvm::Type::VectorTyID :
		                 	arch.getval(properties.latency, "latency::add::vector::integer");
		                	arch.getval(properties.reciprocal, "reciprocal::add::vector:integer");
		                	break;
                                case llvm::Type::IntegerTyID :
		                 	arch.getval(properties.latency, "latency::add::integer");
		                	arch.getval(properties.reciprocal, "reciprocal::add::integer");
                                        break;
                                default :
		                        ExitOnError("Add Unhandled Type",
					ska::UnknownCase);
          	 			break;
                       }
                       break;

		case llvm::Instruction::FAdd:
			// get the instruction latency
			switch(optype) {
				case llvm::Type::FloatTyID:
					arch.getval(properties.latency, "latency::fadd::float");
					arch.getval(properties.reciprocal, "reciprocal::fadd::float");
					break;
				case llvm::Type::DoubleTyID:
					arch.getval(properties.latency, "latency::fadd::double");
					arch.getval(properties.reciprocal, "reciprocal::fadd::double");
					break;
				case llvm::Type::VectorTyID:
					arch.getval(properties.latency, "latency::fadd::vector::float");
					arch.getval(properties.reciprocal, "reciprocal::fadd::vector::float");
					break;
				default:
					ExitOnError("FAdd Unhandled Type",
						ska::UnknownCase);
					break;
			} // switch

			break;

		case llvm::Instruction::Sub:
                        switch(optype) {
                                case llvm::Type::IntegerTyID :
			                arch.getval(properties.latency, "latency::sub::integer");
			                arch.getval(properties.reciprocal, "reciprocal::sub::integer");
		                  	break;
                                case llvm::Type::VectorTyID :
			                arch.getval(properties.latency, "latency::sub::vector::integer");
			                arch.getval(properties.reciprocal, "reciprocal::sub::vector::integer");
		                  	break;
                                default :
                                        ExitOnError("Sub Unhandled Type",
						ska::UnknownCase);
					break;
                        }
                        break;

		case llvm::Instruction::FSub:
			// get the instruction latency
			switch(optype) {
				case llvm::Type::FloatTyID:
					arch.getval(properties.latency, "latency::fsub::float");
					arch.getval(properties.reciprocal, "reciprocal::fsub::float");
					break;
				case llvm::Type::DoubleTyID:
					arch.getval(properties.latency, "latency::fsub::double");
					arch.getval(properties.reciprocal, "reciprocal::fsub::double");
					break;
				case llvm::Type::VectorTyID:
					arch.getval(properties.latency, "latency::fsub::vector::float");
					arch.getval(properties.reciprocal, "reciprocal::fsub::vector::float");
					break;
				default:
					ExitOnError("FSub Unhandled Type " <<
						instruction->getType()->getTypeID(),
						ska::UnknownCase);
					break;
			} // switch

			break;

		case llvm::Instruction::Mul:
                        switch(optype){
                                  case llvm::Type::IntegerTyID :
			                arch.getval(properties.latency, "latency::mul::integer");
			                arch.getval(properties.reciprocal, "reciprocal::mul::integer");
			                break;

                                  case llvm::Type::VectorTyID :
                                        arch.getval(properties.latency, "latency::mul::vector::integer");
			                arch.getval(properties.reciprocal, "reciprocal::mul::vector::integer");
			                break;

                                  default :
                                        ExitOnError("Mul Unhandled Type",
						ska::UnknownCase);
					break;
                        }

                        break;

		case llvm::Instruction::FMul:
			// get the instruction latency
			switch(optype) {
				case llvm::Type::FloatTyID:
					arch.getval(properties.latency, "latency::fmul::float");
			arch.getval(properties.reciprocal, "reciprocal::fmul::float");
					break;
				case llvm::Type::DoubleTyID:
					arch.getval(properties.latency, "latency::fmul::double");
			arch.getval(properties.reciprocal, "reciprocal::fmul::double");
					break;
				case llvm::Type::VectorTyID:
					arch.getval(properties.latency, "latency::fmul::vector::float");
			arch.getval(properties.reciprocal, "reciprocal::fmul::vector::float");
					break;
				default:
					ExitOnError("FMul Unhandled Type",
						ska::UnknownCase);
					break;
			} // switch

			break;

		case llvm::Instruction::UDiv:
			arch.getval(properties.latency, "latency::udiv");
			arch.getval(properties.reciprocal, "reciprocal::udiv");
			break;

		case llvm::Instruction::SDiv:
			arch.getval(properties.latency, "latency::sdiv");
			arch.getval(properties.reciprocal, "reciprocal::sdiv");
			break;

		case llvm::Instruction::FDiv:
			// get the instruction latency
			switch(optype) {
				case llvm::Type::FloatTyID:
					arch.getval(properties.latency, "latency::fdiv::float");
			arch.getval(properties.reciprocal, "reciprocal::fdiv::float");
					break;
				case llvm::Type::DoubleTyID:
					arch.getval(properties.latency, "latency::fdiv::double");
			arch.getval(properties.reciprocal, "reciprocal::fdiv::double");
					break;
				case llvm::Type::VectorTyID:
					arch.getval(properties.latency, "latency::fdiv::vector::xmm");
			arch.getval(properties.reciprocal, "reciprocal::fdiv::vector");
					break;
				default:
					ExitOnError("FDiv Unhandled Type",
						ska::UnknownCase);
					break;
			} // switch

			break;

		case llvm::Instruction::URem:
			arch.getval(properties.latency, "latency::urem");
			arch.getval(properties.reciprocal, "reciprocal::urem");
			break;

		case llvm::Instruction::SRem:
			arch.getval(properties.latency, "latency::srem");
			arch.getval(properties.reciprocal, "reciprocal::srem");
			break;

		case llvm::Instruction::FRem:
			// get the instruction latency
			switch(optype) {
				case llvm::Type::FloatTyID:
					arch.getval(properties.latency, "latency::frem::float");
			arch.getval(properties.reciprocal, "reciprocal::frem::float");
					break;
				case llvm::Type::DoubleTyID:
					arch.getval(properties.latency, "latency::frem::double");
			arch.getval(properties.reciprocal, "reciprocal::frem::double");
					break;
				case llvm::Type::VectorTyID:
					arch.getval(properties.latency, "latency::frem::vector");
			arch.getval(properties.reciprocal, "reciprocal::frem::vector");
					break;
				default:
					ExitOnError("FRem Unhandled Type",
						ska::UnknownCase);
					break;
			} // switch

			break;

		/*----------------------------------------------------------------------*
		 * Logical operators
		 *----------------------------------------------------------------------*/

		case llvm::Instruction::Shl:
			arch.getval(properties.latency, "latency::shl");
			arch.getval(properties.reciprocal, "reciprocal::shl");
			break;

		case llvm::Instruction::LShr:
			arch.getval(properties.latency, "latency::lshr");
			arch.getval(properties.reciprocal, "reciprocal::lshr");
			break;

		case llvm::Instruction::AShr:
			arch.getval(properties.latency, "latency::ashr");
			arch.getval(properties.reciprocal, "reciprocal::ashr");
			break;

		case llvm::Instruction::And:
			arch.getval(properties.latency, "latency::and");
			arch.getval(properties.reciprocal, "reciprocal::and");
			break;

		case llvm::Instruction::Or:
			arch.getval(properties.latency, "latency::or");
			arch.getval(properties.reciprocal, "reciprocal::or");
			break;

		case llvm::Instruction::Xor:
			arch.getval(properties.latency, "latency::xor");
			arch.getval(properties.reciprocal, "reciprocal::xor");
			break;

		/*----------------------------------------------------------------------*
		 * Memory operators
		 *----------------------------------------------------------------------*/

		case llvm::Instruction::Alloca:
			arch.getval(properties.latency, "latency::alloca");
			arch.getval(properties.reciprocal, "reciprocal::alloca");
			break;

		case llvm::Instruction::Load:
			arch.getval(properties.latency, "latency::load");
			arch.getval(properties.reciprocal, "reciprocal::load");
			break;
	
		case llvm::Instruction::Store:
			arch.getval(properties.latency, "latency::store");
			arch.getval(properties.reciprocal, "reciprocal::store");
			break;
	
		case llvm::Instruction::GetElementPtr:
			arch.getval(properties.latency, "latency::getelementptr");
			arch.getval(properties.reciprocal, "reciprocal::getelementptr");
			break;
	
		case llvm::Instruction::Fence:
			arch.getval(properties.latency, "latency::fence");
			arch.getval(properties.reciprocal, "reciprocal::fence");
			break;
	
		case llvm::Instruction::AtomicCmpXchg:
			arch.getval(properties.latency, "latency::atomiccmpxchg");
			arch.getval(properties.reciprocal, "reciprocal::atomiccmpxchg");
			break;
	
		case llvm::Instruction::AtomicRMW:
			arch.getval(properties.latency, "latency::atomicrmw");
			arch.getval(properties.reciprocal, "reciprocal::atomicrmw");
			break;
	
		/*----------------------------------------------------------------------*
		 * Cast operators
		 *----------------------------------------------------------------------*/

		case llvm::Instruction::Trunc:
			arch.getval(properties.latency, "latency::trunc");
			arch.getval(properties.reciprocal, "reciprocal::trunc");
			break;
	
		case llvm::Instruction::ZExt:
			arch.getval(properties.latency, "latency::zext");
			arch.getval(properties.reciprocal, "reciprocal::zext");
			break;
	
		case llvm::Instruction::SExt:
			arch.getval(properties.latency, "latency::sext");
			arch.getval(properties.reciprocal, "reciprocal::sext");
			break;
	
		case llvm::Instruction::FPToUI:
			arch.getval(properties.latency, "latency::fptoui");
			arch.getval(properties.reciprocal, "reciprocal::fptoui");
			break;
	
		case llvm::Instruction::FPToSI:
			arch.getval(properties.latency, "latency::fptosi");
			arch.getval(properties.reciprocal, "reciprocal::fptosi");
			break;
	
		case llvm::Instruction::UIToFP:
			arch.getval(properties.latency, "latency::uitofp");
			arch.getval(properties.reciprocal, "reciprocal::uitofp");
			break;
	
		case llvm::Instruction::SIToFP:
			arch.getval(properties.latency, "latency::sitofp");
			arch.getval(properties.reciprocal, "reciprocal::sitofp");
			break;
	
		case llvm::Instruction::FPTrunc:
			arch.getval(properties.latency, "latency::fptrunc");
			arch.getval(properties.reciprocal, "reciprocal::fptrunc");
			break;
	
		case llvm::Instruction::FPExt:
			arch.getval(properties.latency, "latency::fpext");
			arch.getval(properties.reciprocal, "reciprocal::fpext");
			break;
	
		case llvm::Instruction::PtrToInt:
			arch.getval(properties.latency, "latency::ptrtoint");
			arch.getval(properties.reciprocal, "reciprocal::ptrtoint");
			break;
	
		case llvm::Instruction::IntToPtr:
			arch.getval(properties.latency, "latency::inttoptr");
			arch.getval(properties.reciprocal, "reciprocal::inttoptr");
			break;
	
		case llvm::Instruction::BitCast:
			arch.getval(properties.latency, "latency::bitcast");
			arch.getval(properties.reciprocal, "reciprocal::bitcast");
			break;
	
		/*----------------------------------------------------------------------*
		 * Other operators
		 *----------------------------------------------------------------------*/

		case llvm::Instruction::ICmp:
			arch.getval(properties.latency, "latency::icmp");
			arch.getval(properties.reciprocal, "reciprocal::icmp");
			break;

		case llvm::Instruction::FCmp:
			arch.getval(properties.latency, "latency::fcmp");
			arch.getval(properties.reciprocal, "reciprocal::fcmp");
			break;

		case llvm::Instruction::PHI:
			arch.getval(properties.latency, "latency::phi");
			arch.getval(properties.reciprocal, "reciprocal::phi");
			break;

		case llvm::Instruction::Call:
			{
			llvm::CallInst * cinst = llvm::cast<llvm::CallInst>(instruction);

			llvm::Function * func = cinst->getCalledFunction();
			std::string name = func == nullptr ? "unknown" :
				func->getName().str();

			// set defaults
			arch.getval(properties.latency, "latency::call");
			arch.getval(properties.reciprocal, "reciprocal::call");
			properties.name = call_name(name);

			// demangle call name
			name = try_demangle_and_strip(name);

			// check for x86 intrinsics
			if(x86_match(name)) {
				name = x86_call_map[name];
			} // if

			std::string call = name;
			switch(optype) {
				case llvm::Type::PointerTyID:
					call += "::pointer";
					break;
				case llvm::Type::IntegerTyID:
					call += "::integer";
					break;
				case llvm::Type::FloatTyID:
					call += "::float";
					break;
				case llvm::Type::DoubleTyID:
					call += "::double";
					break;
				case llvm::Type::VectorTyID:
					call += "::vector";
					break;
				default:
					Warn("Call Unhandled Type " << optype);
					break;
			} // switch

			// try to read values from architecture file
			int32_t ierr = arch.getval(properties.latency, "latency::" + call);
			ierr |= arch.getval(properties.reciprocal, "reciprocal::" + call);

			// set the code for the called function
			properties.callcode = code_map[name];

			// getval returns true for success, false for failure
			if(ierr != 1) {
				Warn("Unrecognized Call " << name << std::endl <<
					properties.ir);
				properties.latency = 1;
				properties.reciprocal = 1;
				properties.name = unknown;
			} // if

			break;

			} // scope
	
		case llvm::Instruction::Select:
			arch.getval(properties.latency, "latency::select");
			arch.getval(properties.reciprocal, "reciprocal::select");
			break;
	
		case llvm::Instruction::UserOp1:
			arch.getval(properties.latency, "latency::userop1");
			arch.getval(properties.reciprocal, "reciprocal::userop1");
			break;
	
		case llvm::Instruction::UserOp2:
			arch.getval(properties.latency, "latency::userop2");
			arch.getval(properties.reciprocal, "reciprocal::userop2");
			break;
	
		case llvm::Instruction::VAArg:
			arch.getval(properties.latency, "latency::vaarg");
			arch.getval(properties.reciprocal, "reciprocal::vaarg");
			break;
	
		case llvm::Instruction::ExtractElement:
			arch.getval(properties.latency, "latency::extractelement");
			arch.getval(properties.reciprocal, "reciprocal::extractelement");
			break;
	
		case llvm::Instruction::InsertElement:
			arch.getval(properties.latency, "latency::insertelement");
			arch.getval(properties.reciprocal, "reciprocal::insertelement");
			break;
	
		case llvm::Instruction::ShuffleVector:
			arch.getval(properties.latency, "latency::shufflevector");
			arch.getval(properties.reciprocal, "reciprocal::shufflevector");
			break;

		case llvm::Instruction::ExtractValue:
			arch.getval(properties.latency, "latency::extractvalue");
			arch.getval(properties.reciprocal, "reciprocal::extractvalue");
			break;
	
		case llvm::Instruction::InsertValue:
			arch.getval(properties.latency, "latency::insertvalue");
			arch.getval(properties.reciprocal, "reciprocal::insertvalue");
			break;
	
		case llvm::Instruction::LandingPad:
			arch.getval(properties.latency, "latency::landingpad");
			arch.getval(properties.reciprocal, "reciprocal::landingpad");
			break;
	
		default:
			ExitOnError("Unhandled Instruction", ska::UnknownCase);
	} // switch


	return properties;
} // decode

} // namespace ska

#endif // Decode_h
