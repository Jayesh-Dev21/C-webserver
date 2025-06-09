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
#include "content_headers.h"
#include "error_logging.h"

const char* get_mime_type(const char* path);
void log_error(const char* type, const char* message, const char* extra);

void log_event_tofile(FILE* log_dest, const char* level, const char* client_ip, int client_port, const char* method, const char* path, const char* version, int status_code);

void log_event_tostdout(const char* level, const char* client_ip, int client_port, const char* method, const char* path, const char* version, int status_code);