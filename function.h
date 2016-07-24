// function.h :
// Templates to call mruby vm functions.
// Written by Klaus Beyer, 2015
//
// It makes it easy to call any function w/o worrying about the count
// of parameter and how to convert them to a mrb_value.
// Currently supported argument types are integral, floating point and strings.
// Examples:
// auto ret = make_call(mrb, self, "func_name", 33, "text", 12.5);
// auto ret = make_call(mrb, self, "func_name");
// auto ret = make_call(mrb, self, "func_name", 100);

#pragma once
#include <algorithm>
#include <string>
#include <vector>

#include <mruby.h>

namespace mruby {

template<typename T, typename std::enable_if< std::is_integral<T>::value>::type * = nullptr >
mrb_value mval(mrb_state* mrb, T val)
{
   (void) mrb;
   return mrb_fixnum_value(val);
}

template<typename T, typename std::enable_if< std::is_floating_point<T>::value>::type * = nullptr >
mrb_value mval(mrb_state* mrb, T val)
{
   return mrb_float_value(mrb, val);
}

template<typename T, typename std::enable_if< std::is_convertible<T, std::string>::value>::type * = nullptr >
mrb_value mval(mrb_state* mrb, T val)
{
   std::string s(val);
   return mrb_str_new(mrb, s.c_str(), s.size());
}

template<typename T>
void pack_param(mrb_state* mrb, std::vector<mrb_value>& pack, T first)
{
   pack.push_back(mval(mrb, first));
}

template<typename T, typename... Ts>
void pack_param(mrb_state* mrb, std::vector<mrb_value>& pack, T first, Ts... params)
{
   pack.push_back(mval(mrb, first));
   pack_param(mrb, pack, params...);
}


template<typename... Ts>
mrb_value make_call(mrb_state* mrb, mrb_value self, const char* fnname, Ts... params)
{
   std::vector<mrb_value> args;
   pack_param(mrb, args, params...);
   mrb_value argv[sizeof...(Ts)];
   int i = 0;
   for( auto p : args ) {
      argv[i++] = p;
   }
   mrb_sym mid = mrb_intern_cstr(mrb, fnname);
   mrb_value ret = mrb_funcall_argv(mrb, self, mid, args.size(), argv);
   return ret;
}

template<>
mrb_value make_call(mrb_state* mrb, mrb_value self, const char* fnname);

};