/**
 * @file    - http_server.h
 * @author  - Mitchell Larson (larsonma@msoe.edu)
 * @brief   - Provides interface for an HTTP server.
 * @version - 0.1
 * @date    - 2019-01-17
 * 
 * Copyright (c) 2019
 * 
 */

#ifndef HTTP_SERVER
#define HTTP_SERVER

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "http_response.h"
#include "http_request.h"

/**
 * @brief - Serves an HTTP client by reading and writing to a TCP socket
 * 
 * @param client_sock_fd - TCP socket file descriptor
 */
extern void serve_client(int client_sock_fd);

#endif