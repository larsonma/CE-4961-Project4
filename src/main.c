//Name: Mitchell Larson
//Course: CE 4961
//Assignment: Project 3

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

#include "http_server.h"

int main(int argc, char** argv){

    //Define variables needed by application
    int server_sock_fd, client_sock_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len;
    int pid;

    //User should pass port to run HTTP server on
    if(argc != 2){
        printf("Usage: echoclient <IP Address> <port>\n");
        exit(1);
    }

    //Create a socket - Exit if unsuccessful
    if((server_sock_fd = socket(PF_INET, SOCK_STREAM, 0)) < 0){
        perror("Error creating socket");
        exit(1);
    }

    unsigned short port;

    //Get port from user - Exit with error code if unsuccessful
    if(sscanf(argv[1], "%hu", &port) != 1){
        perror("Error parsing port");
        exit(1);
    }

    //Add port info to server object
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);

    //bind to port provided by user
    if(bind(server_sock_fd, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0){
        perror("Error binding to port");
        exit(1);
    }

    //Listen for TCP connection requests. Allowing 5 queued connections at a time
    if(listen(server_sock_fd, 5) < 0){
        perror("Error listening to socket");
        exit(1);
    }

    //accept new connections in a seperate process
    while(1){
        client_addr_len = sizeof(client_addr);
        client_sock_fd = accept(server_sock_fd, (struct sockaddr*) &client_addr, &client_addr_len);

        if(client_sock_fd < 0){
            perror("Error accepting client connection");
        }
        serve_client(client_sock_fd);

        //pid = fork();

        // if(pid < 0){
        //     perror("Error forking process");
        // }else if(pid == 0){
        //     //child process. Close main socket pipe.
        //     close(server_sock_fd);
        //     serve_client(client_sock_fd);
        //     exit(0);
        // }else{
        //     //parent process. Close pipe to child socket.
        //     close(client_sock_fd);
        // }
    }

    return 0;
}