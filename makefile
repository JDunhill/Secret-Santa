CC:= gcc
CFLAGS:= -c

all: clientfinal serverfinal

clientfinal: client.o	
	@echo "Compiling program"
	$(CC) client.o -o clientfinal

serverfinal: server.o server_linked_list.o
	echo "Compiling program"
	$(CC) server.o server_linked_list.o -o serverfinal

client.o: client.c 
	@echo "Compiling Client" 
	$(CC) $(CFLAGS) client.c

server.o: server.c server_linked_list.c
	@echo "Compiling Server" 
	$(CC) $(CFLAGS) server.c server_linked_list.c

server_linked_list.o: server_linked_list.c
	@echo "Compiling Server" 
	$(CC) $(CFLAGS) server_linked_list.c

clean: 
	@echo "removing everything"
	rm a.out server.o client.o serverfinal clientfinal  server_linked_list.o