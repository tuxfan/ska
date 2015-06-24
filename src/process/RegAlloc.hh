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

//#include <iostream>
//#include <fstream> //for writing debug files



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

          typedef std::map<llvm::Value *,bool> tree_list;

          std::map<llvm::Value *, tree_list> BB_livin; //livein mapped
                                                      //to basic block
          


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
          std::ofstream debug_liv; //debug liveness info


          int print_debug(std::map<llvm::Value *, bool> mm);
                                                  //liveness debug info

          std::map<llvm::Value * ,bool> //a map of instructions
            BBLiveness
                      (llvm::ilist_iterator<llvm::Instruction> iita, 
                                  llvm::BasicBlock * bita   );


          std::map<llvm::Value * ,bool> //a map of instructions
           recur_live 
                      (llvm::ilist_iterator<llvm::Instruction> iita, 
                                  llvm::BasicBlock * bita   );

          bool check_livein();

};  //flowgraph

flow_graph::flow_graph(dependency_map_t dmap, int n,
                        llvm::Module::iterator fita){

        //rootBB = new node; //root basic block
        auto aita = fita->arg_begin();//argument iterator
        auto bita = (*fita).begin(); //basic block iterator
        auto iita = (*bita).begin(); //instruction iterator
        root = iita; //store the root iita
        int name_count=0;

        debug_liv.open("Liveness_Analysis");
        debug_liv << "Printing instructions in IR "<< std::endl;

        while (aita != fita->arg_end()){
               regCover[aita]=false; //add operands to regCover
               aita++;
        }

        while (iita != (*bita).end()){ //iterate through rootBB
                                       //populate value map
               iita->dump();
               regCover[iita]=false;   //no coverage on init
               std::string str;
               llvm::raw_string_ostream rso(str);
               rso<<*iita;
               debug_liv << rso.str();
               debug_liv << std::endl;

               iita++;
        }


        debug_liv << std::endl
                  << "Now printing instructions covered by"
                  << " liveness analysis "
                  << std::endl;


        auto bb_livin = recur_live(iita,bita);
                
        debug_liv << std::endl
                  << "Now printing liveness tables"
                  << std::endl;

        auto debug_it = bita->end();
        while (debug_it != bita->begin()){
                debug_it--;
                if (live_tab.find(debug_it)==live_tab.end())
                          continue;
                else{
                           debug_liv << "----------" << std::endl;
                                                      //start of table entry
                           std::string str;
                           llvm::raw_string_ostream rso(str);
                           rso<<*debug_it;
                           debug_liv << rso.str();
                           debug_liv << std::endl;

                           auto row = live_tab[debug_it];
                           auto row_1 = row.live_in;
                           auto row_2 = row.live_out;

                           debug_liv << " ** Live In Values ** " << std::endl;
                           print_debug(row_1);

                           debug_liv << " ** Live Out Values **" << std::endl;
                           print_debug(row_2);
                           debug_liv << "----------" << std::endl;
                                                      // end of tab entry
                }
        }


        //build_iGraph();

        debug_liv.close(); //end of liveness debug

} //flow_graph constructor

std::map<llvm::Value * ,bool> //a map of instructions
 flow_graph::BBLiveness(llvm::ilist_iterator<llvm::Instruction> iita, 
                                  llvm::BasicBlock * bita   ){

        //take input of live variables that are live in at subsequent basic 
        //blocks
        //check until where they are live in this basic block and construct 
        //intf. graph
        //accordingly

        while (iita != (*bita).begin()){//need to add multiple BB
                                        //liveness analysis

               iita--;
               iita->dump();
               int numOp = iita->getNumOperands();
               while(numOp>0){
                        auto xx = (iita->getOperand(numOp-1));
                        xx->dump();
                        if (regCover.find(xx) != regCover.end()){
                                  auto init=iita;
                                  liveness_flow(xx,init,
                                                   bita);
                        }
                        numOp--;
               }
        }

        return  live_tab[bita->begin()].live_in; //live_in info
                                                 //for the BB

}


