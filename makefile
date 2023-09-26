CC:= gcc
CFLAGS:= 

all:  clientfinal serverfinal

clientfinal: client.o userIO.o shared.o
	@echo "Compiling program"
	$(CC) client.o userIO.o shared.o -o clientfinal

serverfinal: server.o server_linked_list.o
	echo "Compiling program"
	$(CC) server.o server_linked_list.o -o serverfinal


client.o:  client.c userIO.c shared.c
	@echo "Compiling Client" 
	$(CC) $(CFLAGS) -c client.c userIO.c shared.c

userIO.o: userIO.c
	$(CC) $(CFLAGS) -c userIO.c

shared.o: shared.c
	$(CC) $(CFLAGS) -c shared.c

server.o: server.c server_linked_list.c
	@echo "Compiling Server" 
	$(CC) $(CFLAGS) -c server.c server_linked_list.c

server_linked_list.o: server_linked_list.c
	@echo "Compiling Server" 
	$(CC) $(CFLAGS) -c server_linked_list.c

clean: 
	@echo "removing everything"
	rm a.out server.o client.o serverfinal clientfinal  server_linked_list.o shared.o userIO.o