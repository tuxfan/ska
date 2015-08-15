/*----------------------------------------------------------------------------*
 * Copyright (c) 2012 Los Alamos National Security, LLC
 * All rights reserved
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
 * Statistics class.
 *----------------------------------------------------------------------------*/

#ifndef Statistics_h
#define Statistics_h

#include <map>

namespace ska {

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

} // namespace ska

#endif // Statistics_h

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
