#include "pipe_networking.h"


int main() {

    int to_server;
    int from_server;
    from_server = client_handshake( &to_server );

    while (1) {       
        char line[BUFFER_SIZE];
        printf("Input: ");
        fgets(line, BUFFER_SIZE, stdin);
        //line[strlen(line) - 1] = '\0';
        if (!strcmp(line, "exit")) return 0;
        write(to_server, line, BUFFER_SIZE);
        char response[BUFFER_SIZE];
        read(from_server, response, sizeof(response));
        printf("OUTPUT: %s\n", response);
    }
    return 0;
}
