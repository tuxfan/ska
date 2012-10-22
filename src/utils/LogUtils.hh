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

#include <FileIO.hh>
#include <ErrCodes.hh>
#include <ColorUtils.hh>

namespace ska {

#if defined(ENABLE_DEBUG)
#define DEBUG(s) \
	file_io_t::instance().log_stream() << s << std::endl
#else
#define DEBUG(s)
#endif

// utility function
template<char C>
std::string rstrip(const char * file) {
	std::string tmp(file);
	return tmp.substr(tmp.rfind(C)+1);
} // rstrip

#define ExitOnError(s, e)															\
	file_io_t::instance().log_stream() << OUTPUT_RED(s) << std::endl;	\
   file_io_t::instance().log_stream() << std::flush;						\
	std::exit((e));

#define Warn(s)																	\
	file_io_t::instance().log_stream() <<									\
		OUTPUT_YELLOW("Warning: ") << OUTPUT_CYAN(s) << std::endl;

// Assert
#define Assert(b, s)									\
	if(!(b)) {											\
		ExitOnError("Assertion Failed " << s,	\
		ska::AssertionFailed);						\
	} // if

// WeakAssert
#define WeakAssert(b, s)								\
	if(!(b)) {												\
		file_io_t::instance().log_stream() <<		\
		OUTPUT_YELLOW("Warning: ") <<					\
		OUTPUT_RED("weak assertion failed ") <<	\
		OUTPUT_BROWN(s) << std::endl;					\
	} // if

} // namespace ska

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
