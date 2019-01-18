/**
 * @file     - http_response.c
 * @author   - Mitchell Larson (larsonma@msoe.edu)
 * @brief    - Parses HTTP request and response structs to generate a valid
 *             HTTP response.
 * @version  - 0.1
 * @date     - 2019-01-17
 * 
 * Copyright (c) 2019
 * 
 */

#include "http_response.h"

static void read_data(struct HTTP_REQUEST_STUCT*, struct HTTP_RESPONSE_STRUCT*);
static void set_header_data(struct HTTP_REQUEST_STUCT*, struct HTTP_RESPONSE_STRUCT*);
static void get_message(int, char*);
static void set_content_type(char*, struct HTTP_RESPONSE_HEADER*);
static void set_content_len(char*, struct HTTP_RESPONSE_HEADER*);
static void set_date(struct HTTP_RESPONSE_HEADER*);
static void set_hostname(struct HTTP_RESPONSE_HEADER*);
static void print_header(struct HTTP_RESPONSE_STRUCT* response);

/**
 * @brief  - Create a HTTP response by setting header data and file data.
 * 
 * @param request - HTTP_REQUEST_STRUCT pointer containing request data.
 * @param response - HTTP_RESPONSE_STRUCT pointer containing parital response data.
 */
void create_response(struct HTTP_REQUEST_STUCT *request, struct HTTP_RESPONSE_STRUCT *response){
    set_header_data(request, response);
    read_data(request, response);
    print_header(response);
}

/**
 * @brief - Open the file based on the filename in the request struct and copy the
 *          file data to the response struct.
 * 
 * @param request - HTTP_REQUEST_STRUCT pointer containing filepath for requested resource.
 * @param response - HTTP_RESPONSE_STRUCT pointer where file data should be copied.
 */
void read_data(struct HTTP_REQUEST_STUCT *request, struct HTTP_RESPONSE_STRUCT *response){
    //Open the requested file in read-binary mode.
    FILE *fp = fopen(request->filepath, "rb");

    //Allocate dyamic memory based on conetent-length and copy file data.
    if(fp){
        response->data = (uint8_t*)malloc((*response).header.content_length + 1);
        fread(response->data, (*response).header.content_length, 1, fp);
        fclose(fp);
    }
}

/**
 * @brief - Set the header data struct with the date, server name, content-type and
 *          content-length
 * 
 * @param request - HTTP_REQUEST_STRUCT pointer containing filepath for requested resource.
 * @param response - HTTP_RESPONSE_STRUCT pointer where header data should be set.
 */
void set_header_data(struct HTTP_REQUEST_STUCT *request, struct HTTP_RESPONSE_STRUCT *response){
    //Set the date that the file was accessed.
    set_date(&(response->header));

    //Set the server name
    set_hostname(&(response->header));

    //Set the content type of the response based on the filename extension
    set_content_type(request->filepath, &(response->header));

    //Set the content length based on the length of the file
    set_content_len(request->filepath, &(response->header));
}

/**
 * @brief - Converts the response header struct into a string.
 * 
 * @param response - HTTP_RESPONSE_STRUCT pointer containing header data.
 */
void print_header(struct HTTP_RESPONSE_STRUCT* response){
    char header[1024], message[64];
    get_message((*response).header.status, message);

    //print HTTP version and response code/message
    strcpy(header, "HTTP/1.1 ");
    strcat(header, message);
    strcat(header, "\n");

    //print date data
    strcat(header, "Date: ");
    strcat(header, (*response).header.date);
    strcat(header, "\n");

    //print server name
    strcat(header, "Server: ");
    strcat(header, (*response).header.server);
    strcat(header, "\n");

    //print requested file's content type
    strcat(header, "Content-Type: ");
    strcat(header, (*response).header.content_type);
    strcat(header, "\n");

    //print requested file's length
    char length[10];
    sprintf(length, "%d", (*response).header.content_length);
    strcat(header, "Content-Length: ");
    strcat(header, length);
    strcat(header, "\n");

    //print connection close
    strcat(header, "Connection: close\n\n\0");

    //copy assembed string to response struct
    response->header_str = (uint8_t*)malloc(strlen(header) + 1);
    strcpy((char*)response->header_str, header);
}

/**
 * @brief - convert an int status into a string status message.
 * 
 * @param status - int status code for response
 * @param dest - string to copy message to
 */
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

/**
 * @brief  - sets the content type header data by parsing the file extension
 *           for the requested document.
 * 
 * @param filename 
 * @param header 
 */
void set_content_type(char* filename, struct HTTP_RESPONSE_HEADER* header){
    //Get substring after the final '.' character
    char *extension = strrchr(filename, '.');

    if(strcmp(extension, ".html") == 0){
        strcpy((*header).content_type, "text/html");
    }else if(strcmp(extension, ".ico") == 0){
        strcpy((*header).content_type, "image/x-icon");
    }else if(strcmp(extension, ".jpg") == 0){
        strcpy((*header).content_type, "image/jpeg");
    }else if(strcmp(extension, ".txt") == 0){
        strcpy((*header).content_type, "text/plain");
    }else if(strcmp(extension, ".png") == 0){
        strcpy((*header).content_type, "image/png");
    }
}

/**
 * @brief - Set the content length field in the response header struct
 * 
 * @param filepath - path of requested file
 * @param header - header struct contained in http response struct.
 */
void set_content_len(char* filepath, struct HTTP_RESPONSE_HEADER* header){
    //Open file in read-binary mode
    FILE* fp = fopen (filepath,"rb");

    if(fp){
        //Get length of using file seeks
        fseek(fp, 0, SEEK_END);
        header->content_length = ftell(fp);  
        fseek(fp, 0, SEEK_SET);

        fclose(fp);
    }
}

/**
 * @brief - set the date field in the http response header struct
 *          by retrieving the current date using the time library.
 * 
 * @param header - header struct contained in http response struct.
 */
void set_date(struct HTTP_RESPONSE_HEADER* header){
    time_t now = time(NULL);

    //Convert time to UTC format.
    struct tm *t = gmtime(&now);

    if(t == NULL){
        perror("Failed to obtain time");
    }

    if(!strftime(header->date, sizeof(header->date)-1, "%a, %d %b %y %T %z", t)){
        perror("Failed to format time and write to header");
    }
}

/**
 * @brief - set the server field in the http_response header struct to the
 *          hostname of this computer.
 * 
 * @param header - header struct contained in http response struct.
 */
void set_hostname(struct HTTP_RESPONSE_HEADER* header){
    gethostname(header->server, sizeof(header->server));
}