#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

int daemonize(){
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        return -1;
    }
    else if (pid > 0)
        return 0;

    if (setsid() == (pid_t)-1) {
        perror("setsid");
        return -1;
    }
    chdir("/");
    fflush(stdout);
    close(fileno(stdin));
    close(fileno(stdout));
    close(fileno(stderr));
    umask(0);
    return 1;
}
