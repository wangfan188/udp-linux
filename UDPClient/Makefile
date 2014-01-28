
SHELL = /bin/sh
CC = gcc

ModleName := UDPClient

Objs += \
main.o

all:
	gcc -o $(Objs) -c main.c
	gcc -o $(ModleName) $(Objs) -lpthread -d 

clean:
	-rm -f $(ModleName) $(Objs)

