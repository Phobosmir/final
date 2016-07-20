#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include "logger.h"

int single_server_guard_lock(){
    int lfp = open("server.lock", O_RDWR | O_CREAT, 0640);
    if (lfp == -1){
        perror("guard lock, open");
        log_message("Cannot open guard file");
        return -1;
    }
    if (lockf(lfp, F_TLOCK, 0) == -1){
        perror("guard lock, lockf");
        log_message("Cannot lock guard file");
        return -1;
    }
    char str[10];
    sprintf(str, "%d\n", getpid());
    if (write(lfp, str, strlen(str)) == -1) {
        perror("guard lock, write");
        log_message("Cannot write to guard file");
        return -1;
    }

    return 0;
}

void close_opened_fds(){
    fflush(stdout);
    int i;
    for (i = getdtablesize(); i>= 0; --i)
            close(i);
    i = open("/dev/null", O_RDWR);
    dup(i);
    dup(i);
}

int daemonize(){
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork");
        log_message("fork process error");
        return -1;
    }
    else if (pid > 0)
        return 0;

    if (setsid() == (pid_t)-1) {
        log_message("setsid error");
        perror("setsid");
        return -1;
    }
    close_opened_fds();
    umask(027);
    if (single_server_guard_lock())
        return -1;
    signal(SIGCHLD, SIG_IGN); 
    return 1;
}
