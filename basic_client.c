#include "pipe_networking.h"


int main() {

    int to_server;
    int from_server;
    from_server = client_handshake( &to_server );
    char line[BUFFER_SIZE];

    while (1) {       
        printf("Input: ");
        fgets(line, BUFFER_SIZE, stdin);
        if (!strcmp(line, "exit")) return 0;
        write(to_server, line, sizeof(line));
        read(from_server, line, sizeof(line));
        printf("OUTPUT: %s\n", line);
    }
    return 0;
}
