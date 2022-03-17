/**
 ** \file ast/break-exp.hxx
 ** \brief Inline methods of ast::BreakExp.
 */

#pragma once

#include <ast/break-exp.hh>

// Hint: this needs to be done at TC-3.

namespace ast
{
  inline Exp* BreakExp::def_get() { return def_; }
  inline void BreakExp::def_set(Exp* def) { def_ = def; }
} // namespace ast
