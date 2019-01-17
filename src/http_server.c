#include "http_server.h"
#include "http_response.h"
#include "http_request.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char root[6] = "./www";

void read_data(struct HTTP_REQUEST_STUCT*, struct HTTP_RESPONSE_STRUCT*);
void reroute(struct HTTP_REQUEST_STUCT*, int);

void serve_client(int client_sock_fd){   
    struct HTTP_REQUEST_STUCT request = {};
    struct HTTP_RESPONSE_STRUCT response = {};
    FILE *inStream;

    if(!(inStream = fdopen(client_sock_fd, "r"))){
        printf("Error opening TCP input stream\n");
    };

    fscanf(inStream, "%s %s %s", request.header.HTTP_verb, request.header.url, request.header.version);

    //GET is the only supported command
    if(strcmp(request.header.HTTP_verb, "GET") != 0){
        response.header.status = 405;
    }else{
        //Check if the file exists
        strcpy(request.filepath, root);

        FILE *fp = fopen (strcat(request.filepath, request.header.url),"rb");

        printf("GET %s\n", request.filepath);

        if (fp == NULL && strcmp(request.header.url, "/brew/coffee") == 0) {
            response.header.status = 418;
        } else if(fp == NULL) {
            response.header.status = 404;
        }else {
            response.header.status = 200;
            fclose(fp);
        }
    }

    if(response.header.status != 200){
        reroute(&request, response.header.status);
    }

    set_header_data(&request, &response);
    read_data(&request, &response);
    create_response(&response);

    write(client_sock_fd, response.header_str, response.header.hlen);
    write(client_sock_fd, response.data, response.header.content_length);

    free(response.header_str);
    free(response.data);

    // close(client_sock_fd);
}

void read_data(struct HTTP_REQUEST_STUCT *request, struct HTTP_RESPONSE_STRUCT *response){
    FILE *fp = fopen(request->filepath, "rb");

    if(fp){
        response->data = (uint8_t*)malloc((*response).header.content_length + 1);
        fread(response->data, (*response).header.content_length, 1, fp);
        fclose(fp);
    }
}

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