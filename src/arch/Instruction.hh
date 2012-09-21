/*----------------------------------------------------------------------------*
 * Copyright (c) 2012 Los Alamos National Security, LLC
 * All rights reserved
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
 * Instruction class.
 *----------------------------------------------------------------------------*/

#ifndef Instruction_hh
#define Instruction_hh

#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <cstdio>
#include <limits>

#include <Dependency.hh>
#include <MachineState.hh>
#include <OpCodes.hh>
#include <LogUtils.hh>

namespace ska {

/*----------------------------------------------------------------------------*
 * Instruction properties class.
 *----------------------------------------------------------------------------*/

struct instruction_properties_t {
	instruction_properties_t()
		: latency(std::numeric_limits<size_t>::max()),
		reciprocal(std::numeric_limits<float>::max()),
		opcode(std::numeric_limits<unsigned>::max()),
		optype(std::numeric_limits<unsigned>::max()),
		ir(""), name("") {}

	size_t latency;
	float reciprocal;
	unsigned opcode;
	unsigned optype;
	std::string ir;
	std::string name;
}; // struct instruction_properties_t

/*----------------------------------------------------------------------------*
 * Instruction class.
 *----------------------------------------------------------------------------*/

class instruction_t : public dependency_t
{
public:

	/*-------------------------------------------------------------------------*
	 * Constructor.
	 *-------------------------------------------------------------------------*/

	instruction_t(instruction_properties_t props)
		: dependency_t(props.name), props_(props), alu_(-1),
		multiple_(1), cycles_(0), issued_(0), retired_(0),
		machine_(machine_state_t::instance()) {
		// remove weird returns from IR
		size_t offset = props_.ir.find_first_of('\n');
		while(offset != std::string::npos) {
			Warn("IR has returns (removing from output)");

			while(props_.ir[offset+1] == ' ') {
				props_.ir.erase(offset+1, 1);
			} // while

			props_.ir.replace(offset, 1, 1, ' ');
			offset = props_.ir.find_first_of('\n');
		} // while
	} // instruction_t

	/*-------------------------------------------------------------------------*
	 * Destructor.
	 *-------------------------------------------------------------------------*/

	~instruction_t() {}

	/*-------------------------------------------------------------------------*
	 * Reset instruction to default state.
	 *
	 * This does not reset any of the static properties of the instruction.
	 *-------------------------------------------------------------------------*/

	void reset() {
		state_ = pending;
		alu_ = -1;
		multiple_ = 1;
		cycles_ = 0;
		issued_ = 0;
		retired_ = 0;
		stream_.str("");
	} // reset

	/*-------------------------------------------------------------------------*
	 * Update the tree properties for the subexpression represented by this
	 * instruction.
	 *
	 * Depth - simple depth from instruction to leaf.
	 *
	 * Strahler Number - a measure of the branching complexity of the
	 * tree of dependencies of an instruction.
	 *-------------------------------------------------------------------------*/

	void update_tree_properties() {
//		if(!is_memory_access_op(props_.opcode)) {
			size_t _strahler_max(1);
			size_t _depth_max(1);
			for(auto ita = depends_.begin(); ita != depends_.end(); ++ita) {
				_strahler_max =
					std::max(_strahler_max, (*ita)->strahler_number());	
				_depth_max = std::max(_depth_max, (*ita)->depth());
			} // for

			size_t _num_with_smax(0);

			for(auto ita = depends_.begin(); ita != depends_.end(); ++ita) {
				_num_with_smax +=
					(*ita)->strahler_number() == _strahler_max ? 1 : 0;
			} // for

			strahler_ = _num_with_smax > 1 ? _strahler_max + 1 : _strahler_max;
			depth_ = _depth_max + 1;
//		} // if

		switch(props_.opcode) {
			case llvm::Instruction::Load:
				set_style("filled");
				set_fill_color("lightblue1");
				break;
			case llvm::Instruction::Store:
				set_style("filled");
				set_fill_color("goldenrod1");
				break;
			case llvm::Instruction::FAdd:
			case llvm::Instruction::FSub:
			case llvm::Instruction::FMul:
			case llvm::Instruction::FDiv:
			case llvm::Instruction::FRem:
				set_style("filled");
				set_fill_color("lightseagreen");
				break;
		} // switch
	} // update_tree_properties

