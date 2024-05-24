#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "../controllers/tasks.c"

void list_tasks(int client_socket, int buffer_size) {
    const char *response_template =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: %d\r\n"
        "\r\n"
        "%s";

    char *json_str = malloc(MAX_ROWS * (sizeof(int) + 100 + 50));
    fetch_all(json_str);
    
    char response[buffer_size];

    int response_length = snprintf(response, buffer_size, response_template, strlen(json_str), json_str);

    write(client_socket, response, strlen(response));
}