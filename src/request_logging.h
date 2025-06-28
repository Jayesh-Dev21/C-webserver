#ifndef REQUEST_LOGGING_H
#define REQUEST_LOGGING_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

void log_event_tostdout(const char* level, const char* client_ip, int client_port, const char* method, const char* path, const char* version, int status_code);
void log_event_tofile(FILE* log_dest, const char* level, const char* client_ip, int client_port, const char* method, const char* path, const char* version, int status_code);

#endif