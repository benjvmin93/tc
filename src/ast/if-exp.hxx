/**
 ** \file ast/if-exp.hxx
 ** \brief Inline methods of ast::IfExp.
 */

#pragma once

#include <ast/if-exp.hh>

namespace ast
{
  const Exp& IfExp::get_test() const
  {
    return *test_;
  }
  Exp& IfExp::get_test()
  {
    return *test_;
  }
  const Exp& IfExp::get_thenclause() const
  {
    return *thenclause_;
  }
  Exp& IfExp::get_thenclause()
  {
    return *thenclause_;
  }
  const Exp& IfExp::get_elseclause() const
  {
    return *elseclause_;
  }
  Exp& IfExp::get_elseclause()
  {
    return *elseclause_;
  }
} // namespace ast
