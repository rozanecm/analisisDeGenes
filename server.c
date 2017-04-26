#define BUF_SIZE 900
#include "socket.h"
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#define kMaxLengthOfMessageToDeliver 300

void processRecievedMessage(char *msgToProcess, int msgLength,
                            char *messageToDeliver,
                            int maxLengthOfMsgToDeliver) {
    /*cada byte en el msj representa un codon. */
    int acidoAspartico = 0;
    int acidoGlutamico = 1;
    int alanina = 2;
    int arginina = 3;
    int asparagina = 4;
    int cisteina = 5;
    int fenilalanina = 6;
    int glicina = 7;
    int histidina = 8;
    int isoleucina = 9;
    int leucina = 10;
    int lisina = 11;
    int metionina = 12;
    int prolina = 13;
    int serina = 14;
    int tirosina = 15;
    int treonina = 16;
    int triptofano = 17;
    int valina = 18;
    int stopCodon = 19;

    /* En int cantidadesDeAminoacidos[] se guardan las cantidades.
     * La posicion donde se guarda se corresponde con el nro. asignado
     * a cada amin. en las lineas previas
     * */
    int cantidadesDeAminoacidos[20] = {0};

    const char *aminoacidos[20];
    aminoacidos[0] = "Ácido aspártico";
    aminoacidos[1] = "Ácido glutámico";
    aminoacidos[2] = "Alanina";
    aminoacidos[3] = "Arginina";
    aminoacidos[4] = "Asparagina";
    aminoacidos[5] = "Cisteina";
    aminoacidos[6] = "Fenilalanina";
    aminoacidos[7] = "Glicina";
    aminoacidos[8] = "Histidina";
    aminoacidos[9] = "Isoleucina";
    aminoacidos[10] = "Leucina";
    aminoacidos[11] = "Lisina";
    aminoacidos[12] = "Metionina";
    aminoacidos[13] = "Prolina";
    aminoacidos[14] = "Serina";
    aminoacidos[15] = "Tirosina";
    aminoacidos[16] = "Treonina";
    aminoacidos[17] = "Triptófano";
    aminoacidos[18] = "Valina";
    aminoacidos[19] = "StopCodon";

    for (int i = 0; i < msgLength; i++){
        /*se contabilizan los aminoacidos*/
        switch (msgToProcess[i]){
            case 21:    //UUU
            case 23:    //UUC
                cantidadesDeAminoacidos[fenilalanina]++;
                break;
            case 20:    //UUA
            case 22:    //UUG
            case 53:    //CUU
            case 55:
            case 52:
            case 54:
                cantidadesDeAminoacidos[leucina]++;
                break;
            case 5:
            case 7:
            case 4:
                cantidadesDeAminoacidos[isoleucina]++;
                break;
            case 6:
                cantidadesDeAminoacidos[metionina]++;
                break;
            case 37:
            case 39:
            case 36:
            case 38:
                cantidadesDeAminoacidos[valina]++;
                break;
            case 29:
            case 31:
            case 28:
            case 30:
                cantidadesDeAminoacidos[serina]++;
                break;
            case 61:
            case 63:
            case 60:
            case 62:
                cantidadesDeAminoacidos[prolina]++;
                break;
            case 13:
            case 15:
            case 12:
            case 14:
                cantidadesDeAminoacidos[treonina]++;
                break;
            case 45:
            case 47:
            case 44:
            case 46:
                cantidadesDeAminoacidos[alanina]++;
                break;
            case 17:
            case 19:
                cantidadesDeAminoacidos[tirosina]++;
                break;
            case 16:
            case 18:
            case 24:
                cantidadesDeAminoacidos[stopCodon]++;
                break;
            case 49:
            case 51:
                cantidadesDeAminoacidos[histidina]++;
                break;
            case 48:
            case 50:
            case 32:
            case 34:
                cantidadesDeAminoacidos[acidoGlutamico]++;
                break;
            case 1:
            case 3:
                cantidadesDeAminoacidos[asparagina]++;
                break;
            case 0:
            case 2:
                cantidadesDeAminoacidos[lisina]++;
                break;
            case 33:
            case 35:
                cantidadesDeAminoacidos[acidoAspartico]++;
                break;
            case 25:
            case 27:
                cantidadesDeAminoacidos[cisteina]++;
                break;
            case 26:
                cantidadesDeAminoacidos[triptofano]++;
                break;
            case 57:
            case 59:
            case 56:
            case 58:
            case 8:
            case 10:
                cantidadesDeAminoacidos[arginina]++;
                break;
            case 9:
            case 11:
                cantidadesDeAminoacidos[serina]++;
                break;
            case 41:
            case 43:
            case 40:
            case 42:
                cantidadesDeAminoacidos[glicina]++;
                break;
        }
    }

    /*se arma el msj a enviar*/
    int aminoacidoMasFrecuente = 0;
    int segundoAminoacidoMasFrecuente = 0;
    int tercerAminoacidoMasFrecuente = 0;

    char nombreDelAminoacidoMasFrecuente[20] = {'Z'};
    char nombreDelSegundoAminoacidoMasFrecuente[20] = {'Z'};
    char nombreDelTercerAminoacidoMasFrecuente[20] = {'Z'};

    for (int j = 0; j < 19; j++){
        if (cantidadesDeAminoacidos[j] > aminoacidoMasFrecuente){
            tercerAminoacidoMasFrecuente = segundoAminoacidoMasFrecuente;
            snprintf(nombreDelTercerAminoacidoMasFrecuente,
                     sizeof(nombreDelTercerAminoacidoMasFrecuente), "%s",
                    nombreDelSegundoAminoacidoMasFrecuente);

            segundoAminoacidoMasFrecuente = aminoacidoMasFrecuente;
            snprintf(nombreDelSegundoAminoacidoMasFrecuente,
                     sizeof(nombreDelSegundoAminoacidoMasFrecuente), "%s",
                    nombreDelAminoacidoMasFrecuente);

            aminoacidoMasFrecuente = cantidadesDeAminoacidos[j];
            snprintf(nombreDelAminoacidoMasFrecuente,
                     sizeof(nombreDelAminoacidoMasFrecuente),
                     "%s", aminoacidos[j]);
        }else if (cantidadesDeAminoacidos[j] > segundoAminoacidoMasFrecuente){
            tercerAminoacidoMasFrecuente = segundoAminoacidoMasFrecuente;
            snprintf(nombreDelTercerAminoacidoMasFrecuente,
                     sizeof(nombreDelTercerAminoacidoMasFrecuente), "%s",
                    nombreDelSegundoAminoacidoMasFrecuente);

            segundoAminoacidoMasFrecuente = cantidadesDeAminoacidos[j];
            snprintf(nombreDelSegundoAminoacidoMasFrecuente,
                     sizeof(nombreDelSegundoAminoacidoMasFrecuente), "%s",
                    aminoacidos[j]);
        }else if (cantidadesDeAminoacidos[j] > tercerAminoacidoMasFrecuente){
            tercerAminoacidoMasFrecuente = cantidadesDeAminoacidos[j];
            snprintf(nombreDelTercerAminoacidoMasFrecuente,
                     sizeof(nombreDelTercerAminoacidoMasFrecuente), "%s",
                    aminoacidos[j]);
        }
    }
    snprintf(messageToDeliver, maxLengthOfMsgToDeliver,
             "Cantidad de proteínas encontradas: %i\n\n"
                    "Aminoácidos más frecuentes:\n1) "
                    "%s: %i\n2) %s: %i\n3) %s: %i",
            cantidadesDeAminoacidos[19],
            nombreDelAminoacidoMasFrecuente, aminoacidoMasFrecuente,
            nombreDelSegundoAminoacidoMasFrecuente,
            segundoAminoacidoMasFrecuente,
            nombreDelTercerAminoacidoMasFrecuente,
            tercerAminoacidoMasFrecuente);
}

int server(char *argv[]){
    socket_t serverSocket, clientSocket;
    const char *port = argv[2];

    /*buffer donde guardar los datos*/
    char buf[BUF_SIZE];

    socket_create_and_bind(&serverSocket, port);
    socket_listen(&serverSocket, 5);
    socket_accept(&serverSocket, &clientSocket);

    int msglength;
    socket_recv(&serverSocket,  buf, BUF_SIZE, &msglength);

    char mensajeParaCliente[kMaxLengthOfMessageToDeliver];
    processRecievedMessage(buf, msglength, mensajeParaCliente,
                           kMaxLengthOfMessageToDeliver);

    socket_send(&serverSocket, &mensajeParaCliente,
                strlen(mensajeParaCliente) + 1);

    socket_close(&serverSocket);

    return 0;
}
