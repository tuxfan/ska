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
  
          std::map<llvm::Value *,bool> pot_spill;
                    //stores only potential spills


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


};


select::select(std::stack<spill_info> ss,intf intf_table ){
         

          //pop instructions out of the ss stack 
          //if they are simple nodes, just assign a
          //color and proceed. If they are potential
          //spills, then make a separate list i.e.
          //assign colors to those in the end. If 
          //it turns out that they are colorable later 
          //on, then we are done. Otherwise need to 
          //rewrite program and redo the reg allocation
          while( ss.size() != 0 ){
                   auto ii = ss.pop(); //pop the instruction
                   if(ii.second==false){ //no spill, random color possible
                            color cc = c_map[0];//change to reg type of 
                                                //instruction obtained
                                                //from xml   
                            color min_c = 9999;
                            intf::iterator intf_it = intf_table[ii].begin();
                            while (intf_it != intf_table[ii].end()){
                                      color temp_c = reg_color[intf_it->first] ;
                                      if((temp_c < min_c) && (temp_c >= cc) &&
                                                    (temp_c < cc+reg_map[ii]) ) 
                                                min_c=temp_c; 
                                      intf_it++;
                            }
                            if ( min_c = 9999) c_map[inst_reg[ii]]; 
                            reg_color[ii]=min_c;
                   }else{ //potential spill, color in the end
                          //do nothing here
                          pot_spill.push(ii);
                          color min_c = 9999;
                          intf::iterator intf_it = intf_table[ii].begin();
                          while (intf_it != intf_table[ii].end()){
                                    color temp_c = reg_color[intf_it->first] ;
                                    if((temp_c < min_c) && (temp_c >= cc) &&
                                                  (temp_c < cc+reg_map[ii]) ) 
                                              min_c=temp_c; 
                                    intf_it++;
                          }
                   } 
          }

          while( pot_spill.size() != 0){
                    auto ii = pot_spill.pop();
                    color cc = c_map[0]; //change to reg type of
                                         //instruction obtained 
                                         //from xml
                    color max_c = -1;
                    intf::iterator intf_it = intf_table[ii].begin();
                    while (intf_it != intf_table[ii].end()){
                              color temp_c = reg_color[intf_it->first] ;
                              if((temp_c > max_c) && (temp_c >= cc) &&
                                            (temp_c < cc+reg_map[ii]) ) 
                                        max_c=temp_c;
                              intf_it++;
                    }
                    if (max_c == cc+reg_map[ii]){



                  

                    } //means there is an actual spill
                      //so need to push it to mem after definition
                      //and get load it from mem before use, so that 
                      //there are two tiny live ranges, instead of one large
                      //live range
                              

          }

          doRegAlloc(); //need to do register allocation again
                    

                    
}

}
