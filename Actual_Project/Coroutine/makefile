.PHONY:clean main

cc=gcc
OBJS=Coroutine.o net_sever.o 
TARGET=net_svr net_cli

all:$(TARGET)

net_svr:$(OBJS)
	$(cc) $^ -o $@

net_cli:
	gcc net_client.c -o net_cli

clean:
	rm -rf $(OBJS) main
