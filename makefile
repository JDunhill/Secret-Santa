CC:= gcc
CFLAGS:= -o

all: final

final: client.o server.o	
	echo "Compiling program"
	$(CC) $(CFLAGS) client.o server.o -o final


client.o: client.c 
	echo "Compiling Client" 
	$(CC) $(CFLAGS) client client.c

server.o: server.c 
	echo "Compiling Server" 
	$(CC) $(CFLAGS) server server.c

clean: 
	echo "removing everything"
	rm server.o client.o final