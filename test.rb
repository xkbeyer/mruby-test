# Test ruby file for mruby.

def foo()
  p 'called foo.'
end

def bar( arg )
  p "called bar(#{arg})."
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

p 'In test.rb'

p 'end test.rb'
