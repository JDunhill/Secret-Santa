CC:= gcc
CFLAGS:= -c

all: clientfinal serverfinal

clientfinal: client.o	
	@echo "Compiling program"
	$(CC) client.o -o clientfinal

serverfinal: server.o	
	echo "Compiling program"
	$(CC) server.o -o serverfinal


client.o: client.c 
	@echo "Compiling Client" 
	$(CC) $(CFLAGS) client.c

server.o: server.c 
	@echo "Compiling Server" 
	$(CC) $(CFLAGS) server.c

clean: 
	@echo "removing everything"
	rm a.out client server.o client.o serverfinal clientfinal