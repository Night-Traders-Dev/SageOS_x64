#include "value.h"
#include "module.h"
#include "env.h"
#include "io.h"
#include "console.h"
#include "scheduler.h"
#include "serial.h"
#include "keyboard.h"
#include "ata.h"
#include "sdhci.h"
#include "net.h"
#include "acpi.h"
#include "wifi_qca6174.h"
#include "pci.h"
#include "smp.h"
#include "swap.h"
#include "sysinfo.h"
#include "idt.h"
#include "bootinfo.h"
#include "vfs.h"
#include "shell.h"
#include "version.h"
#include <string.h>
#include <stdio.h>

// External kernel functions
extern void ata_timer_tick(void);
extern void console_periodic_flip(void);
extern void timer_irq(void);
extern SageOSBootInfo* console_boot_info(void);
extern uint32_t console_get_fg(void);

// --- Timer Natives ---

static Value n_outb(int argCount, Value* args) {
    if (argCount < 2) return val_nil();
    uint16_t port = (uint16_t)AS_NUMBER(args[0]);
    uint8_t val = (uint8_t)AS_NUMBER(args[1]);
    outb(port, val);
    return val_nil();
}

static Value n_console_periodic_flip(int argCount, Value* args) {
    (void)argCount; (void)args;
    console_periodic_flip();
    return val_nil();
}

static Value n_ata_timer_tick(int argCount, Value* args) {
    (void)argCount; (void)args;
    ata_timer_tick();
    return val_nil();
}

static Value n_sched_timer_tick(int argCount, Value* args) {
    (void)argCount; (void)args;
    sched_timer_tick();
    return val_nil();
}

static Value n_timer_irq(int argCount, Value* args) {
    (void)argCount; (void)args;
    timer_irq();
    return val_nil();
}

// --- Driver Natives ---

static Value n_serial_init(int argCount, Value* args) {
    (void)argCount; (void)args;
    serial_init(); return val_nil();
}
static Value n_keyboard_init(int argCount, Value* args) {
    (void)argCount; (void)args;
    keyboard_init(); return val_nil();
}
static Value n_console_init(int argCount, Value* args) {
    (void)argCount; (void)args;
    console_init(console_boot_info()); return val_nil();
}
static Value n_ata_init(int argCount, Value* args) {
    (void)argCount; (void)args;
    ata_init(); return val_nil();
}
static Value n_sdhci_init(int argCount, Value* args) {
    (void)argCount; (void)args;
    sdhci_init(); return val_nil();
}
static Value n_net_init(int argCount, Value* args) {
    (void)argCount; (void)args;
    net_init(); return val_nil();
}
static Value n_acpi_init(int argCount, Value* args) {
    (void)argCount; (void)args;
    acpi_init(console_boot_info()); return val_nil();
}
static Value n_qca6174_init(int argCount, Value* args) {
    (void)argCount; (void)args;
    qca6174_init(); return val_nil();
}
static Value n_pci_enumerate(int argCount, Value* args) {
    (void)argCount; (void)args;
    pci_enumerate(); return val_nil();
}
static Value n_smp_init(int argCount, Value* args) {
    (void)argCount; (void)args;
    smp_init(); return val_nil();
}
static Value n_smp_init_firmware_bsp(int argCount, Value* args) {
    (void)argCount; (void)args;
    smp_init_firmware_bsp(); return val_nil();
}
static Value n_swap_init(int argCount, Value* args) {
    (void)argCount; (void)args;
    swap_init(); return val_nil();
}
static Value n_idt_init(int argCount, Value* args) {
    (void)argCount; (void)args;
    idt_init(); return val_nil();
}
static Value n_irq_enable(int argCount, Value* args) {
    (void)argCount; (void)args;
    irq_enable(); return val_nil();
}

// --- OS Natives ---

static Value n_os_version_string(int argCount, Value* args) {
    (void)argCount; (void)args;
    return val_string(SAGEOS_VERSION);
}

static Value n_os_write_char(int argCount, Value* args) {
    if (argCount >= 1 && IS_NUMBER(args[0])) {
        console_putc((char)AS_NUMBER(args[0]));
    }
    return val_nil();
}

static Value n_os_write_str(int argCount, Value* args) {
    if (argCount >= 1 && IS_STRING(args[0])) {
        console_write(AS_STRING(args[0]));
    }
    return val_nil();
}

