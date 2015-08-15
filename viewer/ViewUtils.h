/*----------------------------------------------------------------------------*
 * Copyright (c) 2012 Los Alamos National Security, LLC
 * All rights reserved
 *----------------------------------------------------------------------------*/

#ifndef ViewUtils_hh
#define ViewUtils_hh

#include <iostream>

#define ASSERT(v,s)																	\
	if(!(v)) {																			\
		std::cerr << "ASSERTION FAILED: " << s << "!!!" << std::endl;	\
	} // if

#endif // ViewUtils_hh
