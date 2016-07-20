#include <linux/limits.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "arguments.h"
#include "daemonize.h"
#include "logger.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

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


    int master_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (master_socket == -1) {
        perror("socket");
        exit(EXIT_FAILURE);
    }
    struct sockaddr_in socket_address;
    socket_address.sin_family = AF_INET;
    socket_address.sin_port = htons(port);

    if (inet_pton(AF_INET, ip4_addr, &socket_address.sin_addr) != 1) {
        // log_message();
        exit(EXIT_FAILURE);
    }
    
    sleep(10);

    close(master_socket);

    return EXIT_SUCCESS;
}
