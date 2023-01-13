# NYX Language
**nyx** is yet another a dynamically typed scripting language with flexible syntax and strict type system.
Its syntax resembles Python,Java and C family languages. If you've already familiar with any other popular languages,
you can seamlessly write code after skimming its language reference manual.
In addition, nyx attempts to detect sorts of erroneous program forms at compile time that benefited from its underlying
strict type system, this could tremendously save your time for debugging type relevant bugs.

# Build
```bash
$ mkdir build && cd build
$ cmake ..
$ make
$ nyx <your_source_file.nyx>
```

# Code Examples
```$bash
$ nyx nyx_test/example/9x9table.ynx
1x1=1
1x2=2 2x2=4
1x3=3 2x3=6 3x3=9
1x4=4 2x4=8 3x4=12 4x4=16
1x5=5 2x5=10 3x5=15 4x5=20 5x5=25
1x6=6 2x6=12 3x6=18 4x6=24 5x6=30 6x6=36
1x7=7 2x7=14 3x7=21 4x7=28 5x7=35 6x7=42 7x7=49
1x8=8 2x8=16 3x8=24 4x8=32 5x8=40 6x8=48 7x8=56 8x8=64
1x9=9 2x9=18 3x9=27 4x9=36 5x9=45 6x9=54 7x9=63 8x9=72 9x9=81

$ nyx nyx_test/example/yang_hui_san_jiao.nyx
n = 1      [1]
n = 2      [1,1]
n = 3      [1,2,1]
n = 4      [1,3,3,1]
n = 5      [1,4,6,4,1]
n = 6      [1,5,10,10,5,1]
n = 7      [1,6,15,20,15,6,1]
n = 8      [1,7,21,35,35,21,7,1]
n = 9      [1,8,28,56,70,56,28,8,1]

$ nyx nyx_test/example/quick_sort.nyx
[4,-5,9,15,-6,-2,0] => [-6,-5,-2,0,4,9,15]

$ nyx nyx_test/example/love.nyx

                *********           *********
            *****************   *****************
          ****************************************
         *******************************************
        *********************************************
        *********************************************
        *********************************************
        *********************************************
        *********************************************
        *********************************************
         *******************************************
          *****************************************
          ****************************************
            *************************************
             ***********************************
              *********************************
                *****************************
                  *************************
                    *********************
                       ***************
                          *********
                             ***
```

## Language Reference
See [reference.md](./docs/reference.md) for detailed language reference，
[`nyx_test/*`](./nyx_test/) contains various nyx code snippets, they are probably the best way to learn how to program in **nyx**.

# License
**nyx** is licensed under the [MIT LICENSE](LICENSE)。
