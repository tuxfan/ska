#ifndef ALU_hh
#define ALU_hh

#include <set>

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

	alu_t()
		: state_(ready) {}

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

	bool issue(unsigned op) {
		if(state_ == busy) {
			return false;
		}

		if(codes_.find(op) != codes_.end()) {
			state_ = busy;
			return true;
		} // if

		return false;
	}

	/*-------------------------------------------------------------------------*
	 * Update ALU state.
	 *-------------------------------------------------------------------------*/

	state_t advance() {
		state_ = ready;
		return state_;
	}

	/*-------------------------------------------------------------------------*
	 * Return current ALU state.
	 *-------------------------------------------------------------------------*/

	state_t state() const { return state_; }

	/*-------------------------------------------------------------------------*
	 * Return true if this ALU can execute the given opcode, false otherwise.
	 *-------------------------------------------------------------------------*/

	bool handles(unsigned op) {
		return codes_.find(op) != codes_.end();
	} // handles

private:

	state_t state_;
	std::set<unsigned> codes_;

}; // class alu_t

#endif // ALU_hh
