/*~--------------------------------------------------------------------------~*
 *~--------------------------------------------------------------------------~*/

#ifndef TypeDefs_h
#define TypeDefs_h

#include "../arch/RegisterSet.h"

namespace ska {

	typedef std::map<llvm::Value *,bool> intf; //represents adjacency
	typedef std::map<llvm::Value *, dependency_t *> dependency_map_t;

	// FIXME
	//typedef int reg_type; //register type
  using numPhys = size_t;
	using reg_type = register_set_t::register_type_t;

} // namespace ska

#endif // TypeDefs_h

/*~-------------------------------------------------------------------------~-*
 * vim: tabstop=2 shiftwidth=2 expandtab :
 *~-------------------------------------------------------------------------~-*/
