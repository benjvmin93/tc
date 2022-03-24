/**
 ** \file type/array.hh
 ** \brief The class Array.
 */
#pragma once

#include <type/fwd.hh>
#include <type/type.hh>

namespace type
{
  /// Array types.
  class Array : public Type
  {
    Array(const Type& type);

    const Type& type_get() const;

    const Type& type_;
  };

} // namespace type

#include <type/array.hxx>
