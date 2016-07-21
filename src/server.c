#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "server.h"

#define LOCKFILE "server.lock"

int single_server_guard_lock(){
    int lfp = open(LOCKFILE, O_RDWR | O_CREAT, 0640);
    if (lfp == -1){
        perror("guard lock, open");
        return -1;
    }
    if (lockf(lfp, F_TLOCK, 0) == -1){
        perror("guard lock, lockf");
        return -1;
    }
    char str[10];
    sprintf(str, "%d\n", getpid());
    if (write(lfp, str, strlen(str)) == -1) {
        perror("guard lock, write");
        return -1;
    }
    return 0;
}

int start_server(char *ip4_addr, int port) {
    if (single_server_guard_lock())
        return -1;
    int master_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (master_socket == -1) {
        perror("socket");
	return -1;
    }
    struct sockaddr_in socket_address;
    socket_address.sin_family = AF_INET;
    socket_address.sin_port = htons(port);

    if (inet_pton(AF_INET, ip4_addr, &socket_address.sin_addr) != 1) {
        // log_message();
	return -1;
    }
    if (bind(master_socket, (struct sockaddr*)&socket_address, 
	sizeof(socket_address)) != 0){
	perror("bind");
	return -1;
    }

    if (listen(master_socket, SOMAXCONN)) {
	perror("listen");
	return -1;
    }

    return master_socket;
}

int stop_server(int master_socket){
    shutdown(master_socket, SHUT_RDWR);
    close(master_socket);
    return 0;
}
