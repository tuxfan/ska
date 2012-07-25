/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/

#ifndef Statistics_hh
#define Statistics_hh

#include <map>

class statistics_t : public std::map<std::string, size_t>
{
public:

	static statistics_t & instance() {
		static statistics_t m;
		return m;
	} // instance

	void reset() {
		clear();
	} // reset

private:

	statistics_t() {}
	statistics_t(const statistics_t &);

	~statistics_t() {}

}; // class statistics_t

#endif // Statistics_hh
