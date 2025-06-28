#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "request_logging.h"

void log_event_tofile(FILE* log_dest, const char* level, const char* client_ip, int client_port, const char* method, const char* path, const char* version, int status_code){

    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char timestamp[64];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", t);

    if (client_ip){
        fprintf(log_dest, "[%s] %s - %s:%d - \"%s %s %s\" - %d\n",
                level, timestamp, client_ip, client_port, method, path, version, status_code);
    } 
    else{
        fprintf(log_dest, "[%s] %s - %s\n", level, timestamp, method); // for general logs
    }

    fflush(log_dest);
}

void log_event_tostdout(const char* level, const char* client_ip, int client_port, const char* method, const char* path, const char* version, int status_code){
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char timestamp[64];
    strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", t);

    if (client_ip){
        fprintf(stdout, "[%s] %s - %s:%d - \"%s %s %s\" - %d\n", 
            level, timestamp, client_ip, client_port, method, path, version, status_code);
    } 
    else{
        fprintf(stdout, "[%s] %s - %s\n", level, timestamp, method); // for server events or errors
    }
}
