
# Client/Server Socket Application

This is a C-based Client/Server socket application. It supports several commands that can be executed from the client's CLI and handled by the server.


## Usage

-   To use this application, clone the repository to your local machine.
-   Open the terminal and navigate to the project directory.
-   Run `make` to build the executable files for both the client and server.
-   Run the server executable by change directory to `server_dir` and typing `./server` in the terminal. Default `<IP_ADDRESS>` is `127.0.0.7` and `<PORT>` is `8080`, but you can change it as you want .
-   Run the client executable in other terminal window, navigate to the project directories `client` directory and typing `./client` in the terminal. To connect the server type `connect` command with `<IP_ADDRESS>` the IP address of the server and `<PORT>` the port number you set for the server.
-   Once connected, use the following commands to interact with the server:
    -   `shell "COMMAND"`.    
    -   `disconnect`.
   

## Contributing

Contributions to this project are welcome! If you find any bugs or have any suggestions for improvements, please feel free to open an issue or submit a pull request.