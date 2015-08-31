/*~--------------------------------------------------------------------------~*
 * Copyright (c) 2012 Los Alamos National Security, LLC
 * All rights reserved
 *~--------------------------------------------------------------------------~*/

#ifndef RegisterSet_h
#define RegisterSet_h

namespace ska {

/*----------------------------------------------------------------------------*
 * Register Set class.
 * All rights reserved
 *----------------------------------------------------------------------------*/

class register_set_t
{
public:

  enum class register_type_t {
    Integer, //0
    Float,   //1
    Vector,  //2
    Default
  }; // enum class type

  register_set_t(register_type_t type, size_t num_registers)
    : type_(type), num_registers_(num_registers) {}

  ~register_set_t() {}

  register_type_t type() const { return type_; }
  size_t num_registers() const { return num_registers_; }

private:

  register_type_t type_;
  size_t num_registers_;

}; // class register_set_t

using register_set_shared_t = std::shared_ptr<register_set_t>;

} // namespace ska

#endif // RegisterSet_h

/*~-------------------------------------------------------------------------~-*
 * vim: set tabstop=2 shiftwidth=2 expandtab :
 *~-------------------------------------------------------------------------~-*/
