#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/fcntl.h"

void find(char *dir, char *name) {
    int fd = open(dir, O_RDONLY);
    if (fd < 0) {
        printf("open err %d\n", fd);
        exit(1);
    }
    struct stat st;
    int ret = fstat(fd, &st);
    if (ret < 0) {
        printf("fstat err %d\n", ret);
        exit(1);
    }
    if (st.type != T_DIR) {
        printf("%s not dir\n", dir);
        exit(1);
    }

    char buf[512];
    strcpy(buf, dir);
    char *p = buf + strlen(buf);
    *p++ = '/';
    
    struct dirent de;
    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
        if (de.inum == 0) {
            continue;
        }
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        struct stat st;
        ret = stat(buf, &st);
        if (ret != 0) {
            printf("stat err %d\n", ret);
            exit(1);
        }
        if (strcmp(de.name, name) == 0) {
            printf("%s\n", buf);
        }
        if (strcmp(de.name, ".") == 0 ||
            strcmp(de.name, "..") == 0) {
            continue;
        }
        if (st.type == T_DIR) {
            find(buf, name);
        }
    }
    close(fd);
}

int main(int argc, char **argv) {
    if (argc != 3) {
        printf("Usage: %s <dir> <name>\n", argv[0]);
        exit(1);
    }
    find(argv[1], argv[2]);
    exit(0);
}
