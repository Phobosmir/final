#include <linux/limits.h>
#include <stdlib.h>
#include <unistd.h>
#include "arguments.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char **argv) {
    char ip4_addr[15];
    int port;
    char work_dir[PATH_MAX];

    if (get_opt_args(argc, argv, ip4_addr, &port, work_dir)){
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }
    else if (pid > 0)
        exit(EXIT_SUCCESS);
        
    chdir("/");
    fflush(stdout);
    close(fileno(stdin));
    close(fileno(stdout));
    close(fileno(stderr));
    umask(0);
    sleep(20);
    return EXIT_SUCCESS;
}
