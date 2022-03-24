/**
 ** \file type/function.cc
 ** \brief Implementation for type/function.hh.
 */

#include <ostream>

#include <range/v3/view/iota.hpp>
#include <type/function.hh>
#include <type/visitor.hh>

namespace type
{
  Function::Function(const Record* formals, const Type& result)
    : result_(result)
  {
    precondition(formals);

    formals_ = formals;
  }

  Function::~Function() { delete formals_; }

  void Function::accept(ConstVisitor& v) const { v(*this); }

  void Function::accept(Visitor& v) { v(*this); }

  bool Function::compatible_with(const Type& other) const
  {
    if (this.result != other.result_get())
      return false;
    if (this.formals_.fields_get().size()
        != other.formals_get().fields_get().size())
      return false;
    auto& first = this.formals_.fields_get();
    auto& second = other.formals_get().fields_get();
    for (int x = 0; x < first.size(); x++)
      {
        if (first[x].type_get() != second[x].type_get())
          return false;
      }
    return true;
  }

} // namespace type
