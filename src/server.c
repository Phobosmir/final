#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "server.h"

int start_server(char *ip4_addr, int port) {
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
