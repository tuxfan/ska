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

namespace ska {

class select {
 public:
  typedef int color;  // color
  typedef int reg_type;  // register type

  typedef std::pair<llvm::Value *, bool> spill_info;

  select(std::stack<spill_info> ss, std::map<llvm::Value *, intf> intf_table,
         std::map<reg_type, numPhys> reg_map,
         llvm::Module::iterator fita);  // select constructor
  // selects colors for each instruction
  // inserts instructions if there are
  // actual spills and iterates regAlloc
  // in the hopes of getting a legal
  // allocation. If no actual spills,
  // then we are done

  std::stack<std::pair<llvm::Value *, bool>> pot_spill;
  // stores only potential spills
  bool checkOperand(llvm::Value *, llvm::Value *);
  bool modifyOperand(llvm::Value *, llvm::Value *, llvm::Value *);
  void populate_starting_colors(std::map<int,int> reg_map);
  bool return_flag();

 private:
  std::map<llvm::Value *, intf> new_intf_table;
  std::map<llvm::Value *, color> reg_color;
  // final register colors

  std::map<reg_type, std::map<color, bool>> phys_reg_list;
  // says which colors correspond
  // to a particular register type
  // in particular,

  std::map<reg_type, int> c_map;  // the starting color for this reg type
  std::map<llvm::Value *, reg_type> inst_reg;
  std::map<llvm::Value *, llvm::Value *> alloca_map;
  bool stop_flag;

};

void select::populate_starting_colors(std::map<int,int> reg_map) {


  int register_size = reg_map.size();
  c_map[0]=1; //starting color is 1
  for (int i=0; i<register_size;i++){
      c_map[i+1]= c_map[i] + reg_map[i];
  }

  //c_map[1] = c_map[0]+reg_map[0];  // float type has colors 101-200
  //c_map[2] = c_map[1]+reg_map[1];
  //c_map[3] = c_map[2]+reg_map[2];

  // ideally, this should be read in from
  // the reg_set class
  // or from xml
}

select::select(std::stack<spill_info> ss,
               std::map<llvm::Value *, intf> intf_table,
               std::map<reg_type, numPhys> reg_map,
               llvm::Module::iterator fita) {
  // pop instructions out of the ss stack
  // if they are simple nodes, just assign a
  // color and proceed. If they are potential
  // spills, then make a separate list i.e.
  // assign colors to those in the end. If
  // it turns out that they are colorable later
  // on, then we are done. Otherwise need to
  // rewrite program and redo the reg allocation

  stop_flag = true;  // we are optimistic

  populate_starting_colors(reg_map);

  std::ofstream node_colors;
  node_colors.open("simple_node_colors");

  while (ss.size() != 0) {
    auto ii = ss.top();  // pop the instruction
    ss.pop();
    if (ii.second == true) {  // no spill, random color possible
      int type = ska::getInstructionType(ii.first);
      color cc = c_map[type];
      color max_c = -1;
      intf::iterator intf_it = intf_table[ii.first].begin();
      // ii.first->dump();

      while (intf_it != intf_table[ii.first].end()) {
        color temp_c;
        //(intf_it->first)->dump();
        if (reg_color.find(intf_it->first) != reg_color.end()) {
          temp_c = reg_color[intf_it->first];
          // printf(" : Colour is %d",temp_c);
        } else {
          temp_c = -1;
        }  // printf(" : Colour not assigned \n"); }
        if ((temp_c >  max_c) && (temp_c >= cc) &&
            (temp_c < cc + reg_map[type]))  // change to correct mapping
          max_c = temp_c;
        intf_it++;
      }
      if (max_c == -1)
        reg_color[ii.first] = c_map[type];
      else
        reg_color[ii.first] = max_c + 1;

      /*std::string str; //prints in the opposite order
      llvm::raw_string_ostream rso(str);
      rso<<*(ii.first);
      node_colors<< rso.str();
      node_colors << "  ";
      node_colors << reg_color[ii.first];
      node_colors << std::endl;*/

    } else {  // potential spill, color in the end
      // do nothing here
      pot_spill.push(ii);
    }
  }

  node_colors.close();
  node_colors.open("Coloring spill nodes");
  node_colors << "Could not color, so added load store, possibly alloca";

  while (pot_spill.size() != 0) {
    //printf ("Looking to color a spilled node ! \n");

    auto ii = pot_spill.top();
    pot_spill.pop();
    int type = ska::getInstructionType(ii.first);
    color cc = c_map[type];
    color max_c = -1;
    intf::iterator intf_it = intf_table[ii.first].begin();

    while (intf_it != intf_table[ii.first].end()) {
      color temp_c;
      if (reg_color.find(intf_it->first) != reg_color.end())
        temp_c = reg_color[intf_it->first];
      else
        temp_c = -1;
      if ((temp_c > max_c) && (temp_c >= cc) &&
          (temp_c < cc + reg_map[type]))  // change to correct mapping
        max_c = temp_c;
      intf_it++;
    }

    //printf("Max color of the surrounding nodes is %d \n", max_c);

    if (max_c == -1) {
      reg_color[ii.first] = cc;
    } else
    if (max_c ==
        cc + reg_map[type] - 1) {// means we cannot find a color to assign
      // so make tiny live ranges and remove
      // large live range for the instruction
      llvm::Instruction *ii_1 = (llvm::Instruction *)ii.first;
      // llvm::AllocaInst* ai = new llvm::AllocaInst(ii_1->type);//framepointer
      // or reference address

      //printf ("Could not color a spilled node !\n");
      //ii_1->dump();
      auto bita = fita->begin();

      llvm::Function::ArgumentListType &args = fita->getArgumentList();

      for (auto args_it = args.begin(); args_it != args.end(); args_it++) {
        alloca_map[args_it] = bita->begin();
        // args_it->dump();
      }

      while (bita != fita->end()) {
        auto iita = bita->begin();
        int count = 0;
        while (iita != bita->end()) {
           //ii_1->dump();
           //iita->dump();
          unsigned opcode = iita->getOpcode();

          if (opcode == llvm::Instruction::Alloca) {
            alloca_map[iita] = iita;
          }

          if ((llvm::Instruction *)iita == ii_1) {
            //printf ("Created new alloca !\n");
            auto iita_0 = iita;
            auto iita_1 = ++iita;
            iita = iita_0;
            llvm::AllocaInst *ai = new llvm::AllocaInst((iita->getType()));
            alloca_map[iita] = ai;
            bita->getInstList().insert(iita_1, ai);
            llvm::StoreInst *si = new llvm::StoreInst(iita, ai, iita_1);
            iita++;
            iita++;
          }

          else if (checkOperand(iita,
                                ii_1)) {  // checkOperand does not work properly
            // need to investigate with simple test case

            //printf ("adding load and store \n");
            auto iita_0 = iita;
            auto iita_1 = ++iita;
            iita = iita_0;

            if (alloca_map.find(ii_1) == alloca_map.end()) {
              iita++;
              continue;
            }  // not a real reg

            llvm::LoadInst *li =
                new llvm::LoadInst((alloca_map.begin())->second, "LOAD", iita);
            modifyOperand(iita, ii_1, li);  // need to check this also
            llvm::StoreInst *si =
                new llvm::StoreInst(li, (alloca_map.begin()->second), iita_1);
            // there should be a frame pointer offset which we have
            // ignored here
            // with simple test cases
            iita++;
          }
          iita++;
          // count++;
        }
        bita++;
      }


      std::string str;
      llvm::raw_string_ostream rso(str);
      rso << *(ii.first);
      node_colors << rso.str();

      node_colors << std::endl;
      stop_flag = false;

    }

    // means there is an actual spill
    // so need to push it to mem after definition
    // and get load it from mem before use, so that
    // there are two tiny live ranges, instead of one large
    // live range

    else {
      reg_color[ii.first] = max_c + 1;

      std::string str;
      llvm::raw_string_ostream rso(str);
      rso << *(ii.first);
      /*node_colors<< rso.str();

      node_colors << "  ";
      node_colors << (max_c+1);
      node_colors << std :: endl;*/
    }
  }

  node_colors.close();

}  // select constructor

bool select::checkOperand(llvm::Value *ii, llvm::Value *i_check) {
  // ii->dump();
  // i_check->dump();
  auto it = ((llvm::Instruction *)ii)->op_begin();
  //(*it)->dump();
  while (it != ((llvm::Instruction *)ii)->op_end()) {
    //(*it)->dump();
    // i_check->dump();
    if ((*it) == (llvm::Instruction *)i_check) {
      return true;
    }
    it++;
  }
  return false;
}

bool select::modifyOperand(llvm::Value *ii, llvm::Value *i_check,
                           llvm::Value *i_modify) {
  auto it = ((llvm::Instruction *)ii)->op_begin();
  while (it != ((llvm::Instruction *)ii)->op_end()) {
    //(*it)->dump();
    // i_check->dump();
    if ((*it) == i_check) {
      *it = i_modify;  // changed the operand
    }
    it++;
  }
  return false;
}

bool select::return_flag() { return stop_flag; }

}  // namespace ska

#endif
