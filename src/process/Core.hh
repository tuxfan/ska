#ifndef Core_hh
#define Core_hh

#include <LU.hh>
#include <MachineState.hh>

namespace ska {

class core_t
{
public:

	/*-------------------------------------------------------------------------*
	 * Constructor.
	 *-------------------------------------------------------------------------*/

	core_t(size_t max_issue)
		: max_issue_(max_issue) {}

	/*-------------------------------------------------------------------------*
	 * Destructor.
	 *-------------------------------------------------------------------------*/

	~core_t() {}

	/*-------------------------------------------------------------------------*
	 * Add LUs to the core.
	 *-------------------------------------------------------------------------*/

	void add_unit(lu_t * lu) {
		units_.push_back(lu);
	} // add_unit

	/*-------------------------------------------------------------------------*
	 * Test to see if the core can except an op.  If yes, the id of the LU
	 * that will execute the op is return, -1 otherwise.
	 *-------------------------------------------------------------------------*/

	int32_t accept(unsigned op, instruction_t * inst) {
		for(auto unit = units_.begin(); unit != units_.end(); ++unit) {
			if((*unit)->issue(op, inst)) {
				return (*unit)->id();
			} // if
		} // for

		return -1;
	} // accept

	/*-------------------------------------------------------------------------*
	 * Advance the core.
	 *-------------------------------------------------------------------------*/

	void advance() {
		for(auto unit = units_.begin(); unit != units_.end(); ++unit) {
			(*unit)->advance();
		} // for

		machine_state_t::instance().advance();
	} // advance

	size_t max_issue() const { return max_issue_; }

	void release(int32_t id) {
		units_[id]->flush();
	} // release

private:

	std::vector<lu_t *> units_;	
	size_t max_issue_;

}; // class core_t

} // namespace ska

#endif // Core_hh
