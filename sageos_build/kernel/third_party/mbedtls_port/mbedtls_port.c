#include <mbedtls/platform.h>
#include <mbedtls/entropy.h>
#include "sage_libc_shim.h"
#include "timer.h"

/* mbed TLS platform functions */

void mbedtls_platform_exit_alt(int status) {
    (void)status;
    // Just loop or do nothing in kernel
    for(;;);
}

/* Hardware entropy source */
int mbedtls_hardware_poll(void *data, unsigned char *output, size_t len, size_t *olen) {
    (void)data;
    uint64_t t;
    size_t i = 0;
    while (i < len) {
        t = timer_ticks();
        // Extract some entropy from low bits of timer
        output[i++] = (unsigned char)(t & 0xFF);
        output[i++] = (unsigned char)((t >> 8) & 0xFF);
        // We really should use RDRAND if available, but this is a fallback
    }
    *olen = (i > len) ? len : i;
    return 0;
}

void mbedtls_port_init(void) {
    /* Configure MbedTLS to use SageOS memory functions */
    mbedtls_platform_set_calloc_free(sage_calloc, sage_free);
}
