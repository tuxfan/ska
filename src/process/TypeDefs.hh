#ifndef TypeDefs_hh
#define TypeDefs_hh


namespace ska {


typedef std::map<llvm::Value *,bool> intf; //represents adjacency
typedef std::map<llvm::Value *, dependency_t *> dependency_map_t;
typedef int numPhys; //number of physical registers


}

#endif
