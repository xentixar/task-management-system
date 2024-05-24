#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

void handle_root(int client_socket, int buffer_size) {
    const char *response_template =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: application/json\r\n"
        "Content-Length: %d\r\n"
        "\r\n"
        "%s";

    const char *json_data = "{\"message\": \"Hello, World!\", \"description\": \"This is a simple HTTP server.\"}";
    char response[buffer_size];

    int response_length = snprintf(response, buffer_size, response_template, strlen(json_data), json_data);

    write(client_socket, response, strlen(response));
}