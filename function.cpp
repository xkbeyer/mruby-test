
#include "stdafx.h"

#include "function.h"

template<>
mrb_value mruby::make_call(mrb_state* mrb, mrb_value self, const char* fnname)
{
   return mrb_funcall(mrb, self, fnname, 0);
}
