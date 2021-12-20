#include "pipe_networking.h"


int main() {

  int to_client;
  int from_client;

  while (1) {
      
    from_client = server_handshake( &to_client );
    char line[BUFFER_SIZE];

    while (1) {       
            
        int input = read(from_client, line, sizeof(line));
        if (input == -1) break;
        int len = strlen(line);
        int i;
        for (i = 0; i < len; i++) {
            if ((line[i] >= 'a' && line[i] <= 'z')) {
                line[i] = line[i] - 32;
            }
        }
        write(to_client, line, sizeof(line));
    
    }
      
  }
  return 0;
}
