#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "server.h"
#include "client_handler.h"

#define PORT 8080

int main(void)
{
    int server_fd;
    struct sockaddr_in server_addr;

    // create server socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // config socket
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // bind socket to port
    if (bind(server_fd,
             (struct sockaddr *)&server_addr,
             sizeof(server_addr)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // listen for connections
    if (listen(server_fd, 10) < 0)
    {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

    while (1)
    {
        // client info
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int *client_fd = malloc(sizeof(int));

        // accept client connection
        if ((*client_fd = accept(server_fd,
                                 (struct sockaddr *)&client_addr,
                                 &client_addr_len)) < 0)
        {
            perror("accept failed");
            continue;
        }

        // create a new thread to handle client request
        pthread_t thread_id;
        pthread_create(&thread_id, NULL, handle_client, (void *)client_fd);
        pthread_detach(thread_id);
    }

    close(server_fd);
    return 0;
}