	/*-------------------------------------------------------------------------*
	 *-------------------------------------------------------------------------*/

	bool in_tree() {
		bool _in_tree(true);

#if 0
		// remove for loop construct
		bool for_loop(false);
		if(opcode() == llvm::Instruction::Br && num_dependencies() == 1) {
			dependency_t * dep = dependency(0);
			if(dep->opcode() == llvm::Instruction::ICmp &&
				dep->num_dependencies() == 1) {
				dep = dep->dependency(0);
				if(dep->opcode() == llvm::Instruction::Trunc &&
					dep->num_dependencies() == 1) {
					dep = dep->dependency(0);
					if(dep->opcode() == llvm::Instruction::Add &&
						dep->num_dependencies() == 1) {
						dep = dep->dependency(0);
						if(dep->opcode() == llvm::Instruction::PHI &&
							dep->num_dependencies() == 1) {
							dep = dep->dependency(0);
							if(dep->opcode() == llvm::Instruction::Add) {
								std::cerr << "FOUND FOR LOOP" << std::endl;
								for_loop = true;
							} // if
						} // if
					} // if
				} // if
			} // if
		} // if

		if(for_loop) {
			prune();
			dependency_t * dep = dependency(0);
			dep->prune();
			dep = dep->dependency(0);
			dep->prune();
			dep = dep->dependency(0);
			dep->prune();
			dep = dep->dependency(0);
			dep->prune();
		} // if
		
		// remove while loop construct
		bool while_loop(false);
		if(opcode() == llvm::Instruction::Br && num_dependencies() == 1) {
			dependency_t * dep = dependency(0);
			if(dep->opcode() == llvm::Instruction::ICmp &&
				dep->num_dependencies() == 1) {
				dep = dep->dependency(0);
				if(dep->opcode() == llvm::Instruction::GetElementPtr &&
					dep->num_dependencies() == 1) {
					dep = dep->dependency(0);
					if(dep->opcode() == llvm::Instruction::PHI &&
						dep->num_dependencies() == 1) {
						dep = dep->dependency(0);
						if(dep->opcode() == llvm::Instruction::GetElementPtr) {
							std::cerr << "FOUND WHILE LOOP" << std::endl;
							while_loop = true;
						} // if
					} // if
				} // if
			} // if
		} // if

		if(while_loop) {
			prune();
			dependency_t * dep = dependency(0);
			dep->prune();
			dep = dep->dependency(0);
			dep->prune();
			dep = dep->dependency(0);
			dep->prune();
		} // if
		
#if 0
		// weird little dangling branch
		bool dangling_branch(false);
		if(opcode() == llvm::Instruction::Br && num_dependencies() == 1) {
			dependency_t * dep = dependency(0);
			if(dep->opcode() == llvm::Instruction::ICmp &&
				dep->num_dependencies() == 0) {
					dangling_branch = true;
			} // if
		} // if

		if(dangling_branch) {
			prune();
			dependency_t * dep = dependency(0);
			dep->prune();
		} // if

		_in_tree = _in_tree && dependency_t::in_tree();
		_in_tree = _in_tree && !is_pruned_op(props_.opcode);
#endif
#endif
		return _in_tree;
	} // in_tree

	/*-------------------------------------------------------------------------*
	 * Check data dependencies.
	 *-------------------------------------------------------------------------*/

	bool ready() {
		for(auto ita = depends_.begin(); ita != depends_.end(); ++ita) {
			if((*ita)->state() != retired) {
				return false;
			} // if
		} // for

		return true;
	} // ready

	/*-------------------------------------------------------------------------*
	 * Return the alu assinged to this instruction.
	 *-------------------------------------------------------------------------*/

	int32_t alu() const { return alu_; }

	/*-------------------------------------------------------------------------*
	 * Issue this instruction.
	 *-------------------------------------------------------------------------*/

	void issue(int32_t alu) {
		alu_ = alu;
		cycles_ = 0;
		issued_ = machine_.current();
		state_ = staging;

		// add space up to current cycle
		stream_.str("");
		for(size_t i(0); i<machine_.current(); ++i) {
			stream_ << ' ';
		} // for
	} // issue

