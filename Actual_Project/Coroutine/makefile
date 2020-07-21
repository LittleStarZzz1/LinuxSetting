.PHONY:clean main

cc=gcc
OBJS=Coroutine.o main.o
TARGET=svr

all:$(TARGET)

svr:$(OBJS)
	$(cc) $^ -o $@

cli:
	gcc client.c -o cli

clean:
	rm -rf $(OBJS) main
