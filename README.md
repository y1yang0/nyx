# nyx
[![Build Status](https://travis-ci.org/racaljk/nyx-lang.svg?branch=master)](https://travis-ci.org/racaljk/nyx-lang) | 
[![Build status](https://ci.appveyor.com/api/projects/status/ptqln5210xp6gupc?svg=true)](https://ci.appveyor.com/project/racaljk/nyx) |
[![CircleCI](https://circleci.com/gh/racaljk/nyx-lang.svg?style=svg)](https://circleci.com/gh/racaljk/nyx-lang) |
[![Documentation Status](https://readthedocs.org/projects/nyx-lang/badge/?version=latest)](https://nyx-lang.readthedocs.io/zh/latest/?badge=latest) 

**nyx**是又一门看似**动态类型**实则有着**严格类型系统**的脚本语言

希望她仅有`python`的灵活，`c/c++`的实用性和语法形式，`java`的工程化，以及非理想主义者的哲学 ：——）

# 语言手册
参见[reference.md](./reference.md)

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

# 示例
1. 9x9乘法表
```nyx
for(a:range(1,10)){
    for(b=1;b<=a;b+=1){
        print(b+"x"+a+"="+a*b+" ")
    }
    println()
}

# 1x1=1 
# 1x2=2 2x2=4 
# 1x3=3 2x3=6 3x3=9 
# 1x4=4 2x4=8 3x4=12 4x4=16 
# 1x5=5 2x5=10 3x5=15 4x5=20 5x5=25 
# 1x6=6 2x6=12 3x6=18 4x6=24 5x6=30 6x6=36 
# 1x7=7 2x7=14 3x7=21 4x7=28 5x7=35 6x7=42 7x7=49 
# 1x8=8 2x8=16 3x8=24 4x8=32 5x8=40 6x8=48 7x8=56 8x8=64 
# 1x9=9 2x9=18 3x9=27 4x9=36 5x9=45 6x9=54 7x9=63 8x9=72 9x9=81
```
2. 杨辉三角
```nyx
func yang_hui_san_jiao(limit){
    row = [1]
    i = 1
    while(i<limit){
        k = 0
        nrow = []
        while(k<i){
            extend = 0
            if(k==0 || k==i-1){
                extend = 1
                
            }else{
                extend = row[k]+row[k-1]
            }
            nrow += extend
            k+=1
        }
        println("n = "+i+"      "+nrow)
        row = nrow
        i += 1
    }
}
yang_hui_san_jiao(10)

# console printing:
# n = 1      [1]
# n = 2      [1,1]
# n = 3      [1,2,1]
# n = 4      [1,3,3,1]
# n = 5      [1,4,6,4,1]
# n = 6      [1,5,10,10,5,1]
# n = 7      [1,6,15,20,15,6,1]
# n = 8      [1,7,21,35,35,21,7,1]
# n = 9      [1,8,28,56,70,56,28,8,1]
```
更多示例参见[`nyx_test/interesting/*`](./nyx_test/interesting)

# 开源协议
**nyx**所有代码基于[MIT LICENSE](LICENSE)协议发布。
