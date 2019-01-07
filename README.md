# nyx
[![Build Status](https://travis-ci.org/racaljk/nyx.svg?branch=master)](https://travis-ci.org/racaljk/nyx) | 
[![Build status](https://ci.appveyor.com/api/projects/status/ptqln5210xp6gupc?svg=true)](https://ci.appveyor.com/project/racaljk/nyx) |
[![CircleCI](https://circleci.com/gh/racaljk/nyx.svg?style=svg)](https://circleci.com/gh/racaljk/nyx) |
[![Documentation Status](https://readthedocs.org/projects/nyx-lang/badge/?version=latest)](https://nyx-lang.readthedocs.io/zh/latest/?badge=latest) 

**nyx** is yet another a dynamically typed scripting language with flexible syntax and strict type system. Its syntax resembles Python,Java and C family languages. We can seemlessly wirte code after skimming language reference manual if you've already familiar with any other popular languages. Moreover, nyx attempts to detect sorts of erroneous program forms at compile time that benefitted from its underlying strict type system, this could tremendously save your time for debugging those type relevant bugs.

# Features
+ `Closure`, which takes functions as the first-class values
+ `Top-level evaluation`
+ `Pattern matching`
 And there is more to its featues than we've highlighted here, you can check language reference manual to get a full list.

# Language reference
See [reference.md](./reference.md) for detailed language reference，
[`nyx_test/*`](./nyx_test/) contains various demos, they are probably the best way to learn how to program.

# Build
```bash
$ mkdir build && cd build
$ cmake ..
$ make
$ nyx <your_source_file.nyx>
```

# Hacking
```bash
racaljk@ubuntu:~/Desktop/nyx-lang/nyx$ tree .
.
├── Ast.h               // Definitions of AST nodes
├── Builtin.cpp         // Functions that had been built in language core set
├── Builtin.h           
├── Interpreter.cpp     // Implementation of interpretere
├── Interpreter.h
├── Main.cpp            // Launcher
├── Nyx.cpp             // Runtime structures such as nyx::Runtime,nyx::Context
├── Nyx.hpp             // 
├── Parser.cpp          // Lexer and parser
├── Parser.h
├── Utils.cpp           // Auxiliary functions
└── Utils.hpp
```

# License
**nyx** is licensed under the [MIT LICENSE](LICENSE)。
