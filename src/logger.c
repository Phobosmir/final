#include <stdio.h>
#include "logger.h"
#define LOGFILE "server.log"

void log_message(char *message) {
    FILE *logfile = fopen(LOGFILE, "a");
    if (logfile == NULL){
        perror("logger, fopen");
        return;
    }
    fprintf(logfile, "%s\n", message);
    fclose(logfile);
}
