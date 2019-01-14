#ifndef HTTP_SERVER
#define HTTP_SERVER

#include <stdio.h>
#include <unistd.h>

extern void serve_client(int client_sock_fd);

#endif