static Value n_os_set_color_hex(int argCount, Value* args) {
    if (argCount >= 1 && IS_NUMBER(args[0])) {
        console_set_fg((uint32_t)AS_NUMBER(args[0]));
    }
    return val_nil();
}

static Value n_os_path_exists(int argCount, Value* args) {
    if (argCount < 1 || !IS_STRING(args[0])) return val_bool(0);
    VfsStat st;
    return val_bool(vfs_stat(AS_STRING(args[0]), &st) == 0);
}

static Value n_os_cat(int argCount, Value* args) {
    if (argCount < 1 || !IS_STRING(args[0])) return val_nil();
    const char* path = AS_STRING(args[0]);
    char buf[513];
    uint64_t off = 0;
    while (1) {
        int n = vfs_read(path, off, buf, 512);
        if (n <= 0) break;
        buf[n] = 0;
        console_write(buf);
        off += (uint64_t)n;
    }
    return val_nil();
}

static Value n_os_stat(int argCount, Value* args) {
    if (argCount < 1 || !IS_STRING(args[0])) return val_nil();
    const char* path = AS_STRING(args[0]);
    VfsStat st;
    if (vfs_stat(path, &st) == 0) {
        Value d = val_dict();
        dict_set(&d, "name", val_string(st.name));
        dict_set(&d, "size", val_number((double)st.size));
        dict_set(&d, "type", val_number((double)st.type));
        return d;
    }
    return val_nil();
}

static Value n_os_shell_exec(int argCount, Value* args) {
    if (argCount < 1 || !IS_STRING(args[0])) return val_nil();
    shell_exec_command(AS_STRING(args[0]));
    return val_nil();
}

static Value n_os_get_color(int argCount, Value* args) {
    (void)argCount; (void)args;
    return val_number((double)console_get_fg());
}

static Value n_os_swap_is_available(int argCount, Value* args) {
    (void)argCount; (void)args;
    return val_bool(swap_is_available());
}

// --- Module Registration ---

void register_sageos_natives(ModuleCache* cache) {
    Module* os = create_native_module(cache, "os");
    
    // Timer
    env_define(os->env, "outb", 4, val_native(n_outb));
    env_define(os->env, "console_periodic_flip", 21, val_native(n_console_periodic_flip));
    env_define(os->env, "ata_timer_tick", 14, val_native(n_ata_timer_tick));
    env_define(os->env, "sched_timer_tick", 16, val_native(n_sched_timer_tick));
    env_define(os->env, "timer_irq", 9, val_native(n_timer_irq));
    
    // Drivers
    env_define(os->env, "serial_init", 11, val_native(n_serial_init));
    env_define(os->env, "keyboard_init", 13, val_native(n_keyboard_init));
    env_define(os->env, "console_init", 12, val_native(n_console_init));
    env_define(os->env, "ata_init", 8, val_native(n_ata_init));
    env_define(os->env, "sdhci_init", 10, val_native(n_sdhci_init));
    env_define(os->env, "net_init", 8, val_native(n_net_init));
    env_define(os->env, "acpi_init", 9, val_native(n_acpi_init));
    env_define(os->env, "qca6174_init", 12, val_native(n_qca6174_init));
    env_define(os->env, "pci_enumerate", 13, val_native(n_pci_enumerate));
    env_define(os->env, "smp_init", 8, val_native(n_smp_init));
    env_define(os->env, "smp_init_firmware_bsp", 21, val_native(n_smp_init_firmware_bsp));
    env_define(os->env, "swap_init", 9, val_native(n_swap_init));
    env_define(os->env, "idt_init", 8, val_native(n_idt_init));
    env_define(os->env, "irq_enable", 10, val_native(n_irq_enable));
    
    // OS
    env_define(os->env, "version", 7, val_native(n_os_version_string));
    env_define(os->env, "write_char", 10, val_native(n_os_write_char));
    env_define(os->env, "write_str", 9, val_native(n_os_write_str));
    env_define(os->env, "set_color_hex", 13, val_native(n_os_set_color_hex));
    env_define(os->env, "path_exists", 11, val_native(n_os_path_exists));
    env_define(os->env, "cat", 3, val_native(n_os_cat));
    env_define(os->env, "stat", 4, val_native(n_os_stat));
    env_define(os->env, "shell_exec", 10, val_native(n_os_shell_exec));
    env_define(os->env, "get_color", 9, val_native(n_os_get_color));
    env_define(os->env, "swap_is_available", 17, val_native(n_os_swap_is_available));
}
