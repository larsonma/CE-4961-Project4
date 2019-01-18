#include "http_response.h"

static void read_data(struct HTTP_REQUEST_STUCT*, struct HTTP_RESPONSE_STRUCT*);
static void set_header_data(struct HTTP_REQUEST_STUCT*, struct HTTP_RESPONSE_STRUCT*);
static void get_message(int, char*);
static void set_content_type(char*, struct HTTP_RESPONSE_HEADER*);
static void set_content_len(char*, struct HTTP_RESPONSE_HEADER*);
static void set_date(struct HTTP_RESPONSE_HEADER*);
static void set_hostname(struct HTTP_RESPONSE_HEADER*);
static void print_header(struct HTTP_RESPONSE_STRUCT* response);

void create_response(struct HTTP_REQUEST_STUCT *request, struct HTTP_RESPONSE_STRUCT *response){
    //Print the header to the full data
    set_header_data(request, response);
    read_data(request, response);
    print_header(response);
}

void read_data(struct HTTP_REQUEST_STUCT *request, struct HTTP_RESPONSE_STRUCT *response){
    FILE *fp = fopen(request->filepath, "rb");

    if(fp){
        response->data = (uint8_t*)malloc((*response).header.content_length + 1);
        fread(response->data, (*response).header.content_length, 1, fp);
        fclose(fp);
    }
}

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

void print_header(struct HTTP_RESPONSE_STRUCT* response){
    char header[1024], message[64];
    get_message((*response).header.status, message);

    strcpy(header, "HTTP/1.1 ");
    strcat(header, message);
    strcat(header, "\n");

    strcat(header, "Date: ");
    strcat(header, (*response).header.date);
    strcat(header, "\n");

    strcat(header, "Server: ");
    strcat(header, (*response).header.server);
    strcat(header, "\n");

    strcat(header, "Content-Type: ");
    strcat(header, (*response).header.content_type);
    strcat(header, "\n");

    char length[10];
    sprintf(length, "%d", (*response).header.content_length);
    strcat(header, "Content-Length: ");
    strcat(header, length);
    strcat(header, "\n");

    strcat(header, "Connection: close\n\n\0");

    response->header_str = (uint8_t*)malloc(strlen(header) + 1);
    strcpy((char*)response->header_str, header);
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

void set_content_type(char* filename, struct HTTP_RESPONSE_HEADER* header){
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

void set_content_len(char* filepath, struct HTTP_RESPONSE_HEADER* header){
    FILE* fp = fopen (filepath,"rb");

    if(fp){
        fseek(fp, 0, SEEK_END);
        header->content_length = ftell(fp);  
        fseek(fp, 0, SEEK_SET);

        fclose(fp);
    }
}

void set_date(struct HTTP_RESPONSE_HEADER* header){
    time_t now = time(NULL);
    struct tm *t = gmtime(&now);

    if(t == NULL){
        perror("Failed to obtain time");
    }

    if(!strftime(header->date, sizeof(header->date)-1, "%a, %d %b %y %T %z", t)){
        perror("Failed to format time and write to header");
    }
}

void set_hostname(struct HTTP_RESPONSE_HEADER* header){
    gethostname(header->server, sizeof(header->server));
}