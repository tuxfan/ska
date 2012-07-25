#ifndef Core_hh
#define Core_hh

#include <ALU.hh>
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
	 * Add ALUs to the core.
	 *-------------------------------------------------------------------------*/

	void add_unit(alu_t * alu) {
		units_.push_back(alu);
	} // add_unit

	/*-------------------------------------------------------------------------*
	 * Test to see if the core can except an op.  If yes, the id of the ALU
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

private:

	std::vector<alu_t *> units_;	
	size_t max_issue_;

}; // class core_t

} // namespace ska

#endif // Core_hh
