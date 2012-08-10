/*----------------------------------------------------------------------------*
 * Instruction class.
 *----------------------------------------------------------------------------*/

#ifndef Instruction_hh
#define Instruction_hh

#include <vector>
#include <string>
#include <sstream>
#include <cstdio>
#include <limits>

#include <MachineState.hh>
#include <OpCodes.hh>

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

	/*-------------------------------------------------------------------------*
	 * Constructor.
	 *-------------------------------------------------------------------------*/

	instruction_t(instruction_properties_t props)
		: props_(props), state_(pending), alu_(-1), multiple_(1), cycles_(0),
		issued_(0), retired_(0), machine_(machine_state_t::instance()) {

		for(size_t i(0); i<machine_.current(); ++i) {
			stream_ << ' ';
		} // for
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
		while(stream_.str().size() < machine_.current()+10) {
			stream_ << ' ';
		} // while

		char buffer[256];
		if(multiple_ > 1) {
			char m;

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

			sprintf(buffer, "%06d | %2d:%1c | ", int(issued_), alu_, m);
		}
		else {
			sprintf(buffer, "%06d | %2d   | ", int(issued_), alu_);
		} // if

		return buffer + stream_.str() + '|' + props_.ir;
	} // string

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

	std::vector<instruction_t *> depends_;
	std::stringstream stream_;

	machine_state_t & machine_;

}; // class instruction_t

} // namespace ska

#endif // Instruction_hh
