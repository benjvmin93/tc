/**
 ** \file ast/record-ty.cc
 ** \brief Implementation of ast::RecordTy.
 */

#include <ast/record-ty.hh>
#include <ast/visitor.hh>
#include <misc/algorithm.hh>

namespace ast
{
  RecordTy::RecordTy(const Location& location, Field::Field& field, Ty& ty)
    : field_(field)
    , ty_(ty)
  {}

  RecordTy::~RecordTy()
  {
    delete field_;
    delete ty_;
  }

  void RecordTy::accept(ConstVisitor& v) const { v(*this); }

  void RecordTy::accept(Visitor& v) { v(*this); }
} // namespace ast
