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
    if (scope_fun_.nb_scope != 1 || e.name_get() != misc::symbol("_main"))
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
    scope_fun_.put(e.name_get(), &e);
    e.formals_get().accept(*this);
    e.result_get()->accept(*this);
    e.body_get()->accept(*this);
  }
  void Binder::operator()(ast::TypeDec& e)
  {
    scope_type_.put(e.name_get(), &e);
    e.ty_get().accept(*this);
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
    e.test_get().accept(*this);
    e.body_get().accept(*this);
    scope_end();
  }
  void Binder::operator()(ast::ForExp& e)
  {
    scope_begin();
    e.vardec_get().accept(*this);
    e.hi_get().accept(*this);
    e.body_get().accept(*this);
    scope_end();
  }

  void Binder::operator()(ast::MethodDec& e)
  {
    scope_begin();
    e.formals_get().accept(*this);
    e.result_get()->accept(*this);
    e.body_get()->accept(*this);
    scope_end();
  }

  void Binder::operator()(ast::MethodChunk& e)
  {
    scope_begin();
    for (auto it = e.begin(); it != e.end(); ++it)
      (*it)->accept(*this);
    scope_end();
  }

  /*-------------------.
  | Visiting VarChunk. |
  `-------------------*/

  void Binder::operator()(ast::VarChunk& e) { chunk_visit(e); }

  /*------------------------.
  | Visiting FunctionChunk. |
  `------------------------*/

  void Binder::operator()(ast::FunctionChunk& e)
  {
    chunk_visit(e);
  }

  /*--------------------.
  | Visiting TypeChunk. |
  `--------------------*/
  void Binder::operator()(ast::TypeChunk& e) { chunk_visit(e); }

} // namespace bind
