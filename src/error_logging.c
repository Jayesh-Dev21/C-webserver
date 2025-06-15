#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "error_logging.h"

void log_error(const char* type, const char* message, const char* extra){
    fprintf(stderr, "[%s] %s: %s\n", type, message, extra);
}
