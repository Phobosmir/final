#include <linux/limits.h>
#include <stdlib.h>
#include "arguments.h"

#include <stdio.h>

int main(int argc, char **argv) {
    char ip4_addr[15];
    int port;
    char work_dir[PATH_MAX];

    if (get_opt_args(argc, argv, ip4_addr, &port, work_dir)){
        exit(EXIT_FAILURE);
    }
    printf("ip: %s port: %d work_dir: \n %s\n", ip4_addr, port, work_dir);

    return 0;
}
