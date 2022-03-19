/**
 ** \file ast/pretty-printer.cc
 ** \brief Implementation of ast::PrettyPrinter.
 */

#include <cstddef>
#include <typeinfo>
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
    /// Output \a e on \a ostr.
    inline std::ostream& operator<<(std::ostream& ostr, const Escapable& e)
    {
      if (escapes_display(ostr)
          // FIXME: Some code was deleted here.
      )
        ostr << "/* escaping */ ";

      return ostr;
    }

    /// \brief Output \a e on \a ostr.
    ///
    /// Used to factor the output of the name declared,
    /// and its possible additional attributes.
    inline std::ostream& operator<<(std::ostream& ostr, const Dec& e)
    {
      ostr << e.name_get();
      if (bindings_display(ostr))
        ostr << " /* " << &e << " */";
      return ostr;
    }
  } // namespace

  PrettyPrinter::PrettyPrinter(std::ostream& ostr)
    : ostr_(ostr)
  {}

  void PrettyPrinter::operator()(const SimpleVar& e)
  {
    ostr_ << e.name_get();
    if (bindings_display(ostr_))
      ostr_ << " /* " << e.def_get() << " */";
  }

  void PrettyPrinter::operator()(const FieldVar& e)
  {
    ostr_ << e.var_get() << "." << e.name_get(); /* not sure */
  }

  void PrettyPrinter::operator()(const Field& e)
  {
    ostr_ << e.name_get() << " : " << e.type_name_get();
  }

  void PrettyPrinter::operator()(const FieldInit& e)
  {
    ostr_ << e.name_get() << " = " << e.init_get();
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
    ostr_ << "\"" << e.string_get() << "\"";
  }

  void PrettyPrinter::operator()(const ForExp& e)
  {
    ostr_ << "for ";
    if (bindings_display(ostr_))
      ostr_ << "/* " << &e << " */";
    ostr_ << e.vardec_get().name_get();
    if (bindings_display(ostr_))
      ostr_ << " /* " << &e.vardec_get() << " */";
    ostr_ << " := " << *(e.vardec_get().init_get()) << " to " << e.hi_get()
          << " do" << misc::incendl;
    ostr_ << e.body_get() << misc::decindent;
  }

  void PrettyPrinter::operator()(const LetExp& e)
  {
    ostr_ << "let" << misc::incendl;
    bool first = false;
    for (auto& x : e.chunklist_get().chunks_get())
      {
        if (!first)
          {
            ostr_ << *x;
            first = true;
          }
        else
          ostr_ << misc::iendl << *x;
      }
    ostr_ << misc::decendl << "in" << misc::incendl << e.exp_get()
          << misc::decendl;
    ostr_ << "end";
  }

  void PrettyPrinter::operator()(const WhileExp& e)
  {
    ostr_ << "while";
    if (bindings_display(ostr_))
      ostr_ << " /* " << &e << " */";
    ostr_ << " (" << e.test_get() << ')' << misc::iendl << "do"
          << misc::incendl;
    ostr_ << e.body_get() << misc::decindent;
  }

  void PrettyPrinter::operator()(const FunctionDec& e)
  {
    if (e.body_get() != nullptr)
      ostr_ << "function ";
    else
      ostr_ << "primitive ";
    ostr_ << e.name_get();
    if (bindings_display(ostr_))
      ostr_ << " /* " << &e << " */";
    ostr_ << '(';
    bool first = false;
    for (auto& x : e.formals_get())
      if (!first)
        {
          ostr_ << x->name_get();
          if (bindings_display(ostr_))
            ostr_ << " /* " << x << " */";
          ostr_ << " : " << x->type_name_get()->name_get();
          if (bindings_display(ostr_))
            ostr_ << " /* " << x->type_name_get() << " */";
          first = true;
        }
      else
        {
          ostr_ << ", " << x->name_get();
          if (bindings_display(ostr_))
            ostr_ << " /* " << x << " */";
          ostr_ << " : " << x->type_name_get()->name_get();
          if (bindings_display(ostr_))
            ostr_ << " /* " << x->type_name_get() << " */";
        }
    ostr_ << ")";
    if (e.result_get() != nullptr)
      {
        ostr_ << " : " << e.result_get()->name_get();
        if (bindings_display(ostr_))
          ostr_ << " /* " << e.result_get() << " */";
      }
    if (e.body_get() != nullptr)
      {
        ostr_ << " =" << misc::incendl << *(e.body_get()) << misc::decendl;
      }
    else
      ostr_ << misc::iendl;
  }

  void PrettyPrinter::operator()(const ArrayExp& e)
  {
    ostr_ << e.type_name_get() << '[' << e.size_get() << ']';
    ostr_ << " of " << e.init_get();
  }

  void PrettyPrinter::operator()(const IfExp& e)
  {
    ostr_ << "if " << e.get_test() << misc::incendl;

    if (&(e.get_elseclause()) != nullptr)
      {
        ostr_ << "then " << e.get_thenclause() << misc::iendl;
        ostr_ << "else ";
        ostr_ << e.get_elseclause() << misc::decindent;
      }
    else
      {
        ostr_ << "then " << e.get_thenclause() << misc::decindent;
      }
  }

  void PrettyPrinter::operator()(const VarDec& e)
  {
    ostr_ << "var " << e.name_get();
    if (bindings_display(ostr_))
      ostr_ << " /* " << &e << " */";
    ostr_ << " ";
    if (e.type_name_get() != nullptr)
      {
        ostr_ << ": " << *(e.type_name_get()) << ' ';
      }
    ostr_ << ":= " << *(e.init_get());
  }

  void PrettyPrinter::operator()(const TypeDec& e)
  {
    ostr_ << "type " << e.name_get();
    if (bindings_display(ostr_))
      ostr_ << " /* " << &e << " */";
    ostr_ << " = " << e.ty_get() << misc::iendl;
  }

  void PrettyPrinter::operator()(const ArrayTy& e)
  {
    ostr_ << "array of " << e.base_type_get();
  }

  void PrettyPrinter::operator()(const ClassTy& e)
  {
    ostr_ << "class";
    if (&(e.super_get()) != nullptr)
      ostr_ << " extends " << e.super_get().name_get() << misc::iendl;
    else
      ostr_ << " extends Object" << misc::iendl;
    ostr_ << "{" << misc::incendl;
    for (auto& ch : e.chunks_get())
      {
        ostr_ << *ch << misc::iendl;
      }

    ostr_ << misc::decendl << "}";
  }

  void PrettyPrinter::operator()(const NameTy& e) { ostr_ << e.name_get(); }

  void PrettyPrinter::operator()(const RecordTy& e)
  {
    ostr_ << "{ ";
    bool first = false;
    for (auto& ch : e.field_get())
      {
        if (!first)
          {
            ostr_ << ch->name_get() << " : " << ch->type_name_get();
            if (bindings_display(ostr_))
              ostr_ << " /* " << &ch->type_name_get() << " */";
            first = true;
          }
        else
          ostr_ << ", " << ch->name_get() << " : " << ch->type_name_get();
      }
    ostr_ << " }";
  }

  void PrettyPrinter::operator()(const AssignExp& e)
  {
    ostr_ << e.var_get();
    if (bindings_display(ostr_))
      ostr_ << " /* " << &e.var_get() << " */";
    ostr_ << " := " << e.exp_get();
  }

  void PrettyPrinter::operator()(const CallExp& e)
  {
    ostr_ << e.name_get();
    if (bindings_display(ostr_))
      ostr_ << " /* " << &e << " */";
    ostr_ << "(";
    if (e.args_get().size() >= 1)
      {
        ostr_ << *(e.args_get()[0]);
        if (bindings_display(ostr_))
          ostr_ << " /* " << e.args_get()[0] << " */";
      }
    for (size_t args = 1; args < e.args_get().size(); args++)
      {
        ostr_ << ", " << *(e.args_get()[args]);
        if (bindings_display(ostr_))
          ostr_ << " /* " << e.args_get()[args] << " */";
      }
    ostr_ << ")";
  }

  void PrettyPrinter::operator()(const ObjectExp& e)
  {
    ostr_ << "new " << e.type_name_get();
  }

  void PrettyPrinter::operator()(const OpExp& e)
  {
    ostr_ << "(" << e.left_get() << " " << str(e.oper_get()) << " "
          << e.right_get() << ")";
  }

  void PrettyPrinter::operator()(const RecordExp& e)
  {
    ostr_ << e.get_type_name() << " { ";
    bool first = false;
    for (auto& fi : e.get_fields())
      {
        if (!first)
          {
            first = true;
            ostr_ << fi->name_get() << " = " << fi->init_get();
          }
        else
          ostr_ << ", " << fi->name_get() << " = " << fi->init_get();
      }
    ostr_ << " }";
  }

  void PrettyPrinter::operator()(const SeqExp& e)
  {
    //static int main;
    //main++;
    if (e.exps_get().size() == 0)
      ostr_ << "()";
    else if (e.exps_get().size() == 1)
      {
        ostr_ << misc::separate(e.exps_get(), std::make_pair(";", misc::iendl));
      }
    else
      {
        ostr_ << "(" << misc::incendl;
        ostr_ << misc::separate(e.exps_get(), std::make_pair(";", misc::iendl));
        ostr_ << misc::decendl << ")";
      }
    //ostr_ << misc::iendl << "Sequence " << main << " ended." << misc::iendl;
    // main--;
  }

  void PrettyPrinter::operator()(const NilExp& e) { ostr_ << "nil"; }

  void PrettyPrinter::operator()(const MethodDec& e)
  {
    ostr_ << "method " << e.name_get() << "(";
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
      ostr_ << ": " << *(e.result_get());
    ostr_ << " = " << *(e.body_get());
  }

  void PrettyPrinter::operator()(const BreakExp& e)
  {
    ostr_ << "break";
    if (bindings_display(ostr_))
      ostr_ << " /* "; //<< e.def_get() << " */"; // FIXME
  }
} // namespace ast
