#include "server.h"

int main() {
    int server_fd, client_fd;
    char *command;
    // create server socket and copy file descriptor into server_fd
    server_fd = create_server_socket();
    // bind socket to address
    bind_server_socket(server_fd);
    // start listening
    listen_for_clients(server_fd);
    while (1) {
        client_fd = accept_client_connection(server_fd);
        command = receive_command_from_client(client_fd);
        process_command_from_client(client_fd, command);
        free(command);
    }
    close(server_fd);
    return 0;
}
