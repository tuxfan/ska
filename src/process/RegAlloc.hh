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

          std::map<llvm::Value *,bool> regCover;//indicate whether
                                                //value was covered
                                                //during liveness
                                                //analysis
          typedef struct live_info{
                  std::map<llvm::Value *,bool> live_in;
                  std::map<llvm::Value *,bool> live_out;
          } live_info; //typedef one row of the liveness table

          std::map<llvm::Value *, live_info> live_tab ; //liveness table

          typedef std::map<llvm::Value *,bool> intf; //represents adjacency
          std::map<llvm::Value *, intf> intf_table; // matrix 
          std::map<llvm::Value *,std::list<llvm::Value*>> intf_list;
                                                    //adj. list
          llvm::Value * root; //the root instruction


public :
          flow_graph(dependency_map_t dmap,
                        int n, llvm::Module::iterator
                        fita); //creates the CFG

          int liveness_flow(llvm::Value * op,
                                  llvm::ilist_iterator<llvm::Instruction> iita,
                                    llvm::BasicBlock * bita) ;
                                    //populates liveness info
          void build_iGraph() ; //interference
                                    //graph
          void traverse_list(std::map<llvm::Value *,bool> imap);
                                //basically detects all pairs
                                //and adds to the intf_table 

};  //flowgraph

flow_graph::flow_graph(dependency_map_t dmap, int n,
                        llvm::Module::iterator fita){

        //rootBB = new node; //root basic block
        auto aita = fita->arg_begin();//argument iterator
        auto bita = (*fita).begin(); //basic block iterator
        auto iita = (*bita).begin(); //instruction iterator
        root = iita; //store the root iita
        int name_count=0;

        while (iita != (*bita).end()){ //iterate through rootBB
                                       //populate value map
               regCover[iita]=false;   //no coverage on init
               iita->dump(); //debug
               iita++;
        }

        iita--;

        while (iita != (*bita).begin()){//need to add multiple BB
                                        //liveness analysis
               iita->dump();
               int numOp = iita->getNumOperands();
               while(numOp>0){
                        auto xx = (iita->getOperand(numOp-1));
                        if (regCover.find(xx) != regCover.end()){
                                  auto init=iita;
                                  liveness_flow(xx,init,
                                                   bita);
                        }
                        numOp--;
               }
               iita--;
        }


       //build_iGraph();

} //flow_graph constructor

int flow_graph::liveness_flow(llvm::Value * op,
                     llvm::ilist_iterator<llvm::Instruction> iita,
                                  llvm::BasicBlock * bita){

          iita->dump(); // debug
          op->dump(); //debug
          if(regCover[op]==true) return 0;
          else if (iita == (*bita).begin()) {
                      live_tab[iita].live_in[op]=true; //instr where operand first
                      live_tab[iita].live_out[op]=false; //used is live in but not
                      regCover[op]=true;
          }
          else{
                      live_tab[iita].live_in[op]=true; //instr where operand first
                      live_tab[iita].live_out[op]=false; //used is live in but not
                                                      //live out
                      iita--;
                      while(iita != (*bita).begin()){
                                              //populate the liveness
                                              //table
                                iita->dump();//debug
                                if (iita==op) {
                                          live_tab[iita].live_in[op]=false;
                                          live_tab[iita].live_out[op]=true;
                                          iita = (*bita).begin(); //breaks out
                                }
                                else{
                                          live_tab[iita].live_in[op]=true;
                                          live_tab[iita].live_out[op]=true;
                                          iita--;
                                }
                      }
                      regCover[op]=true;
          }
} //liveness flow

void flow_graph::build_iGraph(){
       std::map<llvm::Value *, live_info>::iterator it =
                                        live_tab.begin();
       while (it != live_tab.end()){
             auto liv_in = (it->second).live_in;
             auto liv_out = (it->second).live_out;
             traverse_list(liv_in);
             traverse_list(liv_out);
             it++;
       }
}

void flow_graph::traverse_list( std::map<llvm::Value *,bool> imap){
         std::map<llvm::Value *,bool>::iterator it_0 = imap.begin();
         while (it_0 != imap.end()){
                std::map<llvm::Value *,bool>::iterator it_1 =
                                                imap.begin();
                while(it_1 != imap.end()){
                       auto val_0 = it_0->first ;
                       auto val_1 = it_1->first ;
                       intf_table[val_0][val_1] = true;
                       it_1++;
                }
                it_0++;
         }
} // traverse_list

} //namespace ska

#endif
