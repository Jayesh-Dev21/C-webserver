#include "headers.h" //all headers

#define port 3003

#define FILEPATH "../logs/server_activity.log"
const unsigned short backlog = 10; 

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
    
    int yes=1;
    //char yes='1'; // Solaris people use this
    // lose the pesky "Address already in use" error message
    if(setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof yes) == -1){
        perror("setsockopt");
        exit(1);
    }

    // here backlog is how many pending connections you can have before the kernel starts rejecting new one
    if(-1 == listen(sockfd, backlog)){
        perror("Error establishing connection, check previous number of requests\n");
        return error_ret();
    } 

    fprintf(stdout, "Server started on %d\nconnect via: http://localhost:%d\n", port, port);

    FILE* log_file = fopen("accesslog.log", "a");
    if (!log_file) {
        perror("Could not open log file");
        return error_ret();
    }
    
    struct sockaddr_in client_addr;
    socklen_t client_len = sizeof(client_addr);

    const char* response = "HTTP/1.1 200 OK\r\nContent-Length: 14\r\n\r\nHello, World!\n";
    const int response_len = strlen(response);

    sleep(1);

    while(!condition){

       int clientsocket = accept(sockfd, (struct sockaddr*)&hint, &addr_len);
        if(-1 == clientsocket){
            perror("Error accepting connection and reciveing a socket descriptor\n");
            continue;
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

        else if(strcmp(path, "/about") == 0){
            strcpy(path, "/static/about.html");
        }

        else if(strcmp(path, "/404") == 0){
            strcpy(path, "/static/response/404.html");
        }

        else if(strcmp(path, "/page-1") == 0){
            strcpy(path, "/static/wow.html");
        }

        else if(strcmp(path, "/page-2") == 0){
            strcpy(path, "/static/page2.html");
        }

        else if(strcmp(path, "/list") == 0){
            strcpy(path, "/static/list.html");
        }

        else if(strcmp(path, "/200") == 0){
            strcpy(path, "/static/response/200.html");
        }

        else if(strcmp(path, "/403") == 0){
            strcpy(path, "/static/response/403.html");
        }

        // else if(strstr(path, "..") != NULL){
        //     strcpy(path, "/static/response/403.html");  // You can make a 403.html
        // }

        else if(strncmp(path, "/images/", 8) == 0 ||
         strncmp(path, "/js/", 4) == 0 ||
         strncmp(path, "/styles/", 8) == 0){}

        // else{
        //     strcpy(path, "/static/response/404.html");
        // }

        // Prevent directory traversal
        if (strstr(path, "..") != NULL) {
            const char* forbidden = "HTTP/1.1 403 Forbidden\r\nContent-Length: 13\r\n\r\n403 Forbidden";
            log_error("403", "Forbidden path", path);
            write(clientsocket, forbidden, strlen(forbidden));
            close(clientsocket);
            continue;
        }


        // Construct full file path
        char full_path[2048];
        snprintf(full_path, sizeof(full_path), "./public/%s", path);

        FILE* file = fopen(full_path, "rb");
        if (!file) {
            const char* forbidden = "HTTP/1.1 403 Forbidden\r\nContent-Length: 13\r\n\r\n403 Forbidden";
            write(clientsocket, forbidden, strlen(forbidden));

            char client_ip[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));
            int client_port = ntohs(client_addr.sin_port);
            log_event_tofile(log_file, "403", client_ip, client_port, method, path, "1.1", 403);
            log_event_tostdout("403", client_ip, client_port, method, path, "1.1", 403);

            close(clientsocket);
            continue;
        }


        fseek(file, 0, SEEK_END);
        long filesize = ftell(file);
        rewind(file);

        char* file_buf = malloc(filesize);
        if (!file_buf) {
            log_error("ERROR", "Memory allocation failed", full_path);
            fclose(file);
            close(clientsocket);
            continue;
        }

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