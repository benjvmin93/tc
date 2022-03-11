/**
 ** \file ast/pretty-printer.hh
 ** \brief Declaration of ast::PrettyPrinter.
 */

#pragma once

#include <ast/default-visitor.hh>
#include <ast/object-visitor.hh>

namespace ast
{
  /// Visit an Ast and print the content of each node.
  class PrettyPrinter
    : virtual public DefaultConstVisitor
    , virtual public ObjectConstVisitor
  {
  public:
    using super_type = DefaultConstVisitor;
    // Import overloaded virtual functions.
    using super_type::operator();

    /// Build to print on \a ostr.
    PrettyPrinter(std::ostream& ostr);

    /// Visit methods.
    /// \{
    //Var
    void operator()(const SimpleVar& e) override;   //OK
    void operator()(const FieldVar& e) override;    //OK
    void operator()(const SubscriptVar& e) override;//OK
    //Exp
    void operator()(const CastExp& e) override;     //OK

    void operator()(const ArrayExp& e) override;
    void operator()(const AssignExp& e) override;
    void operator()(const CallExp& e) override;
    void operator()(const ForExp& e) override;    //OK
    void operator()(const IfExp& e) override;
    void operator()(const IntExp& e) override;    //OK
    void operator()(const LetExp& e) override;    //OK
    void operator()(const NilExp& e) override;
    void operator()(const ObjectExp& e) override;
    void operator()(const OpExp& e) override;
    void operator()(const RecordExp& e) override;
    void operator()(const SeqExp& e) override;
    void operator()(const StringExp& e) override; //OK
    void operator()(const WhileExp& e) override;  //OK

    // Ty
    void operator()(const ArrayTy& e) override;
    void operator()(const ClassTy& e) override;
    void operator()(const NameTy& e) override;
    void operator()(const RecordTy& e) override;

    //Dec
    void operator()(const FunctionDec& e) override;//OK
    void operator()(const TypeDec& e) override;
    void operator()(const VarDec& e) override;
    // FIXME: Some code was deleted here.
    /// \}

  private:
    // Factor pretty-printing of RecordExp and RecordTy.
    template <typename RecordClass> void print_record(const RecordClass& e);

  protected:
    /// The stream to print on.
    std::ostream& ostr_;
  };

} // namespace ast
