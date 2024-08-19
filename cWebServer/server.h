#ifndef SERVER_H
#define SERVER_H

#include <netinet/in.h>

#define PORT 8080

int create_server_socket();
void configure_server_address(struct sockaddr_in *server_addr);
void bind_socket(int server_fd, struct sockaddr_in *server_addr);
void listen_for_connections(int server_fd);

#endif // SERVER_H
