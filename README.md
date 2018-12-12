# nyx
[![Build Status](https://travis-ci.org/racaljk/nyx.svg?branch=master)](https://travis-ci.org/racaljk/nyx) | 
[![Build status](https://ci.appveyor.com/api/projects/status/ptqln5210xp6gupc?svg=true)](https://ci.appveyor.com/project/racaljk/nyx)

A dynamic typed scripting language :0

# language reference
## data type
+ `int`
+ `double` 
+ `string` 
+ `bool` 
+ `null`

## built-in functions
```nyx
# take various arguments and print their values to stdout
func print(a,b,c...)

# return string representations of type of given argument value
func typeof(a)
```

# structure
+ [Ast](nyx/Ast.h) as it name described, it contains AST definitions of **nyx**
+ [Nyx](nyx/Nyx.h) definitions of **nyx** runtime structures
+ [Parser](nyx/Parser.h) syntax parser and lexer
+ [Interpreter](nyx/Interpreter.h) interpret statements and eval expressions
