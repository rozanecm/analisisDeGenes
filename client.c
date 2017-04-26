#define BUF_SIZE 900

#include <netinet/in.h>
#include "socket.h"
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>

char procesarCodon(char *codon) {
    int p = 0;
    for (int i = 0; i < 3; i++){
        p = p << 2;
        switch (codon[i]){
            case 'A':
                break;
            case 'U':
                p += 1;
                break;
            case 'G':
                p += 2;
                break;
            case 'C':
                p += 3;
                break;
            default:break;
        }
    }
    return (char) p;
}

void encode(char* originalMessage, size_t strlength, char *messageToSend){
    char codon[3];
    int codonesProcesados = 0;
    for (int i = 0; i < strlength; i++){
        codon[i%3] = originalMessage[i];
        if (i%3 == 2){
            /*tenemos un codon entero guardado en codon[]*/
            messageToSend[codonesProcesados] = procesarCodon(codon);
            codonesProcesados++;
        }
    }
}

int client(char *argv[]){
    const char *serverIP = argv[2];
    const char *serverPort = argv[3];
    socket_t socket_fd = {.fd = 0};
    const char *rutaDeArchivo = argv[4];
    /* manejo de archivo */
    FILE *archivo;
    archivo = fopen(rutaDeArchivo, "r");
    if (archivo == NULL){
        printf("Error opening file: %s\n", strerror(errno));
    }
    /*Creacion y conexion de Socket*/
    socket_create_and_connect(&socket_fd, serverIP, serverPort);

    /*lectura de archivo*/
#define kNumOfCharsToRead 18 //se leera de a  6 codones por vez
    char readChars[kNumOfCharsToRead] = "a";
    char messageToSend[BUF_SIZE] = "a";
    memset(messageToSend, 0, BUF_SIZE);
    int lastReadAmount = 0;
    while ((lastReadAmount = fread(readChars, sizeof *readChars,
                                  kNumOfCharsToRead, archivo)) != 0){
        encode(readChars, kNumOfCharsToRead, messageToSend);
        memset(readChars, 0, kNumOfCharsToRead);
        int lengthOfMessageToSend = lastReadAmount / 3;
        socket_send(&socket_fd, &messageToSend, lengthOfMessageToSend);
    }
    socket_shutdown(&socket_fd);

    char msgFromServer[300];
    int recievedMsgLength;
    socket_recv(&socket_fd, msgFromServer, 300, &recievedMsgLength);
    printf("%s\n", msgFromServer);

    /*cierre de archivo*/
    if (fclose(archivo) != 0){
        printf("Error closing file: %s\n", strerror(errno));
    }

    /*cierre de socket*/
    socket_close(&socket_fd);
    return 0;
}