	/*-------------------------------------------------------------------------*
	 * Advance instruction state.
	 *-------------------------------------------------------------------------*/

	void advance() {
		// default state is unissued
		if(state_ == pending) {
			stream_ << ' ';
			return;
		} // if

		// check for active dependencies
		size_t depends_retired(0);
		for(auto ita = depends_.begin(); ita != depends_.end(); ++ita) {
			if((*ita)->state() != retired) {
				state_ = stalled;
				stream_ << '-';
				return;
			} // if

			depends_retired = (*ita)->cycle_retired() > depends_retired ?
				(*ita)->cycle_retired() :
				depends_retired;
		} // for

		// this checks to see if a dependency was retired on this cycle,
		// meaning that this instruction can't advance until next cycle.
		if(machine_.current() > 0 && depends_retired == machine_.current()) {
			state_ = stalled;
			issued_ = machine_.current() + 1;
			stream_ << '-';
			return;
		} // if

		// set new state.
		state_ =
			// if
			++cycles_ >= props_.latency ?
				retired :
			// else if
			cycles_ > size_t(props_.reciprocal-1) ?
				executing :
			// else
				staging; // still inside of issue latency

		// record when instruction retired.
		if(state_ == retired) {
			retired_ = machine_.current();
		} // if

		// write cycle digit to stream.
		stream_ << machine_.counter();
	} // advance

	/*-------------------------------------------------------------------------*
	 * Return the current progress of the instruction.
	 *-------------------------------------------------------------------------*/

	size_t progress() const { return cycles_; }

	/*-------------------------------------------------------------------------*
	 * Set multiple issue state.
	 *-------------------------------------------------------------------------*/

	void set_multiple(int32_t m) { multiple_ = m; }

	/*-------------------------------------------------------------------------*
	 * Return the LLVM OpCode of the instruction.
	 *-------------------------------------------------------------------------*/

	unsigned opcode() const { return props_.opcode; }

	/*-------------------------------------------------------------------------*
	 * Return the LLVM TypeID of the instruction.
	 *-------------------------------------------------------------------------*/

	unsigned optype() const { return props_.optype; }

	/*-------------------------------------------------------------------------*
	 * Return execution history as a string.
	 *-------------------------------------------------------------------------*/

	std::string string() {
		std::string tmp(stream_.str());

		while(tmp.size() < machine_.current()+10) {
			tmp += ' ';
		} // while

		char buffer[256];
		if(multiple_ > 1) {
			char m = 'X';

			switch(multiple_) {
				case 2:
					m = 'D';
					break;
				case 3:
					m = 'T';
					break;
				case 4:
					m = 'Q';
					break;
			} // switch

			sprintf(buffer, "%06d | %1d:%1c | ", int(issued_), alu_, m);
		}
		else {
			sprintf(buffer, "%06d | %1d   | ", int(issued_), alu_);
		} // if

		return buffer + tmp + '|' + props_.ir;
	} // string

	/*-------------------------------------------------------------------------*
	 * Return the latency of this instruction.
	 *-------------------------------------------------------------------------*/

	size_t latency() const { return props_.latency; }

	/*-------------------------------------------------------------------------*
	 * Return the issue latency of this instruction.
	 *-------------------------------------------------------------------------*/

	size_t reciprocal() const { return size_t(props_.reciprocal); }

	/*-------------------------------------------------------------------------*
	 * Return the cycle on which this instruction was issued.
	 *-------------------------------------------------------------------------*/

	size_t cycle_issued() const {
		return state_ > pending ?
			issued_ :
			std::numeric_limits<size_t>::max();
	} // cycle_issued

	/*-------------------------------------------------------------------------*
	 * Return the cycle on which this instruction was retired.
	 *-------------------------------------------------------------------------*/

	size_t cycle_retired() const {
		return state_ == retired ?
			retired_ :
			std::numeric_limits<size_t>::max();
	} // cycle_retired

private:

	instruction_properties_t props_;

	int32_t alu_;
	int32_t multiple_;
	size_t cycles_;
	size_t issued_;
	size_t retired_;

	std::stringstream stream_;

	machine_state_t & machine_;
}; // class instruction_t

} // namespace ska

#endif // Instruction_hh

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
