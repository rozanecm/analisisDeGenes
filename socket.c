#define _POSIX_C_SOURCE 200112L
#include <sys/socket.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <printf.h>
#include <unistd.h>
#include <stdio.h>
#include <netdb.h>
#include "socket.h"

void socket_create_and_connect(socket_t *socket_fd, const char *serverIP,
                               const char *port){
    const char * node = serverIP;
    const char * service = port;
    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    struct addrinfo *result, *rp;

    int g = getaddrinfo(node, service, &hints, &result);
    /*Error check*/
    if (g != 0){
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(g));
    }else{
        for (rp = result; rp != NULL; rp = rp->ai_next) {
            socket_fd->fd = socket(rp->ai_family, rp->ai_socktype,
                                   rp->ai_protocol);
            /* ERROR CHECKING
             * According to documentation: "On success, a file descriptor
             * for the new socket is returned.  On error, -1 is returned,
             * and errno is set appropriately."
             * */
            if (socket_fd->fd == -1){
                continue;
            }
            if (connect(socket_fd->fd, rp->ai_addr, rp->ai_addrlen) != -1){
                break;
            }
            /*if everything fails...*/
            close(socket_fd->fd);
            printf("Error en la creacion/conexion del socket.\n");
        }
    }
    freeaddrinfo(result);
}

void socket_create_and_bind(socket_t *socket_fd, const char *port){
    const char * service = port;
    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    struct addrinfo *result, *rp;

    int g = getaddrinfo(NULL, service, &hints, &result);
    /*Error check*/
    if (g != 0){
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(g));
    }else{
        for (rp = result; rp != NULL; rp = rp->ai_next) {
            socket_fd->fd = socket(rp->ai_family, rp->ai_socktype,
                                   rp->ai_protocol);
            /* ERROR CHECKING
             * According to documentation: "On success, a file descriptor
             * for the new socket is returned.  On error, -1 is returned,
             * and errno is set appropriately."
             * */
            if (socket_fd->fd == -1){
                continue;
            }
            if (bind(socket_fd->fd, rp->ai_addr, rp->ai_addrlen) != -1){
                break;
            }
            /*if everything fails...*/
            close(socket_fd->fd);
            printf("Error with socket creation/binding.\n");
        }
    }
    freeaddrinfo(result);
}

void socket_send(socket_t *socket_fd, const void *messageToSend,
                 size_t messageLength){
    ssize_t totalSentAmount = 0;
    ssize_t lastSentAmount = send(socket_fd->fd, messageToSend, messageLength,
                                  MSG_NOSIGNAL);
    while (totalSentAmount < messageLength){
        if (lastSentAmount == -1){
            int error = errno;
            printf("Error en el recieve: %s\n", strerror(error));
            break;
        } else {
            totalSentAmount += lastSentAmount;
            lastSentAmount = send(socket_fd->fd,
                                  &messageToSend + totalSentAmount,
                                  messageLength-totalSentAmount, MSG_NOSIGNAL);
        }
    }
}

void socket_recv(socket_t *socket_fd, char messageToRecieve[],
                 size_t messageToReadLength, int *readMsgLength) {
    bool socketShutDown = false;
    ssize_t totalRecievedAmount = 0;
    ssize_t lastRecievedAmount = 0;
    while (totalRecievedAmount < messageToReadLength && !socketShutDown){
        /*aka until socket is not shut down*/
        lastRecievedAmount = recv(socket_fd->fd,
                                  &messageToRecieve[totalRecievedAmount],
                                  messageToReadLength-totalRecievedAmount,
                                  MSG_NOSIGNAL);
        if (lastRecievedAmount == -1){
            int error = errno;
            printf("Error en el recieve: %s\n", strerror(error));
            break;
        } else if (lastRecievedAmount == 0) {    //socket was shut down
            socketShutDown = true;
        } else {
            totalRecievedAmount += lastRecievedAmount;
        }
    }

    *readMsgLength = (int) totalRecievedAmount;
}

void socket_close(socket_t *socket_fd){
    int ret = close(socket_fd->fd);

    /*VERIFICACION DE ERRORES*/
    if (ret == -1){
        int error = errno;
        printf("Error en el cierre del socket: %s\n", strerror(error));
    }
}

void socket_bind(socket_t *socket_fd, const struct sockaddr *addr,
                 socklen_t addrlen){
    int ret = bind(socket_fd->fd, addr, addrlen);

    /*VERIFICACION DE ERRORES*/
    if (ret == -1){
        int error = errno;
        printf("Error with socket binding: %s\n", strerror(error));
    }
}

void socket_listen(socket_t *socket_fd, int backlog){
    /*The socket_fd argument is a file descriptor that refers to a socket of
     * type SOCK_STREAM or SOCK_SEQPACKET. The backlog argument defines the
     * maximum length to which the queue of pending connections for socket_fd
     * may grow. If a connection request arrives when the queue is full, the
     * client may receive an error with an indication of ECONNREFUSED or, if
     * the underlying protocol supports retransmission, the request may be
     * ignored so that a later reattempt at connection succeeds.
     * */
    int ret = listen(socket_fd->fd, backlog);

    /*VERIFICACION DE ERRORES*/
    if (ret == -1){
        int error = errno;
        printf("Error with socket listen: %s\n", strerror(error));
    }
}

void socket_accept(socket_t *socket_fd, socket_t *clientSocket){
    socklen_t cliLength = sizeof(clientSocket);
    int ret = accept(socket_fd->fd, (struct sockaddr *)&clientSocket,
                     &cliLength);

    /*VERIFICACION DE ERRORES*/
    if (ret == -1){
        int error = errno;
        printf("Error with socket accept: %s\n", strerror(error));
    }
        socket_fd->fd = ret;
}

void socket_shutdown(socket_t *socket_fd){
    /* se cierra el socket para escritura. Lo usa el cliente al terminar de
     * enviar al servidor. De esta forma queda
     * abiero para poder recibir el msj del srvidor
     * */
    int ret = shutdown(socket_fd->fd, SHUT_WR);

    /*VERIFICACION DE ERRORES*/
    if (ret == -1){
        int error = errno;
        printf("Error en shutdown del socket: %s\n", strerror(error));
    }
}
