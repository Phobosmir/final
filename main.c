#include <unistd.h>
#include <string.h>
#include <linux/limits.h>
#include <stdlib.h>
#include <stdio.h>

void print_usage(char *prog_name){
    fprintf(stderr, "Usage: %s -h <ip> -p <port> -d <directory>\n", prog_name);
}

int main(int argc, char **argv) {
    char ip4_addr[15];
    int port;
    char work_dir[PATH_MAX];

    int opt;
    while ((opt = getopt(argc, argv, "h:p:d:")) != -1 ) {
        switch (opt) {
            case 'h':
                strcpy(ip4_addr, optarg);
                break;
            case 'p':
                port = atoi(optarg);
                break;
            case 'd':
                strcpy(work_dir, optarg);
                break;
            default:
                print_usage(argv[0]);
                exit(EXIT_FAILURE);
        }
    }
    if (optind >= argc) {
        print_usage(argv[0]);
	exit(EXIT_FAILURE);
    }


    return 0;
}
