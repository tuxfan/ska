/*----------------------------------------------------------------------------*
 * Copyright (c) 2012 Los Alamos National Security, LLC
 * All rights reserved
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
 * Definition of Errcodes
 *----------------------------------------------------------------------------*/

#ifndef ErrCodes_hh
#define ErrCodes_hh

namespace ska {

	static const int32_t AssertionFailed(-10000);
	static const int32_t UnknownCase(-10001);
	static const int32_t BadArgument(-10002);
	static const int32_t BadFileOperation(-10003);
	static const int32_t OpenCLError(-10004);
	static const int32_t MapKeyDefined(-10005);
	static const int32_t UndefinedMapKey(-1006);
	static const int32_t BadInputValue(-1007);
	static const int32_t XercesError(-1008);
	static const int32_t LLVMError(-1009);
	static const int32_t MemoryAllocationError(-1010);

} // namespace ska

#endif // ErrCodes_hh

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
