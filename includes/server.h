#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <netinet/in.h> 
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 5

char    *receive_command_from_client(int client_fd);
bool    is_valid_shell_command(const char* command);
void    bind_server_socket(int server_fd);
void    process_command_from_client(int client_fd, char *command);
void    listen_for_clients(int server_fd);
int     create_server_socket();
int     accept_client_connection(int server_fd);

#endif // SERVER_H