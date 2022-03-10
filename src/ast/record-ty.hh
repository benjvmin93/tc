/**
 ** \file ast/record-ty.hh
 ** \brief Declaration of ast::RecordTy.
 */

#pragma once

#include <ast/field.hh>
#include <ast/ty.hh>

namespace ast
{
  /// RecordTy.
  class RecordTy : public Ty
  {
  public:
    RecordTy(const Location& location, Field::Field& field, Ty& ty);
    RecordTy(const RecordTy&) = delete;
    RecordTy& operator=(const RecordTy&) = delete;

    ~RecordTy() override;

    void accept(ConstVisitor& v) const override;
    void accept(Visitor& v) override;

    const Field::Field& field_get() const;
    Field::Field& field_get();

    const Ty& ty_get() const;
    Ty& ty_get();

  protected:
    Field::Field* field_;
    Ty* ty_;
  };
} // namespace ast
#include <ast/record-ty.hxx>
