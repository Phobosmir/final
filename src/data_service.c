#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>

#include "logger.h"
#include "data_service.h"
#define REQ_MAX_LENGTH 8192

int process_request(int fd){
    char buffer[REQ_MAX_LENGTH];
    int recv_res = recv(fd, buffer, REQ_MAX_LENGTH, MSG_NOSIGNAL);
    if (recv_res == 0 && errno != EAGAIN) {
        shutdown(fd, SHUT_RDWR);
        close(fd);
    }
    else if (recv_res > 0 ) {
        log_message("new incoming socket");
        log_message(buffer);
        parse_request(buffer);
    }
    return 0;
}

void parse_request(char *req) {
    char req_uri[REQ_MAX_LENGTH];
    sscanf(req, "GET %s HTTP/1.0", req_uri);
    log_message(req_uri);
}

void send_data(int fd, char *filedata) {
    static char ok_header[] = "HTTP/1.0 200 OK\r\n\r\n";
}

void not_found(int fd){
    static char notfound[] = 
"HTTP/1.0 404 Not Found\r\nContent-Length: 0\r\nContent-Type: text/html\r\n\r\n";

}
