/**
 ** \file bind/libbind.cc
 ** \brief Define exported bind functions.
 */

#include <bind/binder.hh>

namespace bind
{
  misc::error bind_compute(ast::Ast& tree)
  {
    Binder bind_compute;
    bind_compute(tree);
    return bind_compute.error_get();
  }
} // namespace bind