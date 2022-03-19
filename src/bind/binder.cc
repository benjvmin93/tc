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
    // FIXME: Some code was deleted here.
  }

  /*----------------.
  | Symbol tables.  |
  `----------------*/

  void Binder::scope_begin() { scope_.scope_begin(); }

  void Binder::scope_end() { scope_.scope_end(); }

  /*---------.
  | Visits.  |
  `---------*/

  void operator()(ast::FunctionDec& e)
  {
    scope_.put(e.name_get(), &e);
    e.formals_get().accept(*this);
    e.result_get()->accept(*this);
    e.body_get()->accept(*this);
  }
  void operator()(ast::TypeDec& e)
  {
    scope_.put(e.name_get(), &e);
    e.ty_get().accept(*this);
  }
  void operator()(ast::VarDec& e)
  {
    scope_.put(e.name_get(), &e);
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

  void operator()(ast::WhileExp& e)
  {
    scope_begin();
    e.test_get().accept(*this);
    e.body_get().accept(*this);
    scope_end();
  }
  void operator()(ast::ForExp& e)
  {
    scope_begin();
    e.vardec_get().accept(*this);
    e.hi_get().accept(*this);
    e.body_get().accept(*this);
    scope_end();
  }

  /*-------------------.
  | Visiting VarChunk. |
  `-------------------*/

  void Binder::operator()(ast::VarChunk& e) { chunk_visit<ast::VarChunk>(e); }

  /*------------------------.
  | Visiting FunctionChunk. |
  `------------------------*/

  void Binder::operator()(ast::FunctionChunk& e)
  {
    chunk_visit<ast::FunctionChunk>(e);
  }

  /*--------------------.
  | Visiting TypeChunk. |
  `--------------------*/
  void Binder::operator()(ast::TypeChunk& e) { chunk_visit<ast::TypeChunk>(e); }

} // namespace bind
