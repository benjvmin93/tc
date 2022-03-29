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
  class Int : public misc::Singleton<Int>, public Type
  {
    friend class misc::Singleton<Int>;
    void accept(ConstVisitor& v) const override;
    void accept(Visitor& v) override;

  private:
    Int(){};
  };

  class String : public misc::Singleton<String>, public Type
  {
    friend class misc::Singleton<String>;
    void accept(ConstVisitor& v) const override;
    void accept(Visitor& v) override;

  private:
    String(){};
  };

  class Void : public misc::Singleton<Void>, public Type
  {
    friend class misc::Singleton<Void>;
    void accept(ConstVisitor& v) const override;
    void accept(Visitor& v) override;
  
  private:
    Void(){};
  };
} // namespace type
