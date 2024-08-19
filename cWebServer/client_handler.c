#include "client_handler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024
#define WWW_ROOT "./www"

void send_file(int client_fd, const char *filename) {
    char filepath[256];
    snprintf(filepath, sizeof(filepath), "%s%s", WWW_ROOT, filename);

    int fd = open(filepath, O_RDONLY);
    if (fd == -1) {
        // File not found, send 404
        char *response = "HTTP/1.1 404 Not Found\nContent-Type: text/plain\n\nFile Not Found";
        write(client_fd, response, strlen(response));
        return;
    }

    // File found, send 200 OK
    char *header = "HTTP/1.1 200 OK\nContent-Type: text/html\n\n";
    write(client_fd, header, strlen(header));

    // Send file content
    char buffer[BUFFER_SIZE];
    ssize_t bytes_read;
    while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0) {
        write(client_fd, buffer, bytes_read);
    }

    close(fd);
}

void *handle_client(void *client_socket) {
    int client_fd = *(int*)client_socket;
    char buffer[BUFFER_SIZE] = {0};
    
    // Read client request
    read(client_fd, buffer, BUFFER_SIZE);
    printf("Received request: %s\n", buffer);

    // Parse the request to get the filename
    char *filename = strtok(buffer, " ");
    filename = strtok(NULL, " "); // Get the second token (the path)

    if (filename == NULL || strcmp(filename, "/") == 0) {
        filename = "/index.html";
    }

    send_file(client_fd, filename);

    close(client_fd);
    free(client_socket);
    return NULL;
}
