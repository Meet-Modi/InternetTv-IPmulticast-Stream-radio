
CC = gcc

.PHONY: all clean

all: server client
	make server
	make client

server:
	$(CC) iserver.c -o server.out -lpthread

client:
	$(CC) -o guiclient.out guiclient.c `pkg-config --libs gtk+-2.0 libvlc` `pkg-config --cflags gtk+-2.0 libvlc`

clean:
	rm server.out guiclient.out
