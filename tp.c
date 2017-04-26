#include "client.h"
#include "server.h"
#include <stdio.h>
#include <string.h>
int main(int argc, char *argv[]){
    const char *programaInvocado = argv[1];
    if (strcmp(programaInvocado, "client") == 0){
        client(argv);
    }else if (strcmp(programaInvocado, "server") == 0){
        server(argv);
    }
    return 0;
}
