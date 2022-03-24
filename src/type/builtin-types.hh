/**
 ** \file type/builtin-types.hh
 ** \brief The classes Int, String, Void.
 */
#pragma once

#include <misc/singleton.hh>
#include <type/fwd.hh>
#include <type/type.hh>

namespace type
{
  class Int : public misc::Singleton<Int>
  {
    friend class misc::Singleton<Int>;

  private:
    Int(){};
  };

  class String : public misc::Singleton<String>
  {
    friend class misc::Singleton<String>;

  private:
    String(){};
  };

  class Void : public misc::Singleton<Void>
  {
    friend class misc::Singleton<Void>;

  private:
    Void(){};
  };
} // namespace type
