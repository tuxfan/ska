#ifndef RegAlloc_hh
#define RegAlloc_hh

#include <string>
#include <cstring>
#include <vector>
#include <list>

#include <llvm/IR/Module.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Instruction.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/ADT/APInt.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/SourceMgr.h>
#include <llvm/IRReader/IRReader.h>
#include <llvm/IR/InstIterator.h>

#include <FileIO.hh>
#include <Decode.hh>
#include <Instruction.hh>
#include <MachineState.hh>
#include <Statistics.hh>

#include <Dependency.hh>

#if defined(HAVE_GRAPHVIZ)
#include <Graphviz.hh>
#endif

#include <OpCodes.hh>
#include <OpTypes.hh>
#include <Core.hh>
#include <Utils.hh>


namespace ska {

class flow_graph{

private :

          typedef std::vector<llvm::Value *> BB;

          typedef struct data {
                int * regList;
                bool * liveInfo;
          } data;

          typedef struct node {
                BB * BBInfo;
                data * dataInfo;
                std :: vector <struct Node *>
                       branches;
          } node;

          int ** iGraph;
          Node * rootBB;

public :

          flow_graph(){ ; } //creates the CFG
          void liveness_flow(){ ; } //populates liveness info
          void build_iGraph() { ; } //interference
                                         //graph

};  //flowgraph


} //namespace ska

#endif
