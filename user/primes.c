#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void primes(int pipefd) __attribute__((noreturn));

void primes(int pipefd) {
    int nextfd[2] = {0};
    int ret = pipe(nextfd);
    if (ret != 0) {
        printf("pipe err %d\n", ret);
        exit(1);
    }

    int lead = 0;
    if (pipefd >= 0) {
        int nread = read(pipefd, &lead, sizeof(lead));
        if (nread < 0) {
            printf("read lead err %d\n", nread);
            exit(1);
        }
        if (nread == 0) {
            exit(0);
        }
    } else {
        lead = 2;
    }
    printf("prime %d\n", lead);

    int pid = fork();
    if (pid < 0) {
        printf("fork err %d\n", pid);
        exit(1);
    }
    // parent
    if (pid > 0) {
        close(nextfd[0]);
        int num = 0;
        while (1) {
            if (pipefd >= 0) {
                int nread = read(pipefd, &num, sizeof(num));
                if (nread < 0) {
                    printf("read err %d\n", nread);
                    exit(1);
                }
                if (nread == 0) {
                    break;
                }
            } else {
                if (num == 0) {
                    num = 2;
                }
                num ++;
                if (num > 280) {
                    break;
                }
            }
            if (num % lead == 0) continue;
            int nwrite = write(nextfd[1], &num, sizeof(num));
            if (nwrite != sizeof(num)) {
                printf("write err %d\n", nwrite);
                exit(1);
            }
        }
        if (pipefd >= 0) {
            close(pipefd);
        }
        close(nextfd[1]);
        wait(0);
        exit(0);
    }
    // child
    if (pid == 0) {
        if (pipefd >= 0) {
            close(pipefd);
        }
        close(nextfd[1]);
        primes(nextfd[0]);
        exit(1);
    }
    exit(1);
}

int main(int argc, char **argv) {
    primes(-1);
    exit(0);
}


