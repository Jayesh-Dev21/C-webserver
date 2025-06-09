#include "headers.h" //all headers

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

    struct sockaddr_in hint; // from socket.h
    socklen_t addr_len = sizeof(hint);

    hint.sin_family = AF_INET;
    hint.sin_port = por_t; // stored as big endian
    hint.sin_addr.s_addr = INADDR_ANY; //uses your IPV4 address
    
    int sockfd = socket(PF_INET, SOCK_STREAM, 0); //or listener
    if(-1 == sockfd){
        perror("Error in creating socket\n");
        // or use fprintf(stderr, "Error in creating socket"); to return directly to std-error
        return error_ret();
    }

    // printf("%hu", INADDR_ANY); => 0

    sleep(1);

    // bind file descriptor
    int bindfd = bind(sockfd, (struct sockaddr*)&hint, sizeof hint);
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

       int clientsocket = accept(sockfd, (struct sockaddr*)&hint, &addr_len);
        if(-1 == clientsocket){
            perror("Error accepting connection and reciveing a socket descriptor\n");
            return error_ret();
        }

        memset(buff, 0, size_buff);

        int read_d = read(clientsocket, buff, size_buff);
        if(-1 == read_d){
            perror("Error reading from socket\n");
            close(clientsocket);
            continue;
        }
        fprintf(stdout, "Request Recived via port: %d\nconnect via: http://localhost:%d\n%s\n\n", port, port, buff);
        // write(clientsocket, response, response_len);

        char method[8], path[1024];
        sscanf(buff, "%s %s", method, path);

        // Default to index.html if requesting "/"
        if (strcmp(path, "/") == 0){
            strcpy(path, "/index.html");
        }

        if(strcmp(path, "/about") == 0){
            strcpy(path, "/static/about.html");
        }

        if(strcmp(path, "/404") == 0){
            strcpy(path, "/static/response/404.html");
        }

        if(strcmp(path, "/page-1") == 0){
            strcpy(path, "/static/wow.html");
        }

        if(strcmp(path, "/page-2") == 0){
            strcpy(path, "/static/page2.html");
        }

        if(strcmp(path, "/list") == 0){
            strcpy(path, "/static/list.html");
        }

        if(strcmp(path, "/200") == 0){
            strcpy(path, "/static/response/200.html");
        }

        if(strcmp(path, "/403") == 0){
            strcpy(path, "/static/response/403.html");
        }

        if(strstr(path, "..") != NULL){
            strcpy(path, "/static/response/403.html");  // You can make a 403.html
        }

        if(strcmp(path, "/images") > 0){}
        if(strcmp(path, "/js") > 0){}
        if(strcmp(path, "/styles") > 0){}

        // else{
        //     strcpy(path, "/static/response/404.html");
        // }

        // Construct full file path
        char full_path[2048];
        snprintf(full_path, sizeof(full_path), "./public/%s", path);

        FILE* file = fopen(full_path, "rb");
        if (!file) {
            const char* not_found = "HTTP/1.1 404 Not Found\r\nContent-Length: 13\r\n\r\n404 Not Found";
            write(clientsocket, not_found, strlen(not_found));
            close(clientsocket);
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

        write(clientsocket, header, strlen(header));
        write(clientsocket, file_buf, filesize);

        free(file_buf);
        // ^^ ^^//

        close(clientsocket);
    }
    
    return 0;   
}