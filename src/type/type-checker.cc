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
    auto int_ptr = &Int::instance();
    type_default(e, int_ptr);
  }

  void TypeChecker::operator()(ast::StringExp& e)
  {
    auto str_ptr = &String::instance();
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
    // INFORMATION
    // Recupere l'instance de int
    // accept le left puis accept le right pour qu'ils recuperent leurs type
    // Grace au parcours des noeuds
    // check le type pour savoir si c'est des int
    // Ce sera a changer parce qu'on peut avoir des records et des type_dec
    auto int_ptr = &Int::instance();
    e.left_get().accept(*this);
    e.right_get().accept(*this);
    check_types(e, "left operand type", *e.left_get().type_get(),
                "expected type", *int_ptr);
    check_types(e, "right operand type", *e.right_get().type_get(),
                "expected type", *int_ptr);
    type_default(e, int_ptr);
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

  void TypeChecker::operator()(ast::IfExp& e)
  {
    // INFORMATION
    // accept le test
    // accept le then
    // accept le else s'il existe
    // apres avoir accept, les noeuds ont recup leur types grace au parcours
    // on peut check_types le then et else pour savoir si c'est le meme type
    // si il n'y a pas de else on doit check le then avec un void
    e.get_test().accept(*this);
    e.get_thenclause().accept(*this);
    if (e.get_elseclause())
      {
        e.get_elseclause().accept(*this);
        check_types(e, "then clause type", e.get_thenclause().type_get(),
                    "else clause type", e.get_elseclause().type_get());
      }
    else
      {
        auto void_ptr = &Void::instance();
        check_types(e, "then clause type", e.get_thenclause().type_get(),
                    "else clause type", *void_ptr);
      }
    type_default(e, e.get_thenclause().type_get());
  }
  void TypeChecker::operator()(ast::ForExp& e)
  {
    auto int_ptr = &Int::instance();
    auto void_ptr = &Void::instance();
    type_default(e, void_ptr);

    e.vardec_get().accept(*this);

    check_types(e, "index type", e.vardec_get().type_get(), "expected type",
                *int_ptr);
    e.hi_get().accept(*this);
    check_types(e, "high bound type", e.hi_get().type_get(), "expected type",
                *int_ptr);
    e.body_get().accept(*this);

    check_types(e, "for type", e.body_get().type_get(), "expected type",
                *void_ptr);
    // INFORMATION
    // check si la variable de l'index est un int et si le high bound est aussi un int
    // check si le body a bien un void en retour
    // TODO
    // check que la variable de l'index n'es pas incrementer ou decrementer dans le body
  }

  void TypeChecker::operator()(ast::WhileExp& e)
  {
    auto void_ptr = &Void::instance();
    type_default(e, void_ptr);

    e.test_get().accept(*this);
    e.body_get().accept(*this);
    check_types(e, "while type", e.body_get().type_get(), "expected type",
                *void_ptr);
    // INFORMATION
    // accept le test puis le body
    // check si le body est bien un void
    // TODO
    // peut etre quelque chose mais j'ai pas d'idee
    // ca se trouve c'est parfait ?
  }

  void TypeChecker::operator()(ast::BreakExp& e)
  {
    // INFORMATION
    // recuperer le type de la loop
    type_default(e, type(*(e.def_get())));
  }

  void TypeChecker::operator()(ast::CallExp& e)
  {
    // INFORMATION
    // recuperer le type de la fonction de deifnition
    // accept la liste d'arguments
    type_default(e, type(*(e.def_get())));
    e.args_get().accept(*this);
  }

  void TypeChecker::operator()(ast::LetExp& e)
  {
    // INFORMATION
    // visit la chunklist
    // visit les expressions qu'il y a dans 'in'
    // check si le in est bien un void puisque un letExp doit rien return
    e.chunklist_get().accept(*this);
    e.exp_get().accept(*this);
    auto void_ptr = &Void::instance();

    check_types(e, "in type", e.exp_get().type_get(), "expected type",
                *void_ptr);
    type_default(e, void_ptr);
  }

  void TypeChecker::operator()(ast::SeqExp& e)
  {
    // INFORMATION
    // visit les expressions
    // le type default est void je crois mais info a verifier
    auto exps = e.exps_get();
    for (auto exp : exps)
      {
        exp->accept(*this);
      }
    auto void_ptr = &Void::instance();
    type_default(e, void_ptr);
  }

  void TypeChecker::operator()(ast::AssignExp& e)
  {
    // INFORMATION
    // accept les expressions
    // check si la variable de gauche est du meme type que le calcul a droite
    // TODO
    // Peut etre gerer aussi le cas du NIL mais info a verifier
    e.var_get().accept(*this);
    e.exp_get().accept(*this);

    check_types(e, "left operand type", e.var_get().type_get(),
                "right operand type", e.exp_get().type_get());
    type_default(e, e.var_get().type_get());
  }

  void TypeChecker::operator()(ast::ArrayExp& e)
  {
    // INFORMATION
    // accept les expressions
    // check si le type du nom est bien le meme que le type de l'init
    // Exemple : ArrInt = array of int
    // arr : arrInt := arrInt [10] of 0
    // C'est un array of int donc l'init est 0 et ne doit pas etre une string
    // Check aussi si la size est bien un int
    // TODO
    // Sans doute d'autre chose a check mais je sais pas a verifier
    e.type_name_get().accept(*this);
    e.size_get().accept(*this);
    e.init_get().accept(*this);
    auto int_ptr = &Int::instance();

    check_types(e, "Array type", e.type_name_get().type_get(), "Array variable",
                e.init_get().type_get());

    check_types(e, "Array size type", e.size_get().type_get(), "Expected type",
                *int_ptr);
    type_default(e, e.type_name_get().type_get());
  }

  void TypeChecker::operator()(ast::CastExp& e)
  {
    // INFORMATION
    // accept les expressions
    // Mettre le type default au nouveau type du cast
    // TODO
    // Je ne sais pas si c'est bon et y a de tres grande chance qu'il
    // faut rajouter d'autre bout de code
    e.exp_get().accept(*this);
    e.ty_get().accept(*this);
    type_default(e, e.ty_get().type_get());
  }

  void TypeChecker::operator()(ast::FieldInit& e)
  {
    // INFORMATION
    // accept les expressions
    // TODO
    // reussir a retrouver le type du misc::symbol pour check si l'init
    // est du meme type que la variable
    // PROBLEME
    // un misc symbol n'a pas de type donc je ne sais pas comment faire
    e.init_get().accept(*this);
  }

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
    if (&e.formals_get())
      e.formals_get().accept(*this);
    if (e.result_get())
      {
        e.result_get()->accept(*this);
        type_default(e, type(*e.result_));
      }
    else
      {
        auto void_ptr = Void::instance();
        type_default(e, void_ptr);
      }
    // INFORMATION
    // Sauvegarder le type de la declaration de fonction avec type_default
    // Le type de la declaration est recuperer avec NameTy
    // Si NameTy (result_get()) est nullptr, ca veut dire que c'est une fonction
    // du type void
  }

  // Type check this function's body.
  template <>
  void TypeChecker::visit_dec_body<ast::FunctionDec>(ast::FunctionDec& e)
  {
    if (e.body_get())
      {
        // INFORMATION
        // Accept le body et fait une visit du body avec visit_routine_body
        // TODO
        // On doit recuperer le type du body d'une maniere ou d'une autre
        // Puis faire un check types avec le qui a ete sauvegarder plus haut

        e.body_get()->accept(*this);
        //visit_routine_body<Function>(e);
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

    auto named = new Named(e.name_get());
    e.create_type_set(e.type_get());
    e.type_set(e.created_type_get());
  }

  // Bind the type body to its name.
  template <> void TypeChecker::visit_dec_body<ast::TypeDec>(ast::TypeDec& e)
  {
    auto typ = dynamic_cast<const Named*>(e.created_type_get());
    typ->type_set(type(e.ty_get()));
    // TODO : Bind the type to the name of the dec.
  }

  /*------------------.
  | Visiting /Chunk/. |
  `------------------*/

  template <class D> void TypeChecker::chunk_visit(ast::Chunk<D>& e)
  {
    for (const auto& dec : e)
      {
        visit_dec_header(*dec);
        visit_dec_body(*dec);
      }
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
