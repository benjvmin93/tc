/**
 ** \file type/record.cc
 ** \brief Implementation for type/record.hh.
 */

#include <ostream>

#include <type/builtin-types.hh>
#include <type/nil.hh>
#include <type/record.hh>
#include <type/visitor.hh>

namespace type
{
  void Record::accept(ConstVisitor& v) const { v(*this); }

  void Record::accept(Visitor& v) { v(*this); }

  // FIXME: Some code was deleted here (Field manipulators).
  const Type* Record::field_type(misc::symbol key) const
  {
    for (auto& v : fields_)
    {
      if (v.name_get() == key)
        return &v.type_get();
    }
    return nullptr;
  }

  int Record::field_index(misc::symbol key) const
  {
    int count = 0;
    for (auto v : fields_)
    {
      if (v.name_get() == key)
        return count; 
      count++;
    }
  }

  // FIXME: Some code was deleted here (Special implementation of "compatible_with" for Record).
  bool Record::compatible_with(const Type& other) const
  {
    return true; //TODO 
  }

} // namespace type
