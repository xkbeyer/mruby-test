# Test ruby file for mruby.

def foo()
  p 'called foo.'
end

def bar( arg )
  p "called bar(#{arg})."
end

def fun( zahl, text, fixzahl )
  p "fun(#{zahl}, #{text}, #{fixzahl}"
end

class Klasse
  attr_accessor :member_var
  
  def initialize
    @member_var = 123
    @desc = 'Klasse description'
  end
  def get_desc
    @desc
  end
  def print
    t_printstr @desc
  end
  def self.bar
    p 'called ruby class method'
  end
end

# How much time takes the floating point arithmetic in the interpreter?
def loadcpu( num1, num2 )
  res = 0.0
  (1..1000000).each do |i|
    res += num1 / num2 * i
  end
  res
end  

# This w/ a call back to C++ is even slower than the pure ruby one.
def loadcpu2( num1, num2 )
  res = 0.0
  (1..1000000).each do |i|
    res = c_mul_add_add(res, num1, num2,i)
  end
  res
end  

# This is the easy one, just put everything into the C++ call back.
def loadcpu3( num1, num2 )
  c_mul_add_add_loop(1000000, num1, num2)
end  


p 'In test.rb'
#x = loadcpu 1.5, 23
#puts x
p 'end test.rb'
