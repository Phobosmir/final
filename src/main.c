#include <linux/limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "arguments.h"
#include "daemonize.h"
#include "logger.h"
#include "server.h"

int main(int argc, char **argv) {
    char ip4_addr[15];
    int port;
    char work_dir[PATH_MAX];

    if (get_opt_args(argc, argv, ip4_addr, &port, work_dir)){
        exit(EXIT_FAILURE);
    }
    if (chdir(work_dir) == -1) {
        perror("chdir");
        exit(EXIT_FAILURE);
    }

    char logstr[255];
    sprintf(logstr, "\nStarting server at %s:%d, work dir: %s", ip4_addr, port, work_dir);
    log_message(logstr);

    int daemon_status = daemonize();
    if (daemon_status == 0){
        // parent exit
        exit(EXIT_SUCCESS);
    } else if (daemon_status < 0) {
        log_message("Daemon starting failed");
        exit(EXIT_FAILURE);
    }
    
    int master_socket = start_server(ip4_addr, port);
    if (master_socket == -1) {
	log_message("Socket init failed");
	exit(EXIT_FAILURE);
    }

    sleep(10);
    
    stop_server(master_socket);

    return EXIT_SUCCESS;
}
