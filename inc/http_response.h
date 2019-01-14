#ifndef HTTP_SERVER
#define HTTP_SERVER

#include <stdio.h>
#include <unistd.h>

extern void create_header(int status, char* content_type, int content_length, char* header);

#endif