/*----------------------------------------------------------------------------*
 * Copyright (c) 2012 Los Alamos National Security, LLC
 * All rights reserved
 *----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*
 * std::string => llvm::Type
 *----------------------------------------------------------------------------*/

#ifndef OpTypes_h
#define OpTypes_h

#include <map>
#include <limits>

#include <llvm/IR/Instructions.h>

namespace ska {

static const unsigned TypeAnyID = std::numeric_limits<unsigned>::max();

unsigned any_type() {
	return TypeAnyID;
} // dummy_type

std::map<std::string, unsigned> type_map = {
	// Primitive Types
	{ "void",			llvm::Type::VoidTyID },
	{ "half",			llvm::Type::HalfTyID },
	{ "float",			llvm::Type::FloatTyID },
	{ "double",			llvm::Type::DoubleTyID },
	{ "x87",				llvm::Type::X86_FP80TyID },

	// Derived Types
	{ "integer",		llvm::Type::IntegerTyID },
	{ "vector",			llvm::Type::VectorTyID },

	// Custom Types
	{ "any",				TypeAnyID }
};

} // namespace ska

#endif // OpTypes_h

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
