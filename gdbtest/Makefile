target=app
SRC=$(wildcard ./src/*.cpp)
SRC += main.cpp
OBJ=$(patsubst %.cpp, %.o, $(SRC))
CC=g++
CPPFLAGS=-Iinclude

$(target):$(OBJ)
	$(CC) $^ -o $@ $(CPPFLAGS)

%.o:%.c
	$(CC) -c $< -o $@ $(CPPFLAGS)

.PHONY:clean #声明伪目标
clean:
	-mkdir /hqf #命令执行出错会忽略继续执行下面的命令
	rm -rf $(OBJ) $(target)
