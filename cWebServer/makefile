CC = gcc
CFLAGS = -Wall -Wextra -pthread
OBJS = main.o server.o client_handler.o

webserver: $(OBJS)
	$(CC) $(CFLAGS) -o webserver $(OBJS)

main.o: main.c server.h client_handler.h
	$(CC) $(CFLAGS) -c main.c

server.o: server.c server.h
	$(CC) $(CFLAGS) -c server.c

client_handler.o: client_handler.c client_handler.h
	$(CC) $(CFLAGS) -c client_handler.c

clean:
	rm -f webserver $(OBJS)
