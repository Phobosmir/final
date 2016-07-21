#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

#include "server.h"
#include "server_lowlvl.h"
#include "daemonize.h"
#include "logger.h"

#include <sys/socket.h>

void server_stop_signal_handler(int sig) {
    log_message("terminate signal catched");
    server_close();
    exit(EXIT_SUCCESS);
}

void setup_signals(){
    signal(SIGCHLD, SIG_IGN); 
    signal(SIGTSTP, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTERM, server_stop_signal_handler);
}

static int server_socket;
void server_run(char *ip4_addr, int port) {
    char logstr[255];
    sprintf(logstr, "\nStarting server at %s:%d", 
            ip4_addr, port);
    log_message(logstr);

    int daemon_status = daemonize();
    if (daemon_status == 0){
        // parent exit
        exit(EXIT_SUCCESS);
    } else if (daemon_status < 0) {
        log_message("Daemon starting failed");
        exit(EXIT_FAILURE);
    }
    setup_signals();

    server_socket = start_server(ip4_addr, port);
    if (server_socket == -1) {
	log_message("Socket init failed");
	exit(EXIT_FAILURE);
    }
    
    while(1){
        int inc_socket = accept(server_socket, 0, 0);
        if (inc_socket == -1) {
            perror("accept");
            log_message("accept failed");
        }
    }
    
}

void server_close(){
    if (stop_server(server_socket) == -1) {
        log_message("Server stop failed");
        exit(EXIT_FAILURE);
    }
    log_message("Server stopped");
}
