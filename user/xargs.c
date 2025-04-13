#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: <prev cmd> | %s <next cmd> [args..]\n", argv[0]);
        exit(1);
    }
    char buf[512];
    char *args[MAXARG+1];
    char **pargs = args;
    for (int i = 1; i < argc; i ++) {
        *pargs++ = argv[i];
    }
    *pargs++ = buf;
    *pargs = 0;

    char *p = buf;
    while (1) {
        int nread = read(0, p, 1);
        if (nread < 0) {
            printf("read err %d\n", nread);
            exit(1);
        }
        if (nread == 1 && *p != '\n') {
            p++;
            continue;
        }
        if (nread == 0 && p == buf) {
            break;
        }
        *p = 0;
        int pid = fork();
        if (pid < 0) {
            printf("fork err %d\n", pid);
        }
        // child
        if (pid == 0) {
            int ret = exec(argv[1], args);
            if (ret != 0) {
                printf("exec err %d\n", ret);
                exit(1);
            }
        }
        // parent
        wait(0);
        if (nread == 0) {
            break;
        }
        p = buf;
    }
    exit(0);
}
