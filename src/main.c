#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
// #include <errno.h>
// #include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define port 3003

typedef enum{false,true}bool;

int main(int argc, char* argv[]){
    
    char buff[256] = {0};
    size_t buff_size = sizeof(buff);

    struct sockaddr_in addr; //socket.h 
    socklen_t addr_len = sizeof(addr);

    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);   
    int accept_fd;

    const char *response = "HTTP/1.1 200 OK\r\nContent-Length: 14\r\n\r\nHello, World!\n";
    
    // socket_fd => socket_file_descriptor
    if(-1 == socket_fd){
        perror("Unable to create socket");
        exit(1);
    }
    
    addr.sin_port = htons(3003); // or 3003 => 0x0BBB and reverse it => 0xBBB0
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    
    int bind_fd = bind(socket_fd, (struct sockaddr*)&addr, sizeof(addr));

    if(0 > bind_fd) {
        perror("Bind failure");
        exit(1);
    }
    
    if(listen(socket_fd, 5) < 0){
        perror("Listen failed");
        exit(1);
    }

    printf( "Server running on port %d\n", port );

    bool is_listening = true;

    while(is_listening){
        
        accept_fd = accept(socket_fd, (struct sockaddr*)&addr, &addr_len);
        
        if (accept_fd < 0) {
            perror("Accept failed");
            continue;
        }
        
        read(accept_fd, buff, buff_size);
        printf("Request received:\n%s\n", buff);

        write(accept_fd, response, strlen(response));   

        close(accept_fd);
    }

    return 0;
}