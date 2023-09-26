CC:= gcc
CFLAGS:= 

all: serverfinal clientfinal

clientfinal: client.o userIO.o shared.o
	@echo "Compiling program"
	$(CC) client.o userIO.o shared.o -o clientfinal

serverfinal: server.o	
	@echo "Compiling program"
	$(CC) server.o -o serverfinal


client.o:   client.c userIO.c shared.c
	@echo "Compiling Client" 
	$(CC) $(CFLAGS) -c client.c userIO.c shared.c

server.o: server.c 
	@echo "Compiling Server" 
	$(CC) $(CFLAGS) -c server.c

userIO.o: userIO.c
	$(CC) $(CFLAGS) -c userIO.c

shared.o: shared.c
	$(CC) $(CFLAGS) -c shared.c

clean: 
	@echo "removing everything"
	rm a.out client server.o client.o userIO.o serverfinal clientfinal  