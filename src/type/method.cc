/**
 ** \file type/method.cc
 ** \brief Implementation for type/method.hh.
 */

#include <iostream>

#include <type/method.hh>
#include <type/visitor.hh>

namespace type
{
  Method::Method(misc::symbol name,
                 const Class* owner,
                 const Record* formals,
                 const Type& result,
                 ast::MethodDec* def)
    : Function(formals, result)
    , name_(name)
    , owner_(owner)
    , def_(def)
  {}

  void Method::accept(ConstVisitor& v) const { v(*this); }

  void Method::accept(Visitor& v) { v(*this); }

  bool Method::compatible_with(const Type& other) const
  {
    if (this->type_get() == other.actual())
      return true;
    return false;
  }
} // namespace type
