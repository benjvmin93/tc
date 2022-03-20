/**
 ** \file bind/binder.cc
 ** \brief Implementation for bind/binder.hh.
 */

#include <ast/all.hh>
#include <bind/binder.hh>

#include <misc/contract.hh>

namespace bind
{
  /*-----------------.
  | Error handling.  |
  `-----------------*/

  /// The error handler.
  const misc::error& Binder::error_get() const { return error_; }

  // FIXME: Some code was deleted here (Error reporting).

  void Binder::check_main(const ast::FunctionDec& e)
  {
    if (e.name_get() == "_main" && scope_fun_.get("_main") != nullptr)
      error(e, "Main error");
  }

  /*----------------.
  | Symbol tables.  |
  `----------------*/

  void Binder::scope_begin()
  {
    scope_fun_.scope_begin();
    scope_type_.scope_begin();
    scope_var_.scope_begin();
  }

  void Binder::scope_end()
  {
    scope_fun_.scope_end();
    scope_type_.scope_end();
    scope_var_.scope_end();
  }

  /*---------.
  | Visits.  |
  `---------*/

  void Binder::operator()(ast::FunctionDec& e)
  {
    check_main(e);
    auto name = scope_fun_.get(e.name_get());
    if (name != nullptr)
      Binder::redefinition(name, &e);
    else
      {
        scope_fun_.put(e.name_get(), &e);
        scope_begin();
        e.formals_get().accept(*this);
        if (e.result_get() && e.result_get()->def_get())
          e.result_get()->def_get()->accept(*this);
        if (e.body_get())
          e.body_get()->accept(*this);
        scope_end();
      }
  }
  void Binder::operator()(ast::TypeDec& e)
  {
    auto name = scope_type_.get(e.name_get());
    if (name != nullptr)
      Binder::redefinition(name, &e);
    else
      {
        scope_type_.put(e.name_get(), &e);
        e.ty_get().accept(*this);
      }
  }
  void Binder::operator()(ast::VarDec& e)
  {
    scope_var_.put(e.name_get(), &e);
    this->accept(e.type_name_get());
    this->accept(e.init_get());
  }

  void Binder::operator()(ast::LetExp& e)
  {
    scope_begin();
    e.chunklist_get().accept(*this);
    e.exp_get().accept(*this);
    scope_end();
  }

  void Binder::operator()(ast::WhileExp& e)
  {
    scope_begin();
    scope_loop_.push_back(&e);
    e.test_get().accept(*this);
    e.body_get().accept(*this);
    scope_end();
    scope_loop_.pop_back();
  }
  void Binder::operator()(ast::ForExp& e)
  {
    scope_begin();
    scope_loop_.push_back(&e);
    e.vardec_get().accept(*this);
    e.hi_get().accept(*this);
    e.body_get().accept(*this);
    scope_end();
    scope_loop_.pop_back();
  }

  void Binder::operator()(ast::MethodDec& e)
  {
    e.formals_get().accept(*this);
    e.result_get()->accept(*this);
    e.body_get()->accept(*this);
  }

  void Binder::operator()(ast::MethodChunk& e)
  {
    for (auto it = e.begin(); it != e.end(); ++it)
      (*it)->accept(*this);
  }

  void Binder::operator()(ast::SimpleVar& e)
  {
    auto name = scope_var_.get(e.name_get());
    if (name != nullptr)
      e.def_set(name);
    else
      Binder::undeclared("undeclared variable: " + e.name_get().get(), e);
  }

  void Binder::operator()(ast::BreakExp& e)
  {
    if (scope_loop_.size() == 0)
      Binder::loop(e);
    else
      {
        auto name = scope_loop_.back();
        e.def_set(name);
      }
  }

  void Binder::operator()(ast::RecordExp& e)
  {
    auto name = scope_type_.get(e.get_type_name().name_get());
    if (name != nullptr)
      e.def_set(name);
    else
      Binder::undeclared(
        "undeclared type: " + e.get_type_name().name_get().get(), e);
    e.get_type_name().accept(*this);
    auto fieldInit = e.get_fields();
    for (auto elt : fieldInit)
      elt->accept(*this);
  }

  /*
  void Binder::operator()(ast::FieldVar& e)
  {
    scope_begin();
    e.var_get().accept(*this);
    scope_end();
  }

  void Binder::operator()(ast::SubscriptVar& e)
  {
    scope_begin();
    e.var_get().accept(*this);
    e.index_get().accept(*this);
    scope_end();
  } */

  /*-------------------.
  | Visiting VarChunk. |
  `-------------------*/

  void Binder::operator()(ast::VarChunk& e) { chunk_visit(e); }

  /*------------------------.
  | Visiting FunctionChunk. |
  `------------------------*/

  void Binder::operator()(ast::FunctionChunk& e) { chunk_visit(e); }

  /*--------------------.
  | Visiting TypeChunk. |
  `--------------------*/
  void Binder::operator()(ast::TypeChunk& e) { chunk_visit(e); }

} // namespace bind
