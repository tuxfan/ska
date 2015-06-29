namespace ska{

class select{

public :

          typedef int color; //color
          typedef int reg_type; //register type

          typedef std::pair<llvm::Value *,bool> spill_info;
          select(); //select constructor
                    //selects colors for each instruction
                    //inserts instructions if there are
                    //actual spills and iterates regAlloc
                    //in the hopes of getting a legal
                    //allocation. If no actual spills,
                    //then we are done


private :

          std::map<llvm::Value *, intf> new_intf_table;
          std::map<llvm::Value *, color> reg_color;
                                    //final register colors
          std::map<reg_type, std::map<color,bool>> phys_reg_list;
                                    //says which colors correspond
                                    //to a particular register type
                                    //in particular, 


};


select::select(std::stack<spill_info> ss){
         

          //pop instructions out of the ss stack 
          //if they are simple nodes, just assign a
          //color and proceed. If they are potential
          //spills, then make a separate list i.e.
          //assign colors to those in the end. If 
          //it turns out that they are colorable later 
          //on, then we are done. Otherwise need to 
          //rewrite program and redo the reg allocation
          while(  )

}

}
