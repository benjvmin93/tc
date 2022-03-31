/**
 ** \file type/type-checker.cc
 ** \brief Implementation for type/type-checker.hh.
 */

#include <memory>

#include <ast/all.hh>
#include <range/v3/view/iota.hpp>
#include <type/type-checker.hh>
#include <type/types.hh>

namespace type
{
  namespace
  {
    // Try to cast the type into a nil type.
    // If it's not actually a `type::Nil`, return `nullptr`.
    // This allows us to write more clear code like:
    // if (auto nil = to_nil(e.type_get())
    // ...
    const Nil* to_nil(const Type& type)
    {
      return dynamic_cast<const Nil*>(&type.actual());
    }

  } // namespace

  TypeChecker::TypeChecker()
    : super_type()
    , error_()
  {}

  const Type* TypeChecker::type(ast::Typable& e)
  {
    if (e.type_get() == nullptr)
      e.accept(*this);
    return e.type_get();
  }

  const Record* TypeChecker::type(const ast::fields_type& e)
  {
    // FIXME: Some code was deleted here.
    auto res = new Record;
    //res->fields_ = e;
    return res;
  }

  const Record* TypeChecker::type(const ast::VarChunk& e)
  {
    auto res = new Record;
    for (const auto& var : e)
      res->field_add(var->name_get(), *type(*var));

    return res;
  }

  const misc::error& TypeChecker::error_get() const { return error_; }

  /*-----------------.
  | Error handling.  |
  `-----------------*/

  void TypeChecker::error(const ast::Ast& ast, const std::string& msg)
  {
    error_ << misc::error::error_type::type << ast.location_get() << ": " << msg
           << std::endl;
  }

  void TypeChecker::type_mismatch(const ast::Ast& ast,
                                  const std::string& exp1,
                                  const Type& type1,
                                  const std::string& exp2,
                                  const Type& type2)
  {
    error_ << misc::error::error_type::type << ast.location_get()
           << ": type mismatch" << misc::incendl << exp1 << " type: " << type1
           << misc::iendl << exp2 << " type: " << type2 << misc::decendl;
  }

  void TypeChecker::check_types(const ast::Ast& ast,
                                const std::string& exp1,
                                const Type& type1,
                                const std::string& exp2,
                                const Type& type2)
  {
    if (!type1.compatible_with(type2))
      type_mismatch(ast, exp1, type1, exp2, type2);
  }

  void TypeChecker::check_types(const ast::Ast& ast,
                                const std::string& exp1,
                                ast::Typable& type1,
                                const std::string& exp2,
                                ast::Typable& type2)
  {
    // Ensure evaluation order.
    check_types(ast, exp1, *type(type1), exp2, *type(type2));
    // FIXME: Some code was deleted here (Check types).
  }

  /*--------------------------.
  | The core of the visitor.  |
  `--------------------------*/

  /*-----------------.
  | Visiting /Var/.  |
  `-----------------*/

  void TypeChecker::operator()(ast::SimpleVar& e)
  {
    type_default(e, type(*(e.def_get())));
  }

  // FIXME: Some code was deleted here.
  /*
  void operator()(ast::FieldVar& e)
  {
    type_default(e, type(e.var_get()));
  }

  void operator()(const_t<SubscriptVar>& e)
  {
    type_default(e, type(e.var_get()));
  }*/

  /*-----------------.
  | Visiting /Exp/.  |
  `-----------------*/

  // Literals.
  void TypeChecker::operator()(ast::NilExp& e)
  {
    auto nil_ptr = std::make_unique<Nil>();
    type_default(e, nil_ptr.get());
    created_type_default(e, nil_ptr.release());
  }

  void TypeChecker::operator()(ast::IntExp& e)
  {
    auto int_ptr = Int::instance();
    type_default(e, int_ptr);
    if (int_ptr == nullptr)
      std::cout << "this is null\n";
    else
      std::cout << "wtf bro srl";
  }

  void TypeChecker::operator()(ast::StringExp& e)
  {
    auto str_ptr = String::instance();
    type_default(e, str_ptr);
  }

  // Complex values.

  void TypeChecker::operator()(ast::RecordExp& e)
  {
    // If no error occured, check for nil types in the record initialization.
    // If any error occured, there's no need to set any nil types.
    // If there are any record initializations, set the `record_type_`
    // of the `Nil` to the expected type.
    // FIXME: Some code was deleted here.
  }

