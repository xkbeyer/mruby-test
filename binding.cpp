
#include <ctime>
#include <iostream>
#include <sstream>
#include <vector>
#include <chrono>

#include <mruby.h>
#include <mruby/compile.h>
#include <mruby/string.h>
#include <mruby/class.h>

#include "binding.h"

class ACppClass
{
public:
   ACppClass() = default;
   void setNumber(int n) { aNumber = n; }
   int  getNumber() const { return aNumber; }
private:
   int aNumber = 0;
};

mrb_value testBinding(mrb_state * mrb)
{
   // Export a C++ class
   auto rubyclass = mruby::bindclass<ACppClass>::it(mrb, "ACppClass");
   mruby::bind<ACppClass, decltype(&ACppClass::getNumber), void, 0>::method(mrb, rubyclass, "get", &ACppClass::getNumber);
   auto ret1 = mrb_load_string(mrb, "app = ACppClass.new\n p app.get()");
   return ret1;
}

