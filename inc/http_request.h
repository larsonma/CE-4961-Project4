/**
 * @file    - http_request.h
 * @author  - Mitchell Larson (larsonma@msoe.edu)
 * @brief   - Common struct for http request data.
 * @version - 0.1
 * @date    - 2019-01-17
 * 
 * Copyright (c) 2019
 * 
 */

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