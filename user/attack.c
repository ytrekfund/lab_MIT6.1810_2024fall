#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"
#include "kernel/riscv.h"

int
main(int argc, char *argv[])
{
  // your code here.  you should write the secret to fd 2 using write
  // (e.g., write(2, secret, 8)
  uint64 feat = *(uint64 *)("very ver");
  char *end = sbrk(PGSIZE*32);
  for (int i = 0; i < 32; i ++) {
    char *thispage = end + i * PGSIZE;
    if ((*(uint64 *)(thispage + 8)) == feat) {
      write(2, thispage + 32, 8);
      break;
    }
  }

  exit(1);
}
