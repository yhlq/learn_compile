# linux 编译c/c++

* 编译hello word

新建文件hello.cpp, 内容如下. 

```
/*hello.cpp*/
#include<stdio.h>

int main()
{
    char s[] = "hello word!";
    printf("%s\n", s);
    return 0;
}
```

> g++ -c hello.cpp
输出结果是一个hello.o文件，这是编译过程的生成的中间文件。-c 表示只编译，不链接。

> g++ -o hello.out hello.o
输出结果是一个hello.out文件，这是最终的可执行文件。-o 表示输出文件，hello.o是上一步生成的.o文件

*上述两步是可以合并执行的  命令如: g++ -o hello.out hello.cpp*

> ./hello.out
```
hello word!
```

当存在多个文件且相互依赖时可采用make编译.
## makefile文件概述
Makefile文件由一系列规则（rules）构成。每条规则的形式如下。
```
<target> : <prerequisites> 
[tab]  <commands>
```
上面第一行冒号前面的部分，叫做"目标"（target），冒号后面的部分叫做"前置条件"（prerequisites）；第二行必须由一个tab键起首，后面跟着"命令"（commands）。
"目标"是必需的，不可省略；"前置条件"和"命令"都是可选的，但是两者之中必须至少存在一个。每条规则就明确两件事：构建目标的前置条件是什么，以及如何构建。下面就详细讲解，每条规则的这三个组成部分。

## 应用实例 
现在有三个文件:
main.cpp
```
/* main.cpp */
#include "Tmath.h"
#include <stdio.h>
int main()
{

    double tab[3] = {1.0,2.0,3.0};
    double avg_ = avg(tab,3);
    printf("this is in test_a %f...\n", avg_);
    return 0;
}
```
Tmath.h
```
/* Tmath.h */
#include <math.h>

/* Average values in an array */
double avg(double *a, int n) ;
```
Tmath.cpp
```
/* Tmath.cpp */
#include "Tmath.h"

double avg(double *a, int n) {
    int i;
    double total = 0.0;
    for (i = 0; i < n; i++) {
        total += a[i];
    }
    return total / n;
}
```
新建文件 makefile 并写入下列指令

```
build: main.o Tmath.o                 // 得到build 的条件是 存在main.o和Tmath.o 即main.cpp 和 Tmath.cpp 编译中间结果
	g++ -o build main.o Tmath.o   // 通过g++ 命令构建
main.o: main.cpp Tmath.h		// 下面的命令与第一条类似
	g++ -g -c main.cpp
Tmath.o: Tmath.h Tmath.cpp
	g++ -g -c Tmath.cpp
clean:                                     // 默认不执行, 只有显式的调用时[make clean], 删除生成的三个文件(main.o Tmath.o build).
	rm main.o Tmath.o build
```

cd 至当前makefile文件所在目录, 执行
```
make
```
即可得到 可执行文件 build
执行 './build'   // 得到结果 2.0000




[参考](https://www.cnblogs.com/zhxilin/p/5881080.html)
[makefile文件编写](http://www.ruanyifeng.com/blog/2015/02/make.html)


## 动态链接库生成&调用


makefile文件
```
# 如果仅执行 make   默认只执行第一个目标 也就是 build. result.cpp 不会执行
build: main.o Tmath.o
	# 执行build
	g++ -o build main.o Tmath.o
main.o: main.cpp Tmath.h
	g++ -g -c main.cpp
Tmath.o: Tmath.h Tmath.cpp
	g++ -g -c Tmath.cpp

# 只有运行 make result.cpp   才能得到执行
result.cpp: main.cpp
	# 执行result.cpp
	cp main.cpp result.cpp


test: libs.so
	g++ test.cpp -ldl -o test
libs.so: Tmath.o
	gcc Tmath.o -shared -o libs.so
Tmath.o: Tmath.h Tmath.cpp
	g++ -g -c Tmath.cpp

.PHONY: clean  # 声明clean是一个伪目标
clean: 
	rm main.o Tmath.o build libs.so

```

test.cpp
```
#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <iostream>
using namespace std;
int main()
{
	double a = 0;
	
	void *handle = dlopen("./libs.so", RTLD_LAZY);
	
	if(!handle)
	{
		printf("open lib error\n");
		cout<<dlerror()<<endl;
		return -1;
	}
    printf("open libs succedd!!!");
	// double avg(double *a, int n) ;
    // int add(int a, int b);
	typedef double (*avg_t)(int a, int b);
	avg_t avg = (avg_t) dlsym(handle, "add");
	if(!avg)
	{   
        cout<<"@@@@@@@@"<<endl;
		cout<<dlerror()<<endl;
		dlclose(handle);
		return -1;
	}
	printf("start cale!!!");

	double tab[3] = {1.0,2.0,3.0};
    
	a = avg(1, 3);
	printf("a = %f\n",a);
	dlclose(handle);
	return 0;
}

```
执行

```
make test
```
这个指令会生成 libs.so[Tmath.cpp和Tmath.h 编译得到的动态链接库] 和 test[可执行文件]. 输出:2.0000

##注意编译动态链接库的时候最好指定编译为C风格的(不同编译器编译的函数名字不同): 这里在 Tmath.h 声明函数时指定修饰规则即可[关键字 extern "C" ]##
```
/* Tmath.h */
#include <math.h>

/* Average values in an array */
extern "C" double avg(double *a, int n) ;
extern "C" int add(int a, int b);
```