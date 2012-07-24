#ifndef Core_hh
#define Core_hh

#include <ALU.hh>

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

	bool accept(unsigned op) {
		for(auto unit = units_.begin(); unit != units_.end(); ++unit) {
			if((*unit)->issue(op)) {
				return true;
			} // if
		} // for

		return false;
	} // accept

#if 0
	bool accept_new() {
	} // accept_new

	void execute(instruction_t * inst) {
	} // execute
#endif

	void advance() {
		for(auto unit = units_.begin(); unit != units_.end(); ++unit) {
			(*unit)->advance();
		} // for
	} // advance

	size_t max_issue() const { return max_issue_; }

private:

	std::vector<alu_t *> units_;	
	size_t max_issue_;

}; // class core_t

#endif // Core_hh
