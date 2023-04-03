#include "client.h"

int main() {
    int client_socket;
    struct sockaddr_in server_address;
    char *response;
    char command[BUFFER_SIZE] = {0};
    int command_len;
    int connected_flag = 0;

    // loop to send multiple commands to server
    while (1) {
        printf("Client> ");
        fgets(command, BUFFER_SIZE, stdin);
        command_len = strlen(command);
        if (command_len > 0 && command[command_len - 1] == '\n') {
            command[command_len - 1] = '\0'; // remove trailing newline
        }

        // Split the input into words
        char buf[BUFFER_SIZE];
        strcpy(buf,command);
        char *token = strtok(buf, " \n");

        if (token == NULL) {
            continue;
        }

        // check command
        if (!connected_flag && strcmp(token, "connect") == 0){
            // take ip address and port
            char* ip_address = strtok(NULL, " \n");
            char* port_str = strtok(NULL, " \n");
            
            //check if they exist
            if (ip_address == NULL || port_str == NULL) {
                printf("Insert correct ip address and port\n");
                continue;
            }

            // create socket
            client_socket = create_client_socket();

            // set server address
            set_server_address(&server_address, ip_address, port_str);

            // connect to server
            connect_to_server(&server_address, client_socket);

            // set flag 1, to check if client already connected
            connected_flag = 1;
            printf("Connected to server\n");
        }
        else if (connected_flag && strcmp(token, "disconnect") == 0){
            connected_flag = 0;
            // send command to server
            send(client_socket, command, strlen(command), 0);

            // receive response from server
            response = receive_response_from_server(client_socket);
            printf("%s", response);
            free(response);
        }
        else if (connected_flag && strcmp(token, "connect") == 0){
            printf("You are already connected\n");
        }
        else if (connected_flag) {
            // send command to server
            send(client_socket, command, strlen(command), 0);

            // receive response from server
            response = receive_response_from_server(client_socket);
            printf("%s", response);
            free(response);
        }
        else {
            printf("Connect to the server using: connect <IP_ADDRESS> <PORT>\n");
        }
    }

    // close socket and exit
    close(client_socket);
    return 0;
}