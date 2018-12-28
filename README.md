# nyx
[![Build Status](https://travis-ci.org/racaljk/nyx.svg?branch=master)](https://travis-ci.org/racaljk/nyx) | 
[![Build status](https://ci.appveyor.com/api/projects/status/ptqln5210xp6gupc?svg=true)](https://ci.appveyor.com/project/racaljk/nyx) |
[![CircleCI](https://circleci.com/gh/racaljk/nyx-lang.svg?style=svg)](https://circleci.com/gh/racaljk/nyx-lang) |
[![Documentation Status](https://readthedocs.org/projects/nyx-lang/badge/?version=latest)](https://nyx-lang.readthedocs.io/zh/latest/?badge=latest) 

**nyx**是又一门看似**动态类型**实则有着**严格类型系统**的脚本语言

希望她仅有`python`的灵活，`c/c++`的实用性和语法形式，`java`的工程化，以及非理想主义者的哲学 ：——）

# 语言手册
语言手册参见[reference.md](./reference.md)，
代码示例参见[`nyx_test/*`](./nyx_test/)

# 源码构建
```bash
$ mkdir build && cd build
$ cmake ..
$ make
$ nyx <your_source_file.nyx>
```

# 开发引导
```bash
racaljk@ubuntu:~/Desktop/nyx-lang/nyx$ tree .
.
├── Ast.h               // ast结构定义
├── Builtin.cpp         // 语言内置函数
├── Builtin.h           
├── Interpreter.cpp     // 解释器实现
├── Interpreter.h
├── Main.cpp            // 启动函数
├── Nyx.cpp             // nyx::Runtime,nyx::Context等运行时结构
├── Nyx.hpp             // 
├── Parser.cpp          // 语法分析器和词法分析器
├── Parser.h
├── Utils.cpp           // 辅助函数
└── Utils.hpp
```

# 开源协议
**nyx**所有代码基于[MIT LICENSE](LICENSE)协议发布。
