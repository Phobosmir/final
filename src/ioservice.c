#include <sys/epoll.h>
#include <unistd.h>
#include "ioservice.h"
#include "logger.h"
#include "server_lowlvl.h"

static int epoll_fd;
int ioservice_init(){
    epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        log_perror("epoll_create");
        return -1;
    }
    return 0;
}

void ioservice_close(){
    if (close(epoll_fd) == -1) {
        log_perror("close epoll");
    }
}

int ioservice_add(int fd){
    if (set_nonblock(fd) == -1) {
        log_perror("set_nonblock");
        return -1;
    }
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = fd;
    int s = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &ev);
    if (s == -1) {
        log_perror("epoll_ctl");
        return -1;
    }
    return 0;
}
