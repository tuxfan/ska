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

namespace atlas {

class instruction_t
{
public:

	enum state_t {
		pending,
		executing,
		stalled,
		last,
		retired
	}; // enum state_t

	instruction_t(size_t latency, std::string & ir)
		: state_(pending), latency_(latency), cycles_(0), issue_(0),
		ir_(ir), m_(machine_state_t::instance()) {

		for(size_t i(0); i<m_.current(); ++i) {
			stream_ << ' ';
		} // for
	}

	void add_dependency(instruction_t * inst) {
		depends_.push_back(inst);
	} // add_dependency

	state_t state() { return state_; }

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

	std::string string() {
		while(stream_.str().size() < m_.current()+10) {
			stream_ << ' ';
		} // while

		char buffer[256];
		sprintf(buffer, "%06d  ", int(issue_));

		return buffer + stream_.str() + ir_;
	} // string

private:

	state_t state_;
	size_t latency_;
	size_t cycles_;
	size_t issue_;

	std::vector<instruction_t *> depends_;
	std::stringstream stream_;
	std::string ir_;

	machine_state_t & m_;

}; // class instruction_t

} // namespace atlas

#endif // Instruction_hh
