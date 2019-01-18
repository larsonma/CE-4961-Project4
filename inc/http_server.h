#ifndef HTTP_SERVER
#define HTTP_SERVER

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "http_response.h"
#include "http_request.h"

extern void serve_client(int client_sock_fd);

#endif