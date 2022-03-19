/**
 ** \file bind/tasks.cc
 ** \brief Bind module tasks implementation.
 */

 #include <iostream>

 #include <bind/libbind.hh>
 #include <ast/tasks.hh>
 #define DEFINE_TASKS 1
 #include <bind/tasks.hh>
 #undef DEFINE_TASKS
 #include <ast/libast.hh>

 namespace bind::tasks
 {
     void bind_compute()
     { 
        auto err = bind::bind_compute(*ast::tasks::the_program);
        if (err)
            err.exit();
     }

     void bind_display() { ast::bindings_display(std::cout) = true; }
 }
