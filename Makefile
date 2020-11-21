
CC = gcc

.PHONY: all clean

all: server client
	make server
	make client

server:
	$(CC) iserver.c -o server -lpthread

client:
	$(CC) -o guiclient guiclient.c `pkg-config --libs gtk+-2.0 libvlc` `pkg-config --cflags gtk+-2.0 libvlc`

clean:
	rm server guiclient
