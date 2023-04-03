#ifndef CLIENT_H
#define CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// define custom size for buffers
#define BUFFER_SIZE 1024

char    *receive_response_from_server(int client_socket);
void    set_server_address(struct sockaddr_in *server_address, char *ip_address, char *port_str);
void    connect_to_server(struct sockaddr_in *server_address, int client_socket);
int     create_client_socket();

#endif //CLIENT_H