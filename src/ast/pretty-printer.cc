/**
 ** \file ast/pretty-printer.cc
 ** \brief Implementation of ast::PrettyPrinter.
 */

#include <cstddef>
#include <ast/all.hh>
#include <ast/libast.hh>
#include <ast/pretty-printer.hh>
#include <misc/escape.hh>
#include <misc/indent.hh>
#include <misc/separator.hh>

namespace ast
{
  // Anonymous namespace: these functions are private to this file.
  namespace
  {

    /// \brief Output \a e on \a ostr.
    ///
    /// Used to factor the output of the name declared,
    /// and its possible additional attributes.
    inline std::ostream& operator<<(std::ostream& ostr, const Dec& e)
    {
      ostr << e.name_get();
      return ostr;
    }
  } // namespace

  PrettyPrinter::PrettyPrinter(std::ostream& ostr)
    : ostr_(ostr)
  {}

  void PrettyPrinter::operator()(const SimpleVar& e) { ostr_ << e.name_get(); }

  void PrettyPrinter::operator()(const FieldVar& e)
  {
    ostr_ << e.name_get(); /* not sure */
  }

  /* Foo[10]. */
  void PrettyPrinter::operator()(const SubscriptVar& e)
  {
    ostr_ << e.var_get() << '[' << misc::incindent << e.index_get()
          << misc::decindent << ']';
  }

  void PrettyPrinter::operator()(const CastExp& e)
  {
    ostr_ << "_cast(" << e.exp_get() << ", " << e.ty_get() << ')';
  }

  void PrettyPrinter::operator()(const IntExp& e) { ostr_ << e.value_get(); }

  void PrettyPrinter::operator()(const StringExp& e)
  {
    ostr_ << e.string_get();
  }

  void PrettyPrinter::operator()(const ForExp& e)
  {
    ostr_ << "(for " << e.vardec_get() << ":= " << *(e.vardec_get().init_get())
          << " to " << e.hi_get() << " do" << misc::incendl;
    ostr_ << e.body_get() << ");" << misc::decendl;
  }

  void PrettyPrinter::operator()(const LetExp& e)
  {
    ostr_ << "let" << misc::incendl << e.chunklist_get() << misc::decendl;
    ostr_ << "in" << misc::incendl << e.exp_get();
    ostr_ << "end;" << misc::iendl;
  }

  void PrettyPrinter::operator()(const WhileExp& e)
  {
    ostr_ << "while (" << e.test_get() << ')' << misc::incendl;
    ostr_ << e.body_get() << misc::decendl;
  }

  void PrettyPrinter::operator()(const FunctionDec& e)
  {
    ostr_ << "function " << e.name_get() << '(';
    bool first = false;
    for (auto& x : e.formals_get())
      if (!first)
        {
          ostr_ << x->name_get() << " : " << x->type_name_get()->name_get();
          first = true;
        }
      else
        ostr_ << ", " << x->name_get() << " : "
              << x->type_name_get()->name_get();
    ostr_ << ")";
    if (e.result_get() != nullptr)
      ostr_ << " : " << e.result_get()->name_get();
    if (e.body_get() != nullptr)
      {
        ostr_ << " =" << misc::incendl;
        ostr_ << "(" << misc::incendl;
        ostr_ << *(e.body_get());
        ostr_ << "()" << misc::decendl;
        ostr_ << ")";
      }
    ostr_ << misc::iendl;
  }

  void PrettyPrinter::operator()(const ArrayExp& e)
  {
    ostr_ << e.type_name_get() << '[' << e.size_get() << ']';
    ostr_ << " = " << e.init_get();
  }

  void PrettyPrinter::operator()(const IfExp& e)
  {
    ostr_ << "(if " << e.get_test() << misc::incendl;

    if (&(e.get_elseclause()) != nullptr)
      {
        ostr_ << "then " << e.get_thenclause() << misc::iendl;
        ostr_ << "else ";
        ostr_ << e.get_elseclause() << ");" << misc::decendl;
      }
    else
      {
        ostr_ << "then " << e.get_thenclause() << ");" << misc::decendl;
      }
  }

  void PrettyPrinter::operator()(const VarDec& e)
  {
    if (e.type_name_get() != nullptr)
      {
        ostr_ << e.type_name_get() << ' ';
      }

    ostr_ << e.name_get() << " = " << e.init_get();
  }

  void PrettyPrinter::operator()(const TypeDec& e)
  {
    ostr_ << e.ty_get() << ' ' << e.name_get();
  }

  void PrettyPrinter::operator()(const ArrayTy& e)
  {
    ostr_ << e.base_type_get();
  }

  void PrettyPrinter::operator()(const ClassTy& e)
  {
    ostr_ << e.super_get() << "\n{" << misc::incendl;

    for (auto& ch : e.chunks_get())
      ostr_ << *ch << misc::iendl;

    ostr_ << misc::decindent << "};" << misc::iendl;
  }

  void PrettyPrinter::operator()(const NameTy& e)
  {
    ostr_ << "typename " << e.name_get() << '<' << e.def_get() << '>';
  }

  void PrettyPrinter::operator()(const RecordTy& e)
  {
    ostr_ << "{ ";
    for (auto& ch : e.field_get())
      ostr_ << *(ch) << ',';
    ostr_ << " }";
  }

  void PrettyPrinter::operator()(const AssignExp& e)
  {
    ostr_ << e.var_get() << " := " << e.exp_get();
  }

  void PrettyPrinter::operator()(const CallExp& e)
  {
    ostr_ << e.name_get() << "(";
    if (e.args_get().size() >= 1)
      ostr_ << *(e.args_get()[0]);
    for (size_t args = 1; args < e.args_get().size(); args++)
      ostr_ << ", " << *(e.args_get()[args]);
    ostr_ << ");" << misc::iendl;
  }

  void PrettyPrinter::operator()(const ObjectExp& e)
  {
    ostr_ << e.type_name_get();
  }

  void PrettyPrinter::operator()(const OpExp& e)
  {
    ostr_ << '(' << e.left_get() << ' ' << e.right_get() << ')';
  }

  void PrettyPrinter::operator()(const RecordExp& e)
  {
    ostr_ << "type " << e.get_type_name() << " = { ";

    for (auto fi : e.get_fields())
      ostr_ << fi->name_get() << " : " << fi->init_get() << ',';

    ostr_ << " }";
  }

} // namespace ast
