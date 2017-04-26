#include <stdio.h>
#include <sys/socket.h>
#include <stdbool.h>

typedef struct {
    int fd;
}socket_t;

void new_tcp_socket(socket_t *sock_fd);

void socket_create_and_connect(socket_t *socket_fd, const char *serverIP,
                               const char *port);

void socket_create_and_bind(socket_t *socket_fd, const char *port);

void socket_connect(socket_t *socket_fd, const struct sockaddr *addr,
                    socklen_t addrlen);

void socket_send(socket_t *socket_fd, const void *messageToSend,
                 size_t messageLength);

void socket_recv(socket_t *socket_fd, char messageToRecieve[],
                 size_t messageToReadLength, int *readMsgLength);

void socket_close(socket_t *socket_fd);

void socket_bind(socket_t *socket_fd, const struct sockaddr *addr,
                 socklen_t addrlen);

void socket_listen(socket_t *socket_fd, int backlog);

void socket_accept(socket_t *socket_fd, socket_t *clientSocket);

void socket_shutdown(socket_t *socket_fd);
