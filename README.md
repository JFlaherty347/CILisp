# CILisp
CILisp is a Lisp-spinoff compiler written in C using Flex and Bison.

### Example Programs
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
  
### Features
CILisp supports user-defined functions as well as recursion. On top of mathematical operations, 
there is support for input/output, conditionals, and variables. Op lists allow for multiple inputs into functions that can be run on multiple inputs. The two supported data types are reals and integers. CILisp employs static scoping, in which it will search for a variable in the symbol table in the current scope first and then each previous parent scope until it finds the variable in question.

### What CILisp Does
CILisp performs both syntactic and semantic analysis to produce an intermediate form of the compiled code. This intermediate
form is made up of various types of nodes that carry different pieces of information. CILisp uses a context-free grammar,
meaning that all transition rules are available at any point.

### Takeaways from CILisp
While CILisp is a great exercise in creating a compiler, I found the most value in understanding exactly how compilers are
implemented. In most software development environments, efficiency is vital to success. Knowing exactly how the high-level code
I write should be compiled allows me to write code that can be optimized by a compiler. Furthermore, CILisp was an excellent
lesson in project planning. Over several weeks I was tasked with finishing CILisp and it was my responsibility
to schedule my work in order to complete the task on time. Finally, with the large list of features in CILisp, I learned how
to write robust code that is easily expandable. Through adding many features I was able to create a framework that could be
easily iterated and improved so that CILisp could not only implement new features but maintain the usefulness of previous features.

### A Note About This Repository
Please note that this repository was created after CILisp was programmed and as a result only contains minor update commits rather than major functionality updates. This project was created during the sophomore year of my undergraduate studies at
CSUCI.
