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
		ir("") {}

	size_t latency;
	float reciprocal;
	unsigned opcode;
	unsigned optype;
	std::string ir;
}; // struct instruction_properties_t

/*----------------------------------------------------------------------------*
 * Instruction class.
 *----------------------------------------------------------------------------*/

class instruction_t
{
public:

	/*-------------------------------------------------------------------------*
	 * Instruction state type.
	 *-------------------------------------------------------------------------*/

	enum state_t {
		pending,
		stalled,
		staging,
		executing,
		retired
	}; // enum state_t

	const char * state_strings[5] = {
		"pending",
		"stalled",
		"staging",
		"executing",
		"retired"
	};

	/*-------------------------------------------------------------------------*
	 * Constructor.
	 *-------------------------------------------------------------------------*/

	instruction_t(instruction_properties_t props)
		: props_(props), state_(pending), alu_(-1), multiple_(1), cycles_(0),
		issued_(0), retired_(0), strahler_(1),
		machine_(machine_state_t::instance()) {

		for(size_t i(0); i<machine_.current(); ++i) {
			stream_ << ' ';
		} // for

		// remove wierd returns from IR
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
	 * Add instruction dependency.
	 *-------------------------------------------------------------------------*/

	void add_dependency(instruction_t * inst) {
		depends_.push_back(inst);
	} // add_dependency

	/*-------------------------------------------------------------------------*
	 * Update the Strahler number of this instruction.
	 *
	 * The Strahler number is a measure of the branching complexity of the
	 * tree of dependencies of an instruction.
	 *-------------------------------------------------------------------------*/

	void update_strahler_number() {
		if(!is_memory_op(props_.opcode)) {
			size_t _max(1);
			for(auto ita = depends_.begin(); ita != depends_.end(); ++ita) {
				_max = std::max(_max, (*ita)->strahler_number());	
			} // for

			size_t _num_with_max(0);

			for(auto ita = depends_.begin(); ita != depends_.end(); ++ita) {
				_num_with_max += (*ita)->strahler_number() == _max ? 1 : 0;
			} // for

			strahler_ = _num_with_max > 1 ? _max + 1 : _max;
		} // if
	} // update_strahler_number

	size_t strahler_number() const { return strahler_; }

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
	 * Return current state.
	 *-------------------------------------------------------------------------*/

	state_t state() { return state_; }

	/*-------------------------------------------------------------------------*
	 * Set current state.
	 *-------------------------------------------------------------------------*/

	void set_state(state_t s) { state_ = s; }

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
	 * Return dependency vector.
	 *-------------------------------------------------------------------------*/

	const std::vector<instruction_t *> & dependencies() {
		return depends_;
	} // dependencies

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

std::string info() {
	std::string info_str("");

	auto c = code_map.begin();
	for(; c != code_map.end(); ++c) {
		if(c->second == opcode()) {
			break;
		} // if
	} // for

	if(c != code_map.end()) {
		char buffer[1024];
		sprintf(buffer, "Instruction\n\top: %s\n\tstate: %s\n\talu: %d\n"
			"\tissue: %d\n\tlatency: %d\n\treciprocal: %d\n\tprogress: %d\n",
			c->first.c_str(), state_strings[state()], alu(), int(cycle_issued()),
			int(latency()), int(reciprocal()), int(progress()));
		info_str += buffer;

		for(auto ita = depends_.begin(); ita != depends_.end(); ++ita) {
			auto dc = code_map.begin();
			for(; dc != code_map.end(); ++dc) {
				if(dc->second == (*ita)->opcode()) {
					break;
				} // if
			} // for

			if(dc != code_map.end()) {
				sprintf(buffer, "\tdependency: %s(%s)\n",
					dc->first.c_str(), state_strings[(*ita)->state()]);
				info_str += buffer;
			} // if
		} // for

		info_str += string() + "\n";
	} // if

	return info_str;
}

	/*-------------------------------------------------------------------------*
	 * Return various instruciton properties.
	 *-------------------------------------------------------------------------*/

	size_t latency() const { return props_.latency; }
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

	state_t state_;
	int32_t alu_;
	int32_t multiple_;
	size_t cycles_;
	size_t issued_;
	size_t retired_;

	size_t strahler_;

	std::vector<instruction_t *> depends_;
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
