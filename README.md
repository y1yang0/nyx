# nyx
[![Build Status](https://travis-ci.org/racaljk/nyx.svg?branch=master)](https://travis-ci.org/racaljk/nyx) | 
[![Build status](https://ci.appveyor.com/api/projects/status/ptqln5210xp6gupc?svg=true)](https://ci.appveyor.com/project/racaljk/nyx)

又一个动态类型的脚本语言：——）


# 语言手册
## 1.基础
**注释** 使用`#`可以注释一行代码。**nyx**不支持多行注释，对于多行需要重复`#`

## 2.数据类型
+ **int** 整数类型，如`3`,`100000`,`1024`
+ **double** 小数类型，如`3.1415926`,`2.232`，`4.4` 
+ **string** 字符串类型，如`"string"`,`"test"`,`""`
+ **bool** 布尔类型，值域只有字面值`true`和`false`
+ **null** 空值类型，用于指示该变量不具有值，值域只有字面值`null`

## 3.运算符
### 3.1 逻辑运算
`&&`表示逻辑与运算，`||`表示逻辑或运算,`!`表示逻辑运算。
。由于宿主语言实现是C++，这些逻辑运算也原生的具有**短路求值**特性。
```nyx
true&&false
false||true
!(false||false||false||(true||false))
```
### 3.2 条件运算
除了逻辑运算外，**nyx**也有完备的条件运算支持：`==`，`!=`，`>`，`>=`，`<`，`<=`:
```nyx
print((true&&false)==false)
print((false&&true)==false)
print((false&&false)==false)
print((true||false)==true)
print((true||true)==true)
print((false||false)==false)
print((false||true)==true)
```
另外值得注意的是`==`,`!=`运算符也支持`null`的条件比较。`null==null`总是为`true`,而`null!=null`总是为`false`。

## 4.内置函数
```nyx
# 接受任意数目的参数，向stdout输出
func print(a,b,c...)

# 接受一个参数，返回一个字符串用以表示实参类型
func typeof(a)
```

# 开源协议
**nyx**所有代码受[MIT LICENSE](LICENSE)保护。
