/**
 ** \file bind/tasks.hh
 ** \brief Bind module related tasks.
 */

#pragma once

#include <task/libtask.hh>

namespace bind::tasks
{
  TASK_GROUP("3. Bind");

  TASK_DECLARE("b|bindings-compute",
               "bind the identifiers ",
               bind_compute,
               "parse");

  TASK_DECLARE("B|binding-display",
               "enable bindings display in the AST",
               bind_display,
               "parse");
} // namespace bind::tasks
