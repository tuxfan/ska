/*----------------------------------------------------------------------------*
 * Logic Unit class.
 *----------------------------------------------------------------------------*/

#ifndef ALU_hh
#define ALU_hh

#include <set>

#include <Instruction.hh>

namespace ska {

/*----------------------------------------------------------------------------*
 * Logic Unit class.
 *----------------------------------------------------------------------------*/

class lu_t
{
public:

	/*-------------------------------------------------------------------------*
	 * Logic Unit state.
	 *-------------------------------------------------------------------------*/

	enum state_t {
		ready,
		busy
	}; // enum state_

	/*-------------------------------------------------------------------------*
	 * Constructor.
	 *-------------------------------------------------------------------------*/

	lu_t(int32_t id)
		: state_(ready), id_(id), type_any_(false), current_(nullptr) {}

	/*-------------------------------------------------------------------------*
	 * Destructor.
	 *-------------------------------------------------------------------------*/

	~lu_t() {}

	/*-------------------------------------------------------------------------*
	 * Add an opcode that the LU can process.
	 *-------------------------------------------------------------------------*/

	void add_op(unsigned op) {
		codes_.insert(op);
	} // add_op

	/*-------------------------------------------------------------------------*
	 * Add a type that the LU can process.
	 *-------------------------------------------------------------------------*/

	void add_type(unsigned type) {
		type_any_ = type == TypeAnyID ? true : type_any_;
		types_.insert(type);
	} // add_type

	/*-------------------------------------------------------------------------*
	 * Try to issue on this ALU.  Returns true if instruction was issued,
	 * false otherwise.
	 *-------------------------------------------------------------------------*/

	bool issue(instruction_t * inst) {
		if(state_ == busy) {
			return false;
		}

		if((type_any_ || types_.find(inst->optype()) != types_.end()) &&
			codes_.find(inst->opcode()) != codes_.end()) {
			state_ = busy;
			current_ = inst;
			current_->issue(id_);
			return true;
		}
		else {
			return false;
		} // if
	}

	/*-------------------------------------------------------------------------*
	 * Update ALU state.
	 *-------------------------------------------------------------------------*/

	state_t advance() {
		if(current_ == nullptr) {
			state_ = ready;
		}
		else if(current_->state() > instruction_t::stalled) {
			state_ = ready;
			current_ = nullptr;
		}
		else {
			state_ = busy;
		} // if

		return state_;
	}

	/*-------------------------------------------------------------------------*
	 * Return current LU state.
	 *-------------------------------------------------------------------------*/

	state_t state() const { return state_; }

	/*-------------------------------------------------------------------------*
	 * Flush the LU's pipeline.
	 *-------------------------------------------------------------------------*/

	void flush() { current_ = nullptr; state_ = ready; }

	/*-------------------------------------------------------------------------*
	 * Return the LU is.
	 *-------------------------------------------------------------------------*/

	int32_t id() const { return id_; }

private:

	state_t state_;
	int32_t id_;
	std::set<unsigned> codes_;
	std::set<unsigned> types_;
	bool type_any_;
	instruction_t * current_;

}; // class lu_t

} // namespace ska

#endif // ALU_hh
