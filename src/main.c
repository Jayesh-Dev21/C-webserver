#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>
#include <errno.h> // error handling
#include <pthread.h> // for multithreaded
#include <arpa/inet.h>

#define port 3003

typedef enum{false,true}bool;

int error_ret(){
    return -1;
}

const char* get_mime_type(const char* path) {
    const char* ext = strrchr(path, '.');
    if (!ext) return "text/plain";
    if (strcmp(ext, ".html") == 0) return "text/html";
    if (strcmp(ext, ".css") == 0) return "text/css";
    if (strcmp(ext, ".js") == 0) return "application/javascript";
    if (strcmp(ext, ".png") == 0) return "image/png";
    if (strcmp(ext, ".jpg") == 0 || strcmp(ext, ".jpeg") == 0) return "image/jpeg";
    if (strcmp(ext, ".gif") == 0) return "image/gif";
    return "application/octet-stream";
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

        int read_d = read(newfd, buff, size_buff);
        if(-1 == read_d){
            perror("Error reading from socket\n");
            close(newfd);
            continue;
        }
        fprintf(stdout, "Request Recived via port: %d\nconnect via: http://localhost:%d\n%s\n\n", port, port, buff);
        // write(newfd, response, response_len);

        // !! new shit !! // 
        // ^ Help of ai ^ //

        char method[8], path[1024];
        sscanf(buff, "%s %s", method, path);

        // Default to index.html if requesting "/"
        if (strcmp(path, "/") == 0) {
            strcpy(path, "/index.html");
        }

        // Construct full file path
        char full_path[2048];
        snprintf(full_path, sizeof(full_path), "./public/%s", path);

        FILE* file = fopen(full_path, "rb");
        if (!file) {
            const char* not_found = "HTTP/1.1 404 Not Found\r\nContent-Length: 13\r\n\r\n404 Not Found";
            write(newfd, not_found, strlen(not_found));
            close(newfd);
            continue;
        }

        fseek(file, 0, SEEK_END);
        long filesize = ftell(file);
        rewind(file);

        char* file_buf = malloc(filesize);
        fread(file_buf, 1, filesize, file);
        fclose(file);

        const char* mime = get_mime_type(full_path);
        char header[512];
        snprintf(header, sizeof(header),
            "HTTP/1.1 200 OK\r\nContent-Type: %s\r\nContent-Length: %ld\r\n\r\n", mime, filesize);

        write(newfd, header, strlen(header));
        write(newfd, file_buf, filesize);

        free(file_buf);
        // ^^ ^^//

        close(newfd);
    }
    
    return 0;   
}