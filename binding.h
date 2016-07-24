// bindcpp.h :
// 
// Written by Klaus Beyer, 2015
//

#pragma once

#include <string>

#include <mruby.h>
#include <mruby/compile.h>
#include <mruby/string.h>
#include <mruby/class.h>
#include <mruby/proc.h>
#include <mruby/data.h>

#include "function.h"

mrb_value testBinding(mrb_state * mrb);

namespace mruby {

template<class _ct, class FunctionType, class ReturnType>
struct func_bind
{
   static mrb_value func(mrb_state* mrb, mrb_value self)
   {
      auto rclass = mrb_class_ptr(self);
      _ct* c = (_ct*) DATA_PTR(self);
      mrb_value method = mrb_cfunc_env_get(mrb, 0);
      using MethodType = ReturnType(_ct::*)();
      auto pMethod = *(MethodType*) mrb_ptr(method);
      auto rc = (c->*pMethod)();

      mrb_value v = mrb_fixnum_value(rc);
      return v;
   }

   static mrb_value ctor(mrb_state* mrb, mrb_value self)
   {
      mrb_data_type* dt = new mrb_data_type{ "CppClass", &func_bind::dtor };
      DATA_TYPE(self) = dt;
      _ct* inst = new _ct;
      DATA_PTR(self) = (void*)inst;
      return self;
   }

   static void dtor(mrb_state* self, void*)
   {
   }
};

template<class T>
struct bindclass
{
   static struct RClass* it(mrb_state* mrb, std::string name) 
   {
      auto self = mrb_define_class(mrb, name.c_str(), mrb->object_class);

      mrb_func_t fn = &func_bind<T, void, void>::ctor;
      mrb_define_method(mrb, self, "initialize", fn, MRB_ARGS_ANY());
      return self;
   };
};

template<class T, typename FunctionType, typename ArgType, int ARGS>
struct bind
{
   static void method(mrb_state* mrb, struct RClass * self, std::string fname, FunctionType f)
   {
      mrb_func_t fn = &func_bind<T, FunctionType, int>::func;
      mrb_sym symb = mrb_intern_cstr(mrb, fname.c_str());
      mrb_value cfunc;
      cfunc.value.p = &f;
      cfunc.tt = MRB_TT_CPTR;
      mrb_value env[] = { cfunc, mrb_symbol_value(symb) };
      auto func = mrb_proc_new_cfunc_with_env(mrb, fn, 2, env);
      mrb_define_method_raw(mrb, self, symb, func);
   };
};

}
