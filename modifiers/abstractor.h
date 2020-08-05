/*********************                                                  */
/*! \file abstractor.h
** \verbatim
** Top contributors (to current version):
**   Makai Mann
** This file is part of the pono project.
** Copyright (c) 2019 by the authors listed in the file AUTHORS
** in the top-level source directory) and their institutional affiliations.
** All rights reserved.  See the file LICENSE in the top-level source
** directory for licensing information.\endverbatim
**
** \brief Generic base class for abstractors -- classes that can
**        create an abstract version of a transition system
**
**/
#pragma once

#include "core/fts.h"
#include "core/rts.h"

namespace pono {

class Abstractor
{
 public:
  Abstractor(const TransitionSystem & ts) : orig_ts_(ts),
  {
    if (orig_ts_.is_functional()) {
      abs_ts_ = FunctionalTransitionSystem(orig_ts_.solver());
    } else {
      abs_ts_ = RelationalTransitionSystem(orig_ts_.solver());
    }
  }

  virtual ~Abstractor() {}

  /** Returns the abstraction of a concrete term
   *  @param the concrete term to abstract
   *  @return the abstracted term
   *  This is a NOP implementation. Derived classes will implement this.
   */
  smt::Term abstract(const smt::Term & t) const
  {
    Term res;
    if (abstraction_cache_.find(t) != abstraction_cache_.end()) {
      res = abstraction_cache_.at(t);
    } else {
      res = t;
    }
    return res;
  }

  /** Returns the concretization of an abstract term
   *  @param the abstract term to concretize
   *  @return the concrete version of the term
   *  This is a NOP implementation. Derived classes will implement this.
   */
  smt::Term concrete(const smt::Term & t) const
  {
    Term res;
    if (concretization_cache_.find(t) != concretization_cache_.end()) {
      res = concretization_cache_.at(t);
    } else {
      res = t;
    }
    return res;
  }

  /** Getter for the abstracted transition system
   *  This is intentionally a non-const reference because often
   *  the abstraction will be refined. This way, it does not
   *  need to be copied before being refined.
   *  @return a reference to the abstracted system
   */
  TransitionSystem & abs_ts() { return abs_ts_; }

 protected:
  /** Perform the abstraction
   *  This should populate the abstraction and concretization caches
   *  This is a NOP implementation. Derived classes will implement this.
   *  Should be called in derived class constructor.
   */
  void do_abstraction(){};

  const TransitionSystem & orig_ts_;
  TransitionSystem abs_ts_;

  smt::UnorderedTermMap abstraction_cache_;
  smt::UnorderedTermMap concretization_cache_;
};

}  // namespace pono