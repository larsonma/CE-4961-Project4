#include "http_server.h"
#include "http_response.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* www = "./www";

void serve_client(int client_sock_fd){
    char readbuffer[1000];
    char* source;
    char HTTP_verb[7+1];
    char url[100];
    char version[10];
    char filepath[107];
    char header[2000];
    int content_len = 0;
    FILE* fp = NULL;
    int status;

    header[0] = '\0';


    //Read data into a buffer using low level api
    if(read(client_sock_fd, readbuffer, 1000) < 0){
        perror("Error reading client data");
        exit(1);
    }

    sscanf(readbuffer, "%s %s %s", HTTP_verb, url, version);

    //GET is the only supported command
    if(strcmp(HTTP_verb, "GET") != 0){
        status = 405;
    }else{
        //Check if the file exists
        strcpy(filepath, www);

        fp = fopen (strcat(filepath, url),"r");

        printf("GET %s\n", filepath);

        if (fp == NULL) {
            //File does not exist. Return 404.
            status = 404;
        } else {
            status = 200;
        }
    }

    if(status == 200){
        fseek(fp, 0, SEEK_END);
        content_len = ftell(fp);  
        fseek(fp, 0, SEEK_SET);

        //read file into memory
        source = malloc(sizeof(char) * (content_len + 1));
        size_t newLen = fread(source, sizeof(char), content_len, fp);
        source[newLen++] = '\0';
    }


    //Write Header data to HTTP client
    create_header(status, "text/html", content_len, header);
    printf(header);
    if(write(client_sock_fd, header, strlen(header)))

    //Write data to HTTP client if file found
    if(status == 200){
        write(client_sock_fd, "\n", 1);
        write(client_sock_fd, source, content_len);
        fclose(fp);
        free(source);
    }

    close(client_sock_fd);
}