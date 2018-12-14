# nyx
[![Build Status](https://travis-ci.org/racaljk/nyx.svg?branch=master)](https://travis-ci.org/racaljk/nyx) | 
[![Build status](https://ci.appveyor.com/api/projects/status/ptqln5210xp6gupc?svg=true)](https://ci.appveyor.com/project/racaljk/nyx) |
[![Documentation Status](https://readthedocs.org/projects/nyx-lang/badge/?version=latest)](https://nyx-lang.readthedocs.io/zh/latest/?badge=latest)
  

又一门动态类型的脚本语言
希望**nyx**有python的语法灵活，c系的语法形式，以及非理想主义者的哲学 ：——）


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
### 3.1 四则运算
`+,-,*,/,%`是计算的基石，所以没有不支持的道理。在**nyx**中四则运算的优先级和运算规则与其它语言无二：
```nyx
d = (3+2)*4+(6*5)-8/2+(3+2*(5-4))
c = -7
a = 3+2-5
b = 3+5*2%2
print(a,b,c,d)                 
q = (((((((((((((((((((1)+1)+1)+1)+1)+1)+1)+1)+1)+1)+1)+1)+1)+1)+1)+1)+1)+1)+1)+1
print(q)
```
对字符类型进行`+`运算得到的是两个字符串拼接后的结果:
```nyx
print("hello,"+"world") #will print hello,world
```

### 3.2 逻辑运算
`&&`表示逻辑与运算，`||`表示逻辑或运算,`!`表示逻辑非运算。
。由于宿主语言是C++，这些逻辑运算也原生的具有[**短路求值**](https://en.wikipedia.org/wiki/Short-circuit_evaluation)特性。
```nyx
true&&false
false||true
!(false||false||false||(true||false))
```
### 3.3 条件运算
除了逻辑运算外，**nyx**也有完备的条件运算支持：`==`，`!=`，`>`，`>=`，`<`，`<=`:
```nyx
print((true&&false)==false)
print((false&&true)==false)
print((true||false)==true)
print((true||true)==true)
print(5>3 && 6<10 && (14>=13||13<=15))
```
另外值得注意的是`==`,`!=`运算符也支持`null`的条件比较:
+ `null==null`总是为`true`
+ `null!=null`总是为`false`。

### 3.4 位运算
条件运算类似于C系语言：
```
# 位与
print(3&5) # 0011 & 0101 => 1

# 位或
print(4|66) # 00000100 & 01000010 => 70

# 位反
print(~43) # 00101011 =>11010100 => -44
```

## 5. 流程控制
## 5.1 if-else分支跳转
`if`语句可以根据条件进行分支跳转。单个`if`分支跳转和`if-else`分支跳转都是允许的：
```nyx
a = input()
if(a+1 == "whatsup"){
    print("fine")
}
b = 10
if(b <10){
    print("b is less than 10")
}else{
    print("b is greater equal than 10")
}
```
## 5.2 while循环
**nyx**不打算在公共语言基础上标新立异，它的`while`做了与其他大多数语言一样的事情，即根据条件进行循环。
```nyx
a= 1
while(a<100){
    print("counter:"+a)
    a = a+1
}
```


## 4.内置函数
```nyx
# 接受任意数目的参数，向stdout输出
func print(a,b,c...)

# 无参数。接受stdin输入并返回输入字符串
func input()

# 接受一个参数，返回一个字符串用以表示实参类型
func typeof(a)
```

# 开源协议
**nyx**所有代码受[MIT LICENSE](LICENSE)保护。
