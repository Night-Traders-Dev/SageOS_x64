#include <stdio.h>
#include "drivers/console.h"
#include "fs/vfs.h"

void debug_vfs() {
    VfsStat st;
    int r = vfs_stat("/bin/sage_shell_combined.sage", &st);
    if (r == 0) {
        console_write("\n[DEBUG] Found shell at /bin/sage_shell_combined.sage, size: ");
        // need a way to print uint64, maybe hex
    } else {
        console_write("\n[DEBUG] Could NOT find /bin/sage_shell_combined.sage\n");
    }
}
