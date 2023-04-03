#include "server.h"

int create_server_socket() {
    int server_fd;
    // create TCP socket (AF_INET, SOCK_STREAM)
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    return server_fd;
}

void bind_server_socket(int server_fd) {
    struct sockaddr_in address; 
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
}

void listen_for_clients(int server_fd) {
    if (listen(server_fd, MAX_CLIENTS) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }
}

int accept_client_connection(int server_fd) {
    int client_fd;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    client_fd = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
    if (client_fd == -1) {
        perror("accept failed");
        exit(EXIT_FAILURE);
    }
    return client_fd;
}

char *receive_command_from_client(int client_fd) {
    char buffer[BUFFER_SIZE] = {0};
    if (recv(client_fd, buffer, BUFFER_SIZE, 0) == -1) {
        perror("recv failed");
        exit(EXIT_FAILURE);
    }
    return strdup(buffer);
}


bool is_valid_shell_command(const char* command) {
    char* cmd = strdup(command); // Make a copy of the command string to avoid modifying the original
    char* saveptr;
    char* token = strtok_r(cmd, " ", &saveptr);
    if (!token) {
        free(cmd);
        return false; // The command string is empty
    }
    char* args[256];
    args[0] = token;
    int i = 1;
    while ((token = strtok_r(NULL, " ", &saveptr))) {
        args[i++] = token;
        if (i == 256) {
            free(cmd);
            return false; // The command has too many arguments
        }
    }
    args[i] = NULL; // Make sure the argument list is terminated with a NULL pointer
    int pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        execvp(args[0], args);
        perror("execvp");
        exit(EXIT_FAILURE);
    } else {
        int status;
        waitpid(pid, &status, 0);
        free(cmd);
        return WIFEXITED(status) && WEXITSTATUS(status) == 0;
    }
}

void process_command_from_client(int client_fd, char *command) {
    char *response;
    while (1) {
        if (strncmp(command, "disconnect", 10) == 0) {
            response = "Disconnecting from server...\n";
            send(client_fd, response, strlen(response), 0);
            close(client_fd);
            return;
        } else if (strncmp(command, "shell", 5) == 0) {
            char *command_to_execute = command + 6; // skip over "shell "
            if (!is_valid_shell_command(command_to_execute)) {
                response = "Unknown command\n";
                send(client_fd, response, strlen(response), 0);
            }
            FILE *fp;
            char output[BUFFER_SIZE] = {0};
            fp = popen(command_to_execute, "r");
            if (fp == NULL) {
                response = "Failed to execute command\n";
                send(client_fd, response, strlen(response), 0);
            } else {
                size_t bytes_read;
                while ((bytes_read = fread(output, 1, BUFFER_SIZE, fp)) > 0) {
                    send(client_fd, output, bytes_read, 0);
                }
                pclose(fp);
            }
        } else {
            response = "Unknown command\n";
            send(client_fd, response, strlen(response), 0);
        }

        // Wait for the next command from the client
        char buffer[BUFFER_SIZE] = {0};
        ssize_t bytes_received = recv(client_fd, buffer, BUFFER_SIZE, 0);
        if (bytes_received <= 0) {
            // Error or connection closed by client
            break;
        } else {
            // Process the received command
            buffer[bytes_received] = '\0'; // remove trailing newline
            process_command_from_client(client_fd, buffer);
        }
    }
}