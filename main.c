#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#include "src/apis/root.c"
#include "src/apis/tasks.c"


#define PORT 8080
#define BUFFER_SIZE 1024
#define LOG_FILE "logs/http_requests.log"

void error(const char *msg) {
    perror(msg);
    exit(1);
}

void write_to_log(const char *message) {
    FILE *file = fopen(LOG_FILE, "a");
    if (file == NULL) {
        perror("Error opening log file");
        return;
    }
    fprintf(file, "%s\n\n", message);
    fclose(file);
}

void handle_request(int client_socket) {
    char buffer[BUFFER_SIZE];
    read(client_socket, buffer, BUFFER_SIZE);
    write_to_log(buffer);

    if (strncmp(buffer, "GET / ", 6) == 0) {
        handle_root(client_socket, BUFFER_SIZE);
    } else if (strncmp(buffer, "GET /tasks ", 11) == 0) {
        list_tasks(client_socket, BUFFER_SIZE);
    }
}

int main() {
    int server_fd, client_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        error("Socket failed");
    }

    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        error("setsockopt failed");
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        error("Bind failed");
    }

    if (listen(server_fd, 3) < 0) {
        error("Listen failed");
    }

    printf("Server listening on port %d\n", PORT);

    while (1) {
        if ((client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            error("Accept failed");
        }
        handle_request(client_socket);
        close(client_socket);
    }
}