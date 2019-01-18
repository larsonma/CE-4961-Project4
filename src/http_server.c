/**
 * @file    - http_server.c
 * @author  - Mitchell Larson (larsonma@msoe.edu)
 * @brief   - Serves an HTTP client by interperting request and fetching the
 *            appropriate response. 
 * @version - 0.1
 * @date    - 2019-01-17
 * 
 * Copyright (c) 2019
 * 
 */

#include "http_server.h"

//webroot
static char root[6] = "./www";

static void reroute(struct HTTP_REQUEST_STUCT*, int);

/**
 * @brief - Serves an HTTP client by reading and writing to a TCP socket
 * 
 * @param client_sock_fd - TCP socket file descriptor
 */
void serve_client(int client_sock_fd){   
    struct HTTP_REQUEST_STUCT request = {};
    struct HTTP_RESPONSE_STRUCT response = {};
    FILE *inStream;

    //Open the file descriptor to use higher-level file I/O
    if(!(inStream = fdopen(client_sock_fd, "r"))){
        printf("Error opening TCP input stream\n");
    };

    //Parse the HTTP request parameters
    fscanf(inStream, "%s %s %s", request.header.HTTP_verb, request.header.url, request.header.version);

    //GET is the only supported command. Set error is not GET
    if(strcmp(request.header.HTTP_verb, "GET") != 0){
        response.header.status = 405;
    }else{
        strcpy(request.filepath, root);
        FILE *fp = fopen (strcat(request.filepath, request.header.url),"rb");

        printf("GET %s\n", request.filepath);

        //Check if the file exists and set status based on presence of file.
        //Bonus: set status to 418 if client attempts to brew coffee.
        if (fp == NULL && strcmp(request.header.url, "/brew/coffee") == 0) {
            response.header.status = 418;
        } else if(fp == NULL) {
            response.header.status = 404;
        }else {
            response.header.status = 200;
            fclose(fp);
        }
    }

    //If the file was not found, reroute so corresponding error file is returned.
    if(response.header.status != 200){
        reroute(&request, response.header.status);
    }

    //Create the HTTP response based on parsed/derived parameters.
    create_response(&request, &response);

    //Write the header to the client, followed by the body.
    write(client_sock_fd, response.header_str, strlen((char*)response.header_str));
    write(client_sock_fd, response.data, response.header.content_length);

    //Free dynamic data generated when creating HTTP response.
    free(response.header_str);
    free(response.data);

    //Close the connection.
    close(client_sock_fd);
}

/**
 * @brief - This function reroutes an HTTP request so that the proper
 *          HTML error page is displayed when an error occurs serving
 *          the HTTP request.
 * 
 * @param request 
 * @param status 
 */
void reroute(struct HTTP_REQUEST_STUCT *request, int status){
    char newPath[16];
    strcpy(newPath, root);

    switch(status){
        case 404:
            strcpy(request->filepath, strcat(newPath, "/404.html\0"));
            break;
        case 405:
            strcpy(request->filepath, strcat(newPath, "/405.html\0"));
            break;
        case 418:
            strcpy(request->filepath, strcat(newPath, "/418.html\0"));
            break;
        case 500:
        default:
            strcpy(request->filepath, strcat(newPath, "/418.html\0"));
            break;
    }
}