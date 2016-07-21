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

static int serverlockfile_fd;

int single_server_guard_lock(){
    int lock_fd = open(LOCKFILE, O_RDWR | O_CREAT, 0640);
    if (lock_fd == -1){
        perror("guard lock, open");
        return -1;
    }
    if (lockf(lock_fd, F_TLOCK, 0) == -1){
        perror("guard lock, lockf");
        return -1;
    }
    char str[10];
    sprintf(str, "%d\n", getpid());
    if (write(lock_fd, str, strlen(str)) == -1) {
        perror("guard lock, write");
        return -1;
    }
    return lock_fd;
}

int single_server_guard_unlock(int lock_fd){
    if (lockf(lock_fd, F_ULOCK, 0) == -1){
        perror("guard unlock, lockf");
        return -1;
    }
    if (close(lock_fd) == -1) {
        perror("guard unlock, close");
        return -1;
    }
    if (unlink(LOCKFILE) == -1) {
        perror("guard unlock, unlink");
        return -1;
    }
    return 0;
}

int start_server(char *ip4_addr, int port) {
    if ((serverlockfile_fd = single_server_guard_lock()) == -1)
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
    if (shutdown(master_socket, SHUT_RDWR))
        return -1;
    if (close(master_socket) == -1) {
        perror("close socket");
        return -1;
    }
    if (single_server_guard_unlock(serverlockfile_fd) == -1) {
        return -1;
    }
    return 0;
}
