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
		last,
		retired
	}; // enum state_t

	/*-------------------------------------------------------------------------*
	 * Constructor.
	 *-------------------------------------------------------------------------*/

	instruction_t(instruction_properties_t props)
		: props_(props), state_(pending), alu_(-1), multiple_(1),
		cycles_(0), issue_(0), machine_(machine_state_t::instance()) {

		for(size_t i(0); i<machine_.current(); ++i) {
			stream_ << ' ';
		} // for
	} // instruction_t

	/*-------------------------------------------------------------------------*
	 * Destructor.
	 *-------------------------------------------------------------------------*/

	~instruction_t() {}

	/*-------------------------------------------------------------------------*
	 * Add instruction dependencies.
	 *-------------------------------------------------------------------------*/

	void add_dependency(instruction_t * inst) {
		depends_.push_back(inst);
		if(inst->state() != retired) {
			state_ = stalled;
		} // if
	} // add_dependency

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
		issue_ = machine_.current() - 1;
		state_ = staging;
	} // issue

	/*-------------------------------------------------------------------------*
	 * Advance instruction state.
	 *-------------------------------------------------------------------------*/

	state_t advance() {
		// check for active dependencies
		for(auto ita = depends_.begin(); ita != depends_.end(); ++ita) {
			if((*ita)->state() != retired) {
				state_ = stalled;
				stream_ << '-';
				return state_;
			} // if
		} // for

		state_ =
			// if
			++cycles_ == props_.latency ?
				last : // in last pipeline stage
			// else if
			cycles_ > props_.latency ?
				retired :
			// else if
			cycles_ > size_t(props_.reciprocal-1) ?
				executing :
			// else
				staging; // still inside of issue latency

		if(state_ != retired) {
			stream_ << machine_.counter();
		} // if

		return state_;
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

			sprintf(buffer, "%06d | %2d:%1c | ", int(issue_), alu_, m);
		}
		else {
			sprintf(buffer, "%06d | %2d   | ", int(issue_), alu_);
		} // if

		return buffer + stream_.str() + '|' + props_.ir;
	} // string

private:

	instruction_properties_t props_;

	state_t state_;
	int32_t alu_;
	int32_t multiple_;
	size_t cycles_;
	size_t issue_;

	std::vector<instruction_t *> depends_;
	std::stringstream stream_;

	machine_state_t & machine_;

}; // class instruction_t

} // namespace ska

#endif // Instruction_hh
