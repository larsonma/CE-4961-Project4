/**
 * @file     - http_response.h
 * @author   - Mitchell Larson (larsonma@msoe.edu)
 * @brief    - Interface and common struct for http response
 * @version  - 0.1
 * @date     - 2019-01-17
 * 
 * Copyright (c) 2019
 * 
 */

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

/**
 * @brief  - Create a HTTP response by setting header data and file data.
 * 
 * @param request - HTTP_REQUEST_STRUCT pointer containing request data.
 * @param response - HTTP_RESPONSE_STRUCT pointer containing parital response data.
 */
extern void create_response(struct HTTP_REQUEST_STUCT *request, struct HTTP_RESPONSE_STRUCT *response);

#endif