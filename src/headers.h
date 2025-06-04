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

const char* get_mime_type(const char* path);