std::map<llvm::Value * ,bool> //a map of instructions
 flow_graph::recur_live(llvm::ilist_iterator<llvm::Instruction> iita, 
                                  llvm::BasicBlock * bita ){

        auto bb_livin = BBLiveness(iita,bita);
                               //get livenesss info for this BB
                               //populates the liveness table
                               //with information from the BB
                               //we need to return liveness
                               //i.e. live_in for this BB
        iita=bita->end();
        iita--; //the last instruction
        bool live_in_change = check_livein(); //checks if the live
                                              //in info changed
        if (live_in_change){
                  unsigned opcode = ((llvm::Instruction *)iita)->getOpcode();
                  if (opcode == llvm::Instruction::Br){
                             
                  }
        }else{
                  return bb_livin;
        }
}

bool flow_graph::check_livein(){

          return false;

};

int flow_graph::print_debug(std::map<llvm::Value *, bool> mm){

          auto it = mm.begin();
          while (it != mm.end()){
                    std::string str;
                    llvm::raw_string_ostream rso(str);
                    rso<<*(it->first);
                    debug_liv << rso.str();

                    if(it->second == true)
                            debug_liv << "  --- true";
                    if(it->second == false)
                            debug_liv << "  --- false";

                    debug_liv << std::endl;
                    it++;
          }
          return 0;
}

int flow_graph::liveness_flow(llvm::Value * op,
                     llvm::ilist_iterator<llvm::Instruction> iita,
                                  llvm::BasicBlock * bita){

          //iita->dump(); // debug
          //op->dump(); //debug
          if(regCover[op]==true) return 0;
          else if (iita == (*bita).begin()) {
                      std::string str;
                      llvm::raw_string_ostream rso(str);
                      rso<<*iita;
                      debug_liv << rso.str();
                      debug_liv << std::endl;
                      live_tab[iita].live_in[op]=true; //instr where operand first
                      live_tab[iita].live_out[op]=false; //used is live in but not
                      regCover[op]=true;
          }
          else{
                      std::string str;
                      llvm::raw_string_ostream rso(str);
                      rso<<*iita;
                      debug_liv << rso.str();
                      debug_liv << std::endl;
                      live_tab[iita].live_in[op]=true; //instr where operand first
                      live_tab[iita].live_out[op]=false; //used is live in but not
                                                      //live out
                      while(iita != (*bita).begin()){
                                              //populate the liveness
                                              //table
                                //iita->dump();//debug

                                if (iita==op) {
                                          iita--;
                                          live_tab[iita].live_in[op]=false;
                                          live_tab[iita].live_out[op]=true;
                                          iita = (*bita).begin(); //breaks out
                                }
                                else{
                                          iita--;
                                          live_tab[iita].live_in[op]=true;
                                          live_tab[iita].live_out[op]=true;
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

       //debug printing loop below

       std::ofstream debug_intf;
       debug_intf.open("Interference Graph");

       debug_intf << "Printing interference table below " <<
                                                               std::endl;

       std::map<llvm::Value *,bool>::iterator reg_it_0= regCover.begin();
       while (reg_it_0 != regCover.end()){
              std::map<llvm::Value *,bool>::iterator reg_it_1 =
                                            regCover.begin();
              if (intf_table.find(reg_it_0->first) != intf_table.end()){
                    while ( reg_it_1 != regCover.end()){
                             if(intf_table[reg_it_0->first].
                                    find(reg_it_1->first) !=
                                    intf_table[reg_it_0->first].end()){

                                       auto val_1 = reg_it_0->first;
                                       auto val_2 = reg_it_1->first;
                                       bool tt =
                                            intf_table[val_1][val_2];

                                      std::string str_0;
                                      llvm::raw_string_ostream rso_0(str_0);
                                      rso_0<<*(reg_it_0->first);
                                      debug_intf << rso_0.str() << " , ";


                                      std::string str_1;
                                      llvm::raw_string_ostream rso_1(str_1);
                                      rso_1<<*(reg_it_1->first);
                                      debug_intf << rso_1.str() << "  ";


                                      if (tt==true){
                                                debug_intf << " X " <<
                                                          std::endl;
                                       }
                                       else     debug_intf << " O " << //never
                                                          std::endl ;//called

                             }
                             reg_it_1++;
                    }

              }
              reg_it_0++;
       }

       debug_intf.close();
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
