#include "http_response.h"

#include <string.h>
#include <time.h>

void get_message(int, char*);

void create_header(int status, char* content_type, int content_length, char* header){
    char message[40];
    get_message(status, message);

    strcat(header, "HTTP/1.1 ");
    strcat(header, message);
    strcat(header, "\n");

    strcat(header, "Content-Type: ");
    strcat(header, content_type);
    strcat(header, "\n");

    char text[100];
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(text, sizeof(text)-1, "%d %m %Y %H:%M", t);

    strcat(header, "Date: ");
    strcat(header, text);
    strcat(header, "\n");

    char length[10];
    sprintf(length, "%d", content_length);
    strcat(header, "Content-Length: ");
    strcat(header, length);
    strcat(header, "\n");

    strcat(header, "Connection: close\n\0");
}

void get_message(int status, char * dest) {
    switch(status) {
        case 200:
            strcpy(dest, "200 OK\0");
            break;
        case 404:
            strcpy(dest, "404 NOT FOUND\0");
            break;
        case 405:
            strcpy(dest, "405 METHOD NOT ALLOWED\0");
            break;
        case 418:
            strcpy(dest, "418 I'M A TEAPOT\0");
            break;
        case 500:
        default:
            strcpy(dest, "500 INTERNAL SERVER ERROR\0");
            break;
    }
}