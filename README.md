# CILisp
CILisp is Lisp-spinoff compiler written in C using Flex and Bison.

Some examples of a program written in CILisp that can be compiled by this compiler are:
```
(add ((let (abcd 1)) (sub 3 abcd)) 4)
```
```
(add ((let (a ((let (b 2)) (mult b (sqrt 10))))) (div a 2)) ((let (c 5)) (sqrt c)))
```
```
((let (integer a (read)) (real b (read))) (print a b))
```

### Supported Operations
  - negation
  - absolute value
  - exponential
  - square root
  - addition
  - subtract
  - multiplication
  - division
  - remainder
  - log
  - power
  - max
  - min
  - exp2 (base-2 exponential)
  - cube root
  - hypotenuse
  - print (output variable)
  - read (input from user)
  - equal ( == )
  - smaller ( < )
  - larger ( > )
  
### Additional Features
CILisp supports user-defined functions as well as recursion. On top of mathematical operations, 
there is support for input/output, conditionals, and variables. Op lists allow for multiple inputs into functions that can be run on multiple inputs. The two supported data types are reals and integers.

### What CILisp Does
CILisp performs both sytactic and semantic analysis to produce an intermediate form of the compiled code.
