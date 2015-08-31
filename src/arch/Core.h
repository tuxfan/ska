/*~--------------------------------------------------------------------------~*
 * Copyright (c) 2012 Los Alamos National Security, LLC
 * All rights reserved
 *~--------------------------------------------------------------------------~*/

/*----------------------------------------------------------------------------*
 * Core class.
 *----------------------------------------------------------------------------*/

#ifndef Core_h
#define Core_h

#include <memory>

#include "LU.h"
#include "RegisterSet.h"
#include "../process/MachineState.h"

namespace ska {

/*----------------------------------------------------------------------------*
 * Core class.
 *----------------------------------------------------------------------------*/

class core_t
{
public:

  /*-------------------------------------------------------------------------*
   * Constructor.
   *-------------------------------------------------------------------------*/

  core_t(size_t max_issue, size_t issue_latency)
    : max_issue_(max_issue), issue_latency_(issue_latency),
    in_issue_(false), last_issue_(0), machine_(machine_state_t::instance()) {}

  /*-------------------------------------------------------------------------*
   * Destructor.
   *-------------------------------------------------------------------------*/

  ~core_t() {}

  /*-------------------------------------------------------------------------*
   * Add register sets to the core.
   *-------------------------------------------------------------------------*/

  void add_register_set(register_set_shared_t & rs) {
    register_sets_[rs->type()] = rs;
  } // add_unit

  size_t registers(register_set_t::register_type_t type) {
    return register_sets_[type]->num_registers();
  } // registers

  /*-------------------------------------------------------------------------*
   * Add LUs to the core.
   *-------------------------------------------------------------------------*/

  void add_unit(lu_shared_t & lu) {
    units_.push_back(lu);
  } // add_unit

  /*-------------------------------------------------------------------------*
   * Test to see if the core can except an op.  If yes, the id of the LU
   * that will execute the op is return, -1 otherwise.
   *-------------------------------------------------------------------------*/

  int32_t accept(instruction_t * inst) {

    // If this is true, the core can't issue because of issue latency
    if(issue_latency_ > 1 &&
      in_issue_ &&
      last_issue_ != machine_.current()) {
      return -1;
    } // if

    for(auto unit = units_.begin(); unit != units_.end(); ++unit) {
      if((*unit)->issue(inst)) {
        in_issue_ = true;
        last_issue_ = machine_.current();
        return (*unit)->id();
      } // if
    } // for

    return -1;
  } // accept

  /*-------------------------------------------------------------------------*
   * Advance the core.
   *-------------------------------------------------------------------------*/

  void advance() {
    for(auto unit = units_.begin(); unit != units_.end(); ++unit) {
      (*unit)->advance();
    } // for

    machine_state_t::instance().advance();

    // check to see if we are still within the latency window
    if((machine_.current() - last_issue_) >= issue_latency_) {
      in_issue_ = false;
    } // if
  } // advance

  /*-------------------------------------------------------------------------*
   * Return the maximum issue rate of the core.
   *-------------------------------------------------------------------------*/

  size_t max_issue() const { return max_issue_; }

  /*-------------------------------------------------------------------------*
   * Return the issue latency of the core.
   *-------------------------------------------------------------------------*/

  size_t issue_latency() const { return issue_latency_; }

  /*-------------------------------------------------------------------------*
   * Release the current instruction stream.
   *-------------------------------------------------------------------------*/

  void release(int32_t id) {
    units_[id]->flush();
  } // release

private:

  std::vector<lu_shared_t> units_;
  std::map<register_set_t::register_type_t,
    register_set_shared_t> register_sets_;
  size_t max_issue_;
  size_t issue_latency_;
  bool in_issue_;
  size_t last_issue_;
  machine_state_t & machine_;

}; // class core_t

using core_shared_t = std::shared_ptr<core_t>;

} // namespace ska

#endif // Core_h

/*----------------------------------------------------------------------------*
 * vim: set tabstop=2 shiftwidth=2 expandtab :
 *----------------------------------------------------------------------------*/
