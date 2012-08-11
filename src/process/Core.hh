/*----------------------------------------------------------------------------*
 * Copyright (c) 2012 Los Alamos National Security, LLC
 * All rights reserved
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
 * Core class.
 *----------------------------------------------------------------------------*/

#ifndef Core_hh
#define Core_hh

#include <LU.hh>
#include <MachineState.hh>

namespace ska {

/*----------------------------------------------------------------------------*
 * Core class.
 *----------------------------------------------------------------------------*/

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

	int32_t accept(instruction_t * inst) {
		for(auto unit = units_.begin(); unit != units_.end(); ++unit) {
			if((*unit)->issue(inst)) {
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

	/*-------------------------------------------------------------------------*
	 * Return the maximum issue rate of the core.
	 *-------------------------------------------------------------------------*/

	size_t max_issue() const { return max_issue_; }

	/*-------------------------------------------------------------------------*
	 * Release the current instruction stream.
	 *-------------------------------------------------------------------------*/

	void release(int32_t id) {
		units_[id]->flush();
	} // release

private:

	std::vector<lu_t *> units_;	
	size_t max_issue_;

}; // class core_t

} // namespace ska

#endif // Core_hh

/*----------------------------------------------------------------------------*
 * Local Variables: 
 * mode:c++
 * c-basic-offset:3
 * indent-tabs-mode:t
 * tab-width:3
 * End:
 *
 * vim: set ts=3 :
 *----------------------------------------------------------------------------*/
