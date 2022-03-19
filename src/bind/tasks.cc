/**
 ** \file bind/tasks.cc
 ** \brief Bind module tasks implementation.
 */

 #include <iostream>

 #include <bind/libbind.hh>
 #include <ast/tasks.hh>
 #include <bind/tasks.hh>
 #include <ast/libast.hh>

 namespace bind::tasks
 {
     void bind_compute() { bind::bind_compute(*ast::tasks::the_program); }

     void bind_display() { ast::bindings_display(std::cout) = true; }
 }
