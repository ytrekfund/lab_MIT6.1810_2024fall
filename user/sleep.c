#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: %s <nticks>\n", argv[0]);
        exit(1);
    }
    int nticks = atoi(argv[1]);
    sleep(nticks);
    exit(0);
}
