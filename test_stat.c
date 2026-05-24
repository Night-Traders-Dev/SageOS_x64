#include <stdio.h>
#include "vfs.h"
int main() {
    VfsStat st;
    int r = vfs_stat("/etc/init.sage", &st);
    printf("stat(/etc/init.sage) = %d\n", r);
    return 0;
}
