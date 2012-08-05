/*----------------------------------------------------------------------------*
 * Instruction class.
 *----------------------------------------------------------------------------*/

#ifndef Instruction_hh
#define Instruction_hh

#include <vector>
#include <string>
#include <sstream>
#include <cstdio>

#include <MachineState.hh>

namespace ska {

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
		issued,
		stalled,
		executing,
		last,
		retired
	}; // enum state_t

	/*-------------------------------------------------------------------------*
	 * Constructor.
	 *-------------------------------------------------------------------------*/

	instruction_t(size_t latency, unsigned opcode, unsigned optype,
		std::string & ir)
		: latency_(latency), opcode_(opcode), optype_(optype), ir_(ir),
		state_(pending), alu_(-1), multiple_(1), cycles_(0), issue_(0),
		machine_(machine_state_t::instance()) {

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
		state_ = issued;
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

		state_ = ++cycles_ == latency_ ? last :
			cycles_ > latency_ ? retired : executing;

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

	unsigned opcode() const { return opcode_; }

	/*-------------------------------------------------------------------------*
	 * Return the LLVM TypeID of the instruction.
	 *-------------------------------------------------------------------------*/

	unsigned optype() const { return optype_; }

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

		return buffer + stream_.str() + '|' + ir_;
	} // string

private:

	size_t latency_;
	unsigned opcode_;
	unsigned optype_;
	std::string ir_;

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
