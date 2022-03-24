/**
 ** \file type/nil.cc
 ** \brief Implementation for type/nil.hh.
 */

#include <ostream>

#include <type/class.hh>
#include <type/nil.hh>
#include <type/record.hh>
#include <type/visitor.hh>

namespace type
{
  /*------.
  | Nil.  |
  `------*/

  void Nil::accept(ConstVisitor& v) const { v(*this); }

  void Nil::accept(Visitor& v) { v(*this); }

  const Type& Nil::actual() const 
  {
    return *this->record_type_get();
  }

  bool Nil::compatible_with(const Type& other) const
  {
    if (other == this->actual())
      return false;
    return true;
  }

  const Type* Nil::record_type_get() const { return record_type_; }

  void Nil::record_type_set(const Type& type) const { record_type_ = &type; }

} // namespace type
