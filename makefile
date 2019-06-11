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