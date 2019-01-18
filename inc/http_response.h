#ifndef HTTP_RESPONSE
#define HTTP_RESPONSE

#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#include "http_request.h"

#define DATE_LEN 40

struct HTTP_RESPONSE_HEADER {
    int status;
    char date[40];
    char content_type[100];
    char server[256];
    int content_length;
};

struct HTTP_RESPONSE_STRUCT {
    struct HTTP_RESPONSE_HEADER header;
    uint8_t *data;
    uint8_t *header_str;
};

extern void create_response(struct HTTP_REQUEST_STUCT *request, struct HTTP_RESPONSE_STRUCT *response);

#endif