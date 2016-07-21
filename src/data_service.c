#include <sys/socket.h>
#include <errno.h>
#include <unistd.h>

#include "logger.h"

int process_request(int fd){
    char buffer[1024];
    int recv_res = recv(fd, buffer, 1024, MSG_NOSIGNAL);
    if (recv_res == 0 && errno != EAGAIN) {
        shutdown(fd, SHUT_RDWR);
        close(fd);
    }
    else if (recv_res > 0 ) {
        log_message("new incoming socket");
        log_message(buffer);
    }
    return 0;
}
