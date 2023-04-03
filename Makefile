CC = gcc
CFLAGS = -Wall -Wextra
I = -I./includes
SRC_S = server/server.c server/server_utils.c
SRC_C = client/client.c client/client_utils.c
HEADER_S = includes/server.h
HEADER_C = includes/client.h

all: server/server client/client

server/server: $(SRC_S) $(HEADER_S)
	$(CC) $(CFLAGS) $(I) -o server/server $(SRC_S)

client/client: $(SRC_C) $(HEADER_C)
	$(CC) $(CFLAGS) $(I) -o client/client $(SRC_C)

clean:
	rm -f server/server client/client
