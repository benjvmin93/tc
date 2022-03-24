/**
 ** \file ast/type-constructor.hh
 ** \brief Declaration of ast::TypeConstructor.
 */

#pragma once

#include <ast/fwd.hh>
#include <type/fwd.hh>

namespace ast
{
  /** \class ast::TypeConstructor
   ** \brief Create a new type.
   */

  class TypeConstructor
  {
    void create_type_set(const type::Type*);
    const type::Type* created_type_get() const;

    virtual void accept(ConstVisitor& v) const;
    virtual void accept(Visitor& v);

    type::Type* type_;
  };
} // namespace ast
#include <ast/type-constructor.hxx>
