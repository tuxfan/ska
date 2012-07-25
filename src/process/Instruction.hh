/*----------------------------------------------------------------------------*
 *
 *----------------------------------------------------------------------------*/

#ifndef Instruction_hh
#define Instruction_hh

#include <vector>
#include <string>
#include <sstream>
#include <cstdio>

#include <MachineState.hh>

namespace ska {

class instruction_t
{
public:

	/*-------------------------------------------------------------------------*
	 * Instruction state type.
	 *-------------------------------------------------------------------------*/

	enum state_t {
		pending,
		stalled,
		executing,
		last,
		retired
	}; // enum state_t

	/*-------------------------------------------------------------------------*
	 * Constructor.
	 *-------------------------------------------------------------------------*/

	instruction_t(int32_t alu, size_t latency, bool multiple, std::string & ir)
		: state_(pending), alu_(alu), latency_(latency), multiple_(multiple),
		cycles_(0), issue_(0), ir_(ir), m_(machine_state_t::instance()) {

		for(size_t i(0); i<m_.current(); ++i) {
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

		if(cycles_ == 0) { issue_ = m_.current() - 1; }

		state_ = ++cycles_ == latency_ ? last :
			cycles_ > latency_ ? retired : executing;

		if(state_ != retired) {
			stream_ << m_.counter();
		} // if

		return state_;
	} // advance

	/*-------------------------------------------------------------------------*
	 * Return execution history as a string.
	 *-------------------------------------------------------------------------*/

	std::string string() {
		while(stream_.str().size() < m_.current()+10) {
			stream_ << ' ';
		} // while

		char buffer[256];
		if(multiple_) {
			sprintf(buffer, "%06d %dM ", int(issue_), alu_);
		}
		else {
			sprintf(buffer, "%06d %d  ", int(issue_), alu_);
		} // if

		return buffer + stream_.str() + ir_;
	} // string

private:

	state_t state_;
	int32_t alu_;
	size_t latency_;
	bool multiple_;
	size_t cycles_;
	size_t issue_;

	std::vector<instruction_t *> depends_;
	std::stringstream stream_;
	std::string ir_;

	machine_state_t & m_;

}; // class instruction_t

} // namespace ska

#endif // Instruction_hh
