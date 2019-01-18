#ifndef HTTP_REQUEST
#define HTTP_REQUEST

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

#define DATE_LEN 40

struct HTTP_REQUEST_HEADER {
    char HTTP_verb[7+1];
    char url[100];
    char version[10];
};

struct HTTP_REQUEST_STUCT {
    struct HTTP_REQUEST_HEADER header;
    char filepath[110];
};

#endif