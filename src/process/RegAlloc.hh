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
#include <llvm/IR/CFG.h>
#include <llvm/IR/Constants.h>

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

#include <Simplify.hh>  //RegAlloc headers
#include <Select.hh>

namespace ska {

// typedef std::map<llvm::Value *, dependency_t *> dependency_map_t;

class flow_graph {
 private:
  std::map<llvm::Value *, bool> regCover;  // indicate whether
  // value was covered
  // during liveness
  // analysis
  typedef struct live_info {
    std::map<llvm::Value *, bool> live_in;
    std::map<llvm::Value *, bool> live_out;
  } live_info;  // typedef one row of the liveness table

  std::map<llvm::Value *, live_info> live_tab;  // liveness table

  // typedef std::map<llvm::Value *,bool> intf; //represents adjacency
  std::map<llvm::Value *, intf> intf_table;  // matrix
  std::map<llvm::Value *, std::list<llvm::Value *>> intf_list;
  // adj. list
  llvm::Value *root;  // the root instruction

  typedef std::map<llvm::Value *, bool> tree_list;

  std::map<llvm::Value *, tree_list> BB_livin;  // livein mapped
  // to basic block
  dependency_map_t dmap;

  simplify_nodes *simp_igraph;

  llvm::Module::iterator root_fita;

  register_set_t **rs;
  size_t register_sets;

 public:
  flow_graph(size_t register_sets, llvm::Module::iterator fita,
             llvm::Module::iterator end, register_set_t ** rs);  // creates the CFG

  int liveness_flow(llvm::Value *op,
                    llvm::ilist_iterator<llvm::Instruction> iita,
                    llvm::BasicBlock *bita);
  // populates liveness info
  void build_iGraph();  // interference
  // graph
  void traverse_list(std::map<llvm::Value *, bool> imap);
  // basically detects all pairs
  // and adds to the intf_table
  std::ofstream debug_liv;  // debug liveness info

  int print_debug(std::map<llvm::Value *, bool> mm);
  // liveness debug info

  std::map<llvm::Value *, bool>  // a map of instructions
      BBLiveness(llvm::BasicBlock *bita, tree_list live_in);

  void  // a map of instructions
      all_BB_liveness(llvm::Function *bita);

  bool check_livein(std::map<llvm::Value *, bool>,
                    llvm::BasicBlock *bb);  // check if there
  // was any change in
  // live_in when
  // recursing over BBs

  void simplify_iGraph() { simp_igraph = new simplify_nodes(intf_table,rs,
                                                 register_sets); };

