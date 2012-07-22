/*----------------------------------------------------------------------------*
 *----------------------------------------------------------------------------*/

#ifndef Metric_hh
#define Metric_hh

#include <map>

class metric_t : public std::map<std::string, size_t>
{
public:

	static metric_t & instance() {
		static metric_t m;
		return m;
	} // instance

	void reset() {
		clear();
	} // reset

private:

	metric_t() {}
	metric_t(const metric_t &);

	~metric_t() {}

}; // class metric_t

#endif // Metric_hh
