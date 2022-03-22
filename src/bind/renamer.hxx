/**
 ** \file bind/renamer.hxx
 ** \brief Template methods of bind::Renamer.
 */

#pragma once

#include <bind/renamer.hh>

namespace bind
{
  // FIXME: Some code was deleted here.

  template <class E, class Def> void Renamer::visit(E& e, const Def* def)
  {
    auto name = new_name(*e);
    //visit(*def, &def);
    super_type::operator()(e);
  }

  misc::symbol Renamer::operator()(const ast::NameTy& ty)
  {
    visit(ty, ty.def_get());
  }

  misc::symbol Renamer::operator()(const ast::CallExp& ty)
  {
    visit(ty, ty.def_get());
  }

  misc::symbol Renamer::operator()(const ast::SimpleVar& ty)
  {
    visit(ty, ty.def_get());
  }

  misc::symbol Renamer::operator()(const ast::FunctionDec& ty)
  {
    visit(ty, &ty);
  }

  misc::symbol Renamer::operator()(const ast::VarDec& ty)
  {
    visit(ty, &ty);
  }

  misc::symbol Renamer::operator()(const ast::TypeDec& ty)
  {
    visit(ty, &ty);
  }

} // namespace bind
