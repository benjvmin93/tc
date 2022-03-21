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
    auto name = new_name(*def);
    
  }

} // namespace bind
