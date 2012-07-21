/*----------------------------------------------------------------------------*
 *
 *----------------------------------------------------------------------------*/

#ifndef Instruction_hh
#define Instruction_hh

#include <vector>

namespace atlas {

class instruction_t
{
public:

	enum state_t {
		pending,
		executing,
		stalled,
		retired
	}; // enum state_t

	instruction_t(size_t latency)
		: state_(pending), latency_(latency), cycles_(0) {}

	void add_dependency(instruction_t * inst) {
		depends_.push_back(inst);
	} // add_dependency

	state_t state() { return state_; }

	state_t advance() {
		// check for active dependencies
		for(auto ita = depends_.begin(); ita != depends_.end(); ++ita) {
			if((*ita)->state() != retired) {
				state_ = stalled;
				return state_;
			} // if
		} // for

		state_ = ++cycles_ == latency_ ? retired : executing;
		return state_;
	} // advance

private:

	state_t state_;
	size_t latency_;
	size_t cycles_;

	std::vector<instruction_t *> depends_;

}; // class instruction_t

} // namespace atlas

#endif // Instruction_hh
