#include "client.h"

int create_client_socket() {
    int client_fd;
    // create TCP socket (AF_INET, SOCK_STREAM)
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd == -1) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    return client_fd;
}

void set_server_address(struct sockaddr_in *server_address, char *ip_address, char *port_str) {
    server_address->sin_family = AF_INET;
    server_address->sin_port = htons(atoi(port_str));
    if (inet_pton(AF_INET, ip_address, (void *)&server_address->sin_addr) <= 0) {
        perror("invalid address");
        exit(EXIT_FAILURE);
    }
}

void connect_to_server(struct sockaddr_in *server_address, int client_socket) {
    if (connect(client_socket, (struct sockaddr *)server_address, sizeof(*server_address)) == -1) {
        perror("connection failed");
        exit(EXIT_FAILURE);
    }
}

char *receive_response_from_server(int client_socket) {
    char buffer[BUFFER_SIZE] = {0};
    if (recv(client_socket, buffer, BUFFER_SIZE, 0) == -1) {
        perror("recv failed");
        exit(EXIT_FAILURE);
    }
    return strdup(buffer);
}