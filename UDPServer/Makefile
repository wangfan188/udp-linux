
SHELL = /bin/sh
CC = gcc

ModleName := UDPServer

Objs += \
main.o \
xlist.o



Libs += -g -lpthread

all:$(Objs) $(ModleName)

$(Objs):%.o:%.c
	$(CC) -o $@ -c $<

$(ModleName):
	$(CC) -o $(ModleName) $(Objs) $(Libs)


clean:
	-rm -f $(ModleName) $(Objs)