  bool select_regs(llvm::Module::iterator fita);
  void empty_the_maps();

};  // flowgraph

flow_graph::flow_graph(size_t register_sets, llvm::Module::iterator fita,
                       llvm::Module::iterator end, register_set_t ** rs){

  this->register_sets = register_sets;
  this->rs = rs;

  // rootBB = new node; //root basic block
  auto aita = fita->arg_begin();  // argument iterator
  auto bita = (*fita).begin();  // basic block iterator
  auto iita = (*bita).begin();  // instruction iterator
  root = iita;  // store the root iita
  root_fita = fita;
  int name_count = 0;

  debug_liv.open("Liveness_Analysis");
  debug_liv << "Printing instructions in IR " << std::endl;
  debug_liv << std::endl
            << "Now printing instructions covered by"
            << " liveness analysis " << std::endl;

  auto fita_1 = fita;
  while (fita_1 != end) {
    all_BB_liveness(fita_1);  // get the
    printf("Function liveness done\n");
    fita_1++;
  }

  debug_liv << std::endl
            << "Now printing liveness tables" << std::endl;

  auto debug_it = bita->end();
  while (debug_it != bita->begin()) {
    debug_it--;
    if (live_tab.find(debug_it) == live_tab.end())
      continue;
    else {
      debug_liv << "----------" << std::endl;
      // start of table entry
      std::string str;
      llvm::raw_string_ostream rso(str);
      rso << *debug_it;
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

  // build_iGraph();

  debug_liv.close();  // end of liveness debug

}  // flow_graph constructor

std::map<llvm::Value *, bool>  // a map of instructions
    flow_graph::BBLiveness(llvm::BasicBlock *bita, tree_list live_in) {
  // take input of live variables that are live in at subsequent basic
  // blocks
  // check until where they are live in this basic block and construct
  // intf. graph
  // accordingly

  auto iita = bita->end();
  iita--;

  // iita->dump(); //debug
  // bita->dump(); //debug

  tree_list::iterator it = live_in.begin();
  while (it != live_in.end()) {
    auto init = iita;
    liveness_flow(it->first, iita, bita);
    it++;
  }

  while (iita != (*bita).begin()) {  // need to add multiple BB
    // liveness analysis

    iita--;
    // iita->dump();
    int numOp = iita->getNumOperands();
    while (numOp > 0) {
      auto xx = (iita->getOperand(numOp - 1));
      // xx->dump();
      // if (regCover.find(xx) != regCover.end()){
      llvm::ConstantInt *CI;
      auto init = iita;
      if (!(CI = llvm::dyn_cast<llvm::ConstantInt>(xx)))
        liveness_flow(xx, init, bita);
      //}
      numOp--;
    }
  }

  return live_tab[bita->begin()].live_in;  // live_in info
  // for the BB
}

void flow_graph::all_BB_liveness(llvm::Function *fita) {
  auto bita = fita->begin();
  tree_list empty_tree;
  while (bita != fita->end()) {
    BB_livin[bita] = BBLiveness(bita, empty_tree);
    bita++;
  }  // got initial liveness info

  bita = fita->begin();  // root BB
  bool stop_flag = false;

  while (!stop_flag) {  // iterate liveness calculations
    stop_flag = true;
    while (bita != fita->end()) {
      tree_list succ_map;
      auto sit = succ_begin(bita);
      while (sit != succ_end(bita)) {
        //(*sit)->dump(); //dump the BB
        tree_list live_in_succ = BB_livin[*sit];
        tree_list::iterator it = live_in_succ.begin();
        while (it != live_in_succ.end()) {
          succ_map[it->first] = true;
          it++;
        }
        sit++;
      }
      tree_list live_in_new = BBLiveness(bita, succ_map);
      stop_flag &= check_livein(live_in_new, bita);
      BB_livin[bita] = live_in_new;
      bita++;
    }
    bita = fita->begin();  // reset the bita
  }
}

bool flow_graph::check_livein(std::map<llvm::Value *, bool> mm,
                              llvm::BasicBlock *bb) {
  bool forward_check = true;
  std::map<llvm::Value *, bool>::iterator it_0 = mm.begin();
  while (it_0 != mm.end()) {
    if (BB_livin[bb].find(it_0->first) == BB_livin[bb].end())
      forward_check = false;
    break;
    it_0++;
  }

  bool backward_check = true;
  std::map<llvm::Value *, bool>::iterator it_1 = BB_livin[bb].begin();
  while (it_1 != BB_livin[bb].end()) {
    if (mm.find(it_1->first) == mm.end()) {
      backward_check = false;
      break;
    }
    it_1++;
  }
  return forward_check && backward_check;
};

int flow_graph::print_debug(std::map<llvm::Value *, bool> mm) {
  auto it = mm.begin();
  while (it != mm.end()) {
    std::string str;
    llvm::raw_string_ostream rso(str);
    rso << *(it->first);
    debug_liv << rso.str();

    if (it->second == true) debug_liv << "  --- true";
    if (it->second == false) debug_liv << "  --- false";

    debug_liv << std::endl;
    it++;
  }
  return 0;
}

int flow_graph::liveness_flow(llvm::Value *op,
                              llvm::ilist_iterator<llvm::Instruction> iita,
                              llvm::BasicBlock *bita) {
  // iita->dump(); //debug
  // op->dump(); //debug

  if (regCover[op] == true)
    return 0;
  else if (iita == (*bita).begin()) {
    // std::string str;
    // llvm::raw_string_ostream rso(str);
    // rso<<*iita;
    // debug_liv << rso.str();
    // debug_liv << std::endl;
    live_tab[iita].live_in[op] = true;  // instr where
    // operand first
    live_tab[iita].live_out[op] = false;  // used is live in
    // but not
    regCover[op] = true;
  } else {
    /*std::string str;
    llvm::raw_string_ostream rso(str);
    rso<<*iita;
    debug_liv << rso.str();
    debug_liv << std::endl;*/
    live_tab[iita].live_in[op] = true;  // instr where
    // operand first
    live_tab[iita].live_out[op] = false;  // used is live in
    // but not
    // live out
    while (iita != (*bita).begin()) {
      // populate the liveness
      // table
      // iita->dump();//debug
      if (iita == op) {
        iita--;
        live_tab[iita].live_in[op] = false;
        live_tab[iita].live_out[op] = true;
        iita = (*bita).begin();  // breaks out
      } else {
        iita--;
        live_tab[iita].live_in[op] = true;
        live_tab[iita].live_out[op] = true;
      }
    }
    regCover[op] = true;
  }

  return 0;
}  // liveness flow

void flow_graph::build_iGraph() {
  std::map<llvm::Value *, live_info>::iterator it = live_tab.begin();
  while (it != live_tab.end()) {
    auto liv_in = (it->second).live_in;
    auto liv_out = (it->second).live_out;
    traverse_list(liv_in);
    traverse_list(liv_out);
    it++;
  }

  // debug printing loop below

  /*std::ofstream debug_intf;
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
t
t
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
                                       else     debug_intf << " O " <<//never
                                                          std::endl ;//called

                             }
                             reg_it_1++;
                    }

              }
              reg_it_0++;
       }

       debug_intf.close();*/
}

void flow_graph::traverse_list(std::map<llvm::Value *, bool> imap) {
  std::map<llvm::Value *, bool>::iterator it_0 = imap.begin();
  while (it_0 != imap.end()) {
    std::map<llvm::Value *, bool>::iterator it_1 = it_0;
    it_1++;
    while (it_1 != imap.end()) {
      auto val_0 = it_0->first;
      auto val_1 = it_1->first;
      intf_table[val_0][val_1] = true;
      it_1++;
    }
    it_0++;
  }
}  // traverse_list

bool flow_graph::select_regs(llvm::Module::iterator fita) {
  select *s_regs = new select(simp_igraph->getStack(), intf_table,
                              simp_igraph->get_reg_map(), fita);

  return s_regs->return_flag();
}

void flow_graph::empty_the_maps() {  // not needed if we make new flowgraph for
                                     // each regalloc iter
  intf_table.empty();
  live_tab.empty();
}

}  // namespace ska

#endif
