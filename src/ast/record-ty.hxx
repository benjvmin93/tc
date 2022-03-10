/**
 ** \file ast/record-ty.hxx
 ** \brief Inline methods of ast::RecordTy.
 */

#pragma once

#include <ast/record-ty.hh>

namespace ast
{
  inline const Field::Field& RecordTy::field_get() const { return *field_; }
  inline Field::Field& RecordTy::field_get() { return *field_; }

  inline const Ty& RecordTy::ty_get() const { return *ty_; }
  inline Ty& RecordTy::ty_get() { return *ty_; }
} // namespace ast
