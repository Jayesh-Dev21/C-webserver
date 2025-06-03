#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>
#include <errno.h> //for multithreaded
#include <arpa/inet.h>

#define port 3003

typedef enum{false,true}bool;

int error_ret(){
    return -1;
}

int main(int argc, char* argv[]){

    //initialising boolean condition 
    bool condition = false;

    // choosing a port
    uint16_t por_t = htons(port);
    // A char buffer for the requests, with all values 0x000
    char buff[1024] = {0};

    size_t size_buff = sizeof(buff);

    struct sockaddr_in addr; // from socket.h
    socklen_t addr_len = sizeof(addr);


    addr.sin_family = AF_INET;
    addr.sin_port = por_t;
    addr.sin_addr.s_addr = INADDR_ANY; //uses your IPV4 address
    
    int sockfd = socket(PF_INET, SOCK_STREAM, 0); //or listener
    if(-1 == sockfd){
        perror("Error in creating socket\n");
        // or use fprintf(stderr, "Error in creating socket"); to return directly to std-error
        return error_ret();
    }

    // printf("%hu", INADDR_ANY); => 0

    sleep(1);

    // bind file descriptor
    int bindfd = bind(sockfd, (struct sockaddr*)&addr, sizeof addr);
    if(-1 == bindfd){
        perror("Error in creating bindfd\n");
        return error_ret();
    }
    //bindfd returns 0 for success or -1 for ^^^ error 
    
    unsigned short backlog = 10; 
    // here backlog is how many pending connections you can have before the kernel starts rejecting new one
    if(-1 == listen(sockfd, backlog)){
        perror("Error establishing connection, check previous number of requests\n");
        return error_ret();
    } 

    fprintf(stdout, "Server started on %d\nconnect via: http://localhost:%d\n", port, port);

    const char* response = "HTTP/1.1 200 OK\r\nContent-Length: 14\r\n\r\nHello, World!\n";
    const int response_len = strlen(response);

    sleep(1);

    while(!condition){

       int newfd = accept(sockfd, (struct sockaddr*)&addr, &addr_len);
        if(-1 == newfd){
            perror("Error accepting connection and reciveing a socket descriptor\n");
            return error_ret();
        }

        read(newfd, buff, size_buff);
        fprintf(stdout, "Request Recived via port: %d\nconnect via: http://localhost:%d\n%s\n\n", port, port, buff);
        write(newfd, response, response_len);

        close(newfd);
    }
    
    return 0;   
}