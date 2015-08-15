#ifndef TypeDefs_h
#define TypeDefs_h


namespace ska {


typedef std::map<llvm::Value *,bool> intf; //represents adjacency
typedef std::map<llvm::Value *, dependency_t *> dependency_map_t;
typedef int numPhys; //number of physical registers
typedef int reg_type; //register type


}

#endif // TypeDefs_h
