#ifndef Select_hh
#define Select_hh

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
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/InstIterator.h>
#include <llvm/IR/CFG.h>

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

namespace ska{

class select{

public :

          typedef int color; //color
          typedef int reg_type; //register type

          typedef std::pair<llvm::Value *,bool> spill_info;

          select(std::stack<spill_info> ss,std::map<llvm::Value *,intf > intf_table,
                             std::map<reg_type, numPhys> reg_map,
                             llvm::Module::iterator fita ); //select constructor
                    //selects colors for each instruction
                    //inserts instructions if there are
                    //actual spills and iterates regAlloc
                    //in the hopes of getting a legal
                    //allocation. If no actual spills,
                    //then we are done

          std::stack<std::pair<llvm::Value *,bool>> pot_spill;
                    //stores only potential spills
          bool checkOperand( llvm::Value *, llvm::Value *);
          bool modifyOperand( llvm::Value *, llvm::Value *, llvm::Value*);


private :

          std::map<llvm::Value *, intf> new_intf_table;
          std::map<llvm::Value *, color> reg_color;
                                    //final register colors
          std::map<reg_type, std::map<color,bool>> phys_reg_list;
                                    //says which colors correspond
                                    //to a particular register type
                                    //in particular,
          std::map<reg_type,int> c_map; //the starting color for this reg type
          std::map<llvm::Value *, reg_type> inst_reg;
          std::map<llvm::Value *, llvm::Value *> alloca_map;


};


select::select(std::stack<spill_info> ss,std::map<llvm::Value *,intf> intf_table,
                            std::map<reg_type, numPhys> reg_map,
                            llvm::Module::iterator fita   ){


          //pop instructions out of the ss stack
          //if they are simple nodes, just assign a
          //color and proceed. If they are potential
          //spills, then make a separate list i.e.
          //assign colors to those in the end. If
          //it turns out that they are colorable later
          //on, then we are done. Otherwise need to
          //rewrite program and redo the reg allocation

          while( ss.size() != 0 ){
                   auto ii = ss.top(); //pop the instruction
                   ss.pop();
                   if(ii.second==false){ //no spill, random color possible
                            color cc = c_map[0];//change to reg type of
                                                //instruction obtained
                                                //from xml
                            color min_c = 9999;
                            intf::iterator intf_it = intf_table[ii.first].begin();
                            while (intf_it != intf_table[ii.first].end()){
                                      color temp_c = reg_color[intf_it->first] ;
                                      if((temp_c < min_c) && (temp_c >= cc) &&
                                                    (temp_c < cc+reg_map[0]) ) //change to correct mapping
                                                min_c=temp_c;
                                      intf_it++;
                            }
                            if ( min_c = 9999) c_map[inst_reg[ii.first]]; 
                            reg_color[ii.first]=min_c;
                   }else{ //potential spill, color in the end
                          //do nothing here

                          color cc = c_map[0];//change to reg type of 
                                                //instruction obtained
                                                //from xml
                          pot_spill.push(ii);
                          color min_c = 9999;
                          intf::iterator intf_it = intf_table[ii.first].begin();
                          while (intf_it != intf_table[ii.first].end()){
                                    color temp_c = reg_color[intf_it->first] ;
                                    if((temp_c < min_c) && (temp_c >= cc) &&
                                                  (temp_c < cc+reg_map[0]) ) //change to correct mapping
                                              min_c=temp_c;
                                    intf_it++;
                          }
                   }
          }

          while( pot_spill.size() != 0){
                    auto ii = pot_spill.top();
                    pot_spill.pop();
                    color cc = c_map[0]; //change to reg type of
                                         //instruction obtained
                                         //from xml
                    color max_c = -1;
                    intf::iterator intf_it = intf_table[ii.first].begin();
                    while (intf_it != intf_table[ii.first].end()){
                              color temp_c = reg_color[intf_it->first] ;
                              if((temp_c > max_c) && (temp_c >= cc) &&
                                            (temp_c < cc+reg_map[0]) ) //change to correct mapping
                                        max_c=temp_c;
                              intf_it++;
                    }
                    if (max_c == cc+reg_map[0]){ //change to correct mapping

                              llvm::Instruction * ii_1 = ( llvm::Instruction *)intf_it->first;

                              auto bita = fita->begin();
                              while (bita != fita->end()){

                                        auto iita = bita->begin();
                                        while (iita != bita->end()){
                                                  if((llvm::Instruction *)iita == ii_1){

                                                           auto iita_0= iita ;
                                                           auto iita_1= iita++;
                                                           iita = iita_0;
                                                           llvm::AllocaInst* ai = new llvm::AllocaInst(iita->getType()); 
                                                           alloca_map[iita]=bita;
                                                           bita->getInstList().insert(iita_1, ai);
                                                           llvm::StoreInst *si = new llvm::StoreInst(iita,ai,iita_1);
                                                  }

                                                  if(checkOperand(iita,ii_1)){

                                                           auto iita_0= iita ;
                                                           auto iita_1= iita++;
                                                           iita = iita_0;
                                                           llvm::LoadInst *li = new llvm::LoadInst(alloca_map[iita], "LOAD",iita );
                                                           llvm::StoreInst *si = new llvm::StoreInst(li,iita,iita_1);
                                                           modifyOperand(iita,ii_1,li);
                                                  }
                                                  iita++;
                                        }
                                        bita++;
                              }

                    } //means there is an actual spill
                      //so need to push it to mem after definition
                      //and get load it from mem before use, so that
                      //there are two tiny live ranges, instead of one large
                      //live range


          }



          //doRegAlloc(); //need to do register allocation again
                        //we can invoke this from Simulator.hh
                        //and instead return a bool flag here



}


bool select::checkOperand( llvm::Value * ii, llvm::Value * i_check){

         auto it = ((llvm::Instruction *)ii)->op_begin();
         while (it != ((llvm::Instruction *)ii)->op_end()){

                    if((llvm::Instruction *)it == (llvm::Instruction *)i_check){

                         return true;
                    }
                    it++;

         }
         return false;

}

bool select::modifyOperand( llvm::Value * ii, llvm::Value * i_check,
                                llvm :: Value * i_modify){
         auto it = ((llvm::Instruction *)ii)->op_begin();
         while (it != ( (llvm::Instruction *)ii)->op_end()){

                    if((llvm::Instruction*)it == (llvm::Instruction *)i_check){

                          it = (llvm::Use *) i_modify;  //changed the operand
                    }
                    it++;
         }
         return false;
}

} //namespace ska

#endif
