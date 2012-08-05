/*----------------------------------------------------------------------------*
 * Statistics class.
 *----------------------------------------------------------------------------*/

#ifndef Statistics_hh
#define Statistics_hh

#include <map>

/*----------------------------------------------------------------------------*
 * Statistics class.
 *----------------------------------------------------------------------------*/

class statistics_t : public std::map<std::string, size_t>
{
public:

	/*-------------------------------------------------------------------------*
	 * Meyer's singleton instance.
	 *-------------------------------------------------------------------------*/

	static statistics_t & instance() {
		static statistics_t m;
		return m;
	} // instance

private:

	statistics_t() {}
	statistics_t(const statistics_t &);

	~statistics_t() {}

}; // class statistics_t

#endif // Statistics_hh
