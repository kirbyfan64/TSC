# Alias Armadillo to Army so that both names work.
Army = Armadillo

# A Point represents a X/Y position in a level.
Point = Struct.new(:x, :y)

# A Rect instance represents a rectangle.
Rect = Struct.new(:x, :y, :width, :height)

##
# Module: Std
#
# Namespace for the TSC Standard Scripting Library (SSL). The classes
# and modules defined in this namespace are not written directly in C++,
# but in Ruby itself.
module Std
end