  void TypeChecker::operator()(ast::OpExp& e)
  {
    auto int_ptr = Int::instance();
    e.left_get().accept(*this);
    e.right_get().accept(*this);
    //check_type(e.left_get(), "left operand type", *int_ptr);
    //check_type(e.right_get(), "right operand type", *int_ptr);
    std::cout << "there is no error here\n";
    /*type_default(e, int_ptr);
    // FIXME: Some code was deleted here.
    // If any of the operands are of type Nil, set the `record_type_` to the
    // type of the opposite operand.
    type(e.left_get());
    type(e.right_get());

    std::cout << "test\n";
    check_types(e, "left", e.left_get(), "right", e.right_get());
    
    if (!error_)
      {
        auto oper = e.oper_get();
        if (oper == ast::OpExp::Oper::add || oper == ast::OpExp::Oper::sub || oper == ast::OpExp::Oper::mul || oper == ast::OpExp::Oper::div)
        {
          if (e.left_get().type_get() != Int::instance() || e.right_get().type_get() != Int::instance())
            error(e, "expected INT");
        }
      }*/
  }

  // FIXME: Some code was deleted here.
  /*
    void operator()(const_t<CallExp>& e) override
    {
      //type_default(e, def_get());
    }

    void operator()(const_t<SeqExp>& e) override;
    void operator()(const_t<AssignExp>& e) override;
    void operator()(const_t<IfExp>& e) override;
    void operator()(const_t<WhileExp>& e) override;
    void operator()(const_t<ForExp>& e) override;
    void operator()(const_t<BreakExp>&) override;
    void operator()(const_t<LetExp>& e) override;
    void operator()(const_t<ArrayExp>& e) override;
    void operator()(const_t<CastExp>& e) override;
    void operator()(const_t<FieldInit>& e) override;
    */

  /*-----------------.
  | Visiting /Dec/.  |
  `-----------------*/

  /*------------------------.
  | Visiting FunctionChunk. |
  `------------------------*/

  void TypeChecker::operator()(ast::FunctionChunk& e)
  {
    chunk_visit<ast::FunctionDec>(e);
  }

  void TypeChecker::operator()(ast::FunctionDec& e)
  {
    // We must not be here.
    unreachable();
  }

  // Store the type of this function.
  template <>
  void TypeChecker::visit_dec_header<ast::FunctionDec>(ast::FunctionDec& e)
  {
    // FIXME: Some code was deleted here.
  }

  // Type check this function's body.
  template <>
  void TypeChecker::visit_dec_body<ast::FunctionDec>(ast::FunctionDec& e)
  {
    if (e.body_get())
      {
        //visit_routine_body<Function>(e);
        e.body_get()->accept(*this);
        // Check for Nil types in the function body.
        if (!error_)
          {
            // FIXME: Some code was deleted here.
          }
      }
  }

  /*---------------.
  | Visit VarDec.  |
  `---------------*/

  void TypeChecker::operator()(ast::VarDec& e)
  {
    // FIXME: Some code was deleted here.
  }

  /*--------------------.
  | Visiting TypeChunk. |
  `--------------------*/

  void TypeChecker::operator()(ast::TypeChunk& e)
  {
    chunk_visit<ast::TypeDec>(e);
  }

  void TypeChecker::operator()(ast::TypeDec&)
  {
    // We must not be here.
    unreachable();
  }

  // Store this type.
  template <> void TypeChecker::visit_dec_header<ast::TypeDec>(ast::TypeDec& e)
  {
    // We only process the head of the type declaration, to set its
    // name in E.  A declaration has no type in itself; here we store
    // the type declared by E.

    auto named = Named(e.name_get());
    e.create_type_set(e.type_get());
  }

  // Bind the type body to its name.
  template <> void TypeChecker::visit_dec_body<ast::TypeDec>(ast::TypeDec& e)
  {
    auto type = e.created_type_get();
    // TODO : Bind the type to the name of the dec.
  }

  /*------------------.
  | Visiting /Chunk/. |
  `------------------*/

  template <class D> void TypeChecker::chunk_visit(ast::Chunk<D>& e)
  {
    for (const auto& dec : e)
<<<<<<< HEAD
      {
        visit_dec_header(*dec);
        visit_dec_body(*dec);
      }
=======
      visit_dec_header<D>(*dec);
>>>>>>> 811229ce622ad61b9929463d510a40e12f7b14fc
  }

  /*-------------.
  | Visit /Ty/.  |
  `-------------*/

  void TypeChecker::operator()(ast::NameTy& e)
  {
    // FIXME: Some code was deleted here (Recognize user defined types, and built-in types).
  }

  void TypeChecker::operator()(ast::RecordTy& e)
  {
    // FIXME: Some code was deleted here.
  }

  void TypeChecker::operator()(ast::ArrayTy& e)
  {
    // FIXME: Some code was deleted here.
  }

} // namespace type
