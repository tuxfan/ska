/*----------------------------------------------------------------------------*
 * Copyright (c) 2012 Los Alamos National Security, LLC
 * All rights reserved
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
 * Logging Utilities
 *----------------------------------------------------------------------------*/

#ifndef LogUtils_hh
#define LogUtils_hh

#include <iostream>
#include <map>
#include <cstdarg>
#include <cstddef>
#include <string>

#include <ErrCodes.hh>

#if defined(ENABLE_DEBUG)
#define DEBUG(s) \
	std::cerr << s << std::endl
#else
#define DEBUG(s)
#endif

// utility function
template<char C>
std::string rstrip(const char * file) {
	std::string tmp(file);
	return tmp.substr(tmp.rfind(C)+1);
} // rstrip

#define ExitOnError(s, e)			\
	std::cerr << s << std::endl;	\
   std::cerr << std::flush;		\
	std::exit((e));

// Assert
#define Assert(b, s)									\
	if(!(b)) {											\
		ExitOnError("Assertion Failed: " << s,	\
		ska::AssertionFailed);						\
	} // if

#endif // LogUtils_hh

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
