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

struct networkData {
    uint32_t value1;
    char string1[6];
    uint8_t value2;
    uint16_t value3;
} __attribute__((packed));

#define DATAMAX 1000

int main(int argc, char** argv){

    //Define variables needed by application
    int sock;
    struct sockaddr_in server;
	struct hostent *hp;
    struct networkData d1 = {
        .value1 = htonl(8675309),
        .string1 = "Jenny\0",
        .value2 = 241,
        .value3 = htons(57005)
    };

    //Get IP address - Exit with error code if unsuccessful
    if(argc != 3){
        printf("Usage: echoclient <IP Address> <port>\n");
        exit(1);
    }

    //Create a socket - Exit if unsuccessful
    if((sock = socket(PF_INET, SOCK_DGRAM, 0)) < 0){
        perror("Error creating socket");
        exit(1);
    }

    server.sin_family = AF_INET;

    //Get host name and print + exit if there was an error.
    if((hp = gethostbyname(argv[1])) == 0){
        perror("Error getting host");
        exit(1);
    }

    //Copy IP address info into server struct
    memcpy(&server.sin_addr.s_addr, hp->h_addr, hp->h_length);

    unsigned short port;

    //Get port from user - Exit with error code if unsuccessful
    if(sscanf(argv[2], "%hu", &port) != 1){
        perror("Error parsing port");
        exit(1);
    }

    //Add port info to server object
    server.sin_port = htons(port);

    printf("Ready to send to remote server %s at port %hu\n", hp->h_name, port);

    //Send UDP request to server
    int size_echoed;
    int size_sent = sendto(sock, &d1, sizeof(d1), 0, (struct sockaddr*) &server, sizeof(server));

    //If there was an error send data, indicate to user and exit.
    if(size_sent < 0){
        perror("Error Sending data");
        exit(1);
    }

    struct timeval timeout = {5,0};

    fd_set sockedReadSet;
    FD_ZERO(&sockedReadSet);
    FD_SET(sock, &sockedReadSet);

    //Setup socket so that a timeout is associated with it. This will allow the busy wait
    //for receiving data to be exited after reaching a time threshold.
    if(select(sock+1, &sockedReadSet, 0, 0, &timeout) < 0){
        perror("error on select");
        exit(1);
    }

    char rec_data[DATAMAX];
    memset(rec_data, 0, DATAMAX);

    //If data received from the server, print it out to the user. If the request takes
	//too long, timeout and print a message indicating that the server could not send
	//a response.
    if(FD_ISSET(sock, &sockedReadSet)){
        size_echoed = recvfrom(sock, rec_data, DATAMAX, 0, NULL, NULL);

        if(size_echoed < 0){
            perror("Error receiving data");
            exit(1);
        }

        printf("Received message: %s\n", rec_data);
    }else{
        printf("Timeout occurred - Server isn\'t listening, the packet was dropped," 
		    "or it refused to respond.\n");
    }

    //Close socket and release it back to the OS.
    close(sock);

    return 0;
}