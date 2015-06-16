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


typedef std::map<llvm::Value *, dependency_t *> dependency_map_t;

class flow_graph{

private :


          typedef struct data {
                int * regList;
                bool * liveInfo;
                llvm::Value inst;
          } data;

          typedef std::vector<data *> BB;

          typedef struct node {
                BB BBInfo;
                std :: vector <struct node *>
                       branches;
          } node;

          int ** iGraph;
          node * rootBB;

public :

          flow_graph( dependency_map_t dmap,
                        int n, llvm::Module::iterator
                        fita); //creates the CFG
          void liveness_flow(){ ; } //populates liveness info
          void build_iGraph() { ; } //interference
                                    //graph

};  //flowgraph

flow_graph::flow_graph(dependency_map_t dmap, int n,
                        llvm::Module::iterator fita){

        rootBB = new node; //root basic block
        auto bita = (*fita).begin(); //basic block iterator
        auto iita = (*bita).begin(); //instruction iterator
        
        while (iita != (*bita).end()){ //iterate through rootBB
                iita++;
        }
}

} //namespace ska

#endif
