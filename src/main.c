#include <linux/limits.h>
#include <stdlib.h>
#include <unistd.h>
#include "arguments.h"
#include "daemonize.h"

int main(int argc, char **argv) {
    char ip4_addr[15];
    int port;
    char work_dir[PATH_MAX];

    if (get_opt_args(argc, argv, ip4_addr, &port, work_dir)){
        exit(EXIT_FAILURE);
    }
    int daemon_status = daemonize();
    if (daemon_status == 0){
        // parent exit
        exit(EXIT_SUCCESS);
    } else if (daemon_status < 0) {
        exit(EXIT_FAILURE);
    }
    sleep(10);
    return EXIT_SUCCESS;
}
