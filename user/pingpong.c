#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main() {
    int ping_pipe[2] = {0};
    int pong_pipe[2] = {0};
    int ret = pipe(ping_pipe);
    if (ret != 0) {
        printf("pipe err %d\n", ret);
        exit(1);
    }
    ret = pipe(pong_pipe);
    if (ret != 0) {
        printf("pipe err %d\n", ret);
        exit(1);
    }

    int pid = fork();
    if (pid < 0) {
        printf("fork err %d\n", pid);
        exit(1);
    }

    int mypid = getpid();
    if (mypid < 0) {
        printf("getpid err %d\n", mypid);
        exit(1);
    }
    // parent
    if (pid > 0) {
        close(ping_pipe[0]);
        close(pong_pipe[1]);
        // send ping
        int nbytes = write(ping_pipe[1], "o", 1);
        if (nbytes != 1) {
            printf("write err %d\n", nbytes);
            exit(1);
        }
        // recv pong
        char buf;
        nbytes = read(pong_pipe[0], &buf, 1);
        if (nbytes != 1) {
            printf("read err %d\n", nbytes);
            exit(1);
        }
        printf("%d: received pong\n", mypid);
        exit(0);
    }
    // child
    if (pid == 0) {
        close(ping_pipe[1]);
        close(pong_pipe[0]);
        // recv ping
        char buf;
        int nbytes = read(ping_pipe[0], &buf, 1);
        if (nbytes != 1) {
            printf("read err %d\n", nbytes);
            exit(1);
        }
        printf("%d: received ping\n", mypid);
        // send pong
        nbytes = write(pong_pipe[1], &buf, 1);
        if (nbytes != 1) {
            printf("write err %d\n", nbytes);
            exit(1);
        }
        exit(0);
    }
    // unreachable..
    exit(1);
}
