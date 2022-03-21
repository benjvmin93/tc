/**
 ** \file bind/renamer.cc
 ** \brief Implementation of bind::Renamer.
 */

#include <bind/renamer.hh>

namespace bind
{
  using namespace ast;

  Renamer::Renamer()
    : super_type()
    , new_names_()
  {}

  template <typename Def>
  misc::symbol Renamer::new_name_compute(const Def& e)
  {
    // FIXME PRIMITIVE TYPES MUST NOT BE RENAMED.
    auto name = e.name_get();
    if (name != "_main")
      return misc::symbol::fresh(name);
  }

  template <typename Def>
  misc::symbol Renamer::new_name(const Def& e)
  {
    auto it = new_names_.find(e);
    if (it == new_names_.end())
      return new_name_compute(e);
    return new_names_[*it];
  }

} // namespace bind
