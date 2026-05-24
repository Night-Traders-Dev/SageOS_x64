#include <stdio.h>
#include "drivers/keyboard.h"
#include "drivers/console.h"

void debug_input() {
    KeyEvent ev;
    printf("Debug: starting keyboard test\n");
    while(1) {
        if(keyboard_wait_event(&ev)) {
             printf("Key: scancode=0x%02x, ascii=%d, pressed=%d\n", ev.scancode, ev.ascii, ev.pressed);
        }
    }
}
