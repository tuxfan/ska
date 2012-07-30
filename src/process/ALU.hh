#ifndef ALU_hh
#define ALU_hh

#include <set>

#include <Instruction.hh>

namespace ska {

class alu_t
{
public:

	/*-------------------------------------------------------------------------*
	 *-------------------------------------------------------------------------*/

	enum state_t {
		ready,
		busy
	}; // enum state_

	/*-------------------------------------------------------------------------*
	 * Constructor.
	 *-------------------------------------------------------------------------*/

	alu_t(int32_t id)
		: state_(ready), id_(id), current_(nullptr) {}

	/*-------------------------------------------------------------------------*
	 * Destructor.
	 *-------------------------------------------------------------------------*/

	~alu_t() {}

	void add_op(unsigned op) {
		codes_.insert(op);
	} // add_op

	/*-------------------------------------------------------------------------*
	 * Try to issue on this ALU.  Returns true if instruction was issued,
	 * false otherwise.
	 *-------------------------------------------------------------------------*/

	bool issue(unsigned op, instruction_t * inst) {
//std::cerr << "ALU " << id_ << " issue called: ";
		if(state_ == busy) {
//std::cerr << "busy" << std::endl;
			return false;
		}

		if(codes_.find(op) != codes_.end()) {
//std::cerr << "issuing op " << inst->op() << std::endl;
			state_ = busy;
			current_ = inst;
			current_->issue(id_);
			return true;
		}
		else {
//std::cerr << "can't handle op " << inst->op() << std::endl;
			return false;
		} // if
	}

	/*-------------------------------------------------------------------------*
	 * Update ALU state.
	 *-------------------------------------------------------------------------*/

	state_t advance() {
//std::cerr << "ALU " << id_ << " advance: ";
		if(current_ == nullptr) {
//std::cerr << "ready" << std::endl;
			state_ = ready;
		}
		else if(current_->state() > instruction_t::stalled) {
//std::cerr << "ready (instruction executing)" << std::endl;
			state_ = ready;
			current_ = nullptr;
		}
		else {
//std::cerr << "busy" << std::endl;
			state_ = busy;
		} // if

		return state_;
	}

	/*-------------------------------------------------------------------------*
	 * Return current ALU state.
	 *-------------------------------------------------------------------------*/

	state_t state() const { return state_; }

	void flush() { current_ = nullptr; state_ = ready; }

	int32_t id() const { return id_; }

	/*-------------------------------------------------------------------------*
	 * Return true if this ALU can execute the given opcode, false otherwise.
	 *-------------------------------------------------------------------------*/

	bool handles(unsigned op) {
		return codes_.find(op) != codes_.end();
	} // handles

private:

	state_t state_;
	int32_t id_;
	std::set<unsigned> codes_;
	instruction_t * current_;

}; // class alu_t

} // namespace ska

#endif // ALU_hh
