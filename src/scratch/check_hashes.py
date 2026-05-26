def fnv1a(s):
    h = 2166136261
    for char in s:
        h ^= ord(char)
        h = (h * 16777619) & 0xffffffff
    return h

names = [
    "os_write_char", "os_write_str", "os_read_char", "os_read_key", "os_poll_char", "os_poll_key",
    "os_set_color_hex", "os_set_color", "os_get_color", "os_console_clear", "os_cursor_home",
    "os_input_begin", "os_line_redraw", "len", "os_strlen", "os_streq", "os_char_at", "os_substr",
    "os_str_chop", "os_chr", "os_num_to_str", "os_array_push", "os_version_string", "os_input_backend",
    "os_fb_available", "os_fb_base_str", "os_fb_width_str", "os_fb_height_str", "os_fb_pps_str",
    "os_ram_used_mb", "os_ram_total_mb", "os_ram_used_str", "os_ram_total_str", "os_cpu_percent",
    "os_battery_percent", "os_smp_cpu_count", "os_uptime_str", "os_draw_bar", "os_delay_ms",
    "os_path_exists", "os_ls", "os_cat", "os_mkdir", "os_touch", "os_rm", "os_rm_recursive",
    "os_stat", "os_write", "os_execelf", "os_shell_exec", "os_shell_completion_count",
    "os_shell_completion_at", "os_shell_completion_common", "os_shell_suggestion",
    "os_shell_print_completions", "os_keydebug", "os_dmesg_dump", "os_dmesg_get_total",
    "os_dmesg_get_head", "os_dmesg_get_size", "os_dmesg_get_char", "os_status_print",
    "os_status_refresh", "os_sysinfo", "os_timer_info", "os_smp_info", "os_smp_boot_aps",
    "os_battery_info", "os_pci_info", "os_sdhci_info", "os_acpi_summary", "os_acpi_tables",
    "os_acpi_fadt", "os_acpi_madt", "os_acpi_lid", "os_acpi_battery", "os_shutdown", "os_suspend",
    "os_halt", "os_reboot", "os_qemu_exit", "os_sage_exec", "os_get_c0",
    "shell_prompt", "shell_dispatch", "shell_run", "read_line", "redraw_line",
    "current_suggestion", "history_suggestion", "history_len", "history_get",
    "history_add", "starts_with", "str_insert", "str_delete_before", "str_delete_at",
    "token_start_at", "display_len", "accept_completion", "show_completions",
    "g_history", "g_history_max", "KEY_UP", "KEY_DOWN", "KEY_RIGHT", "KEY_LEFT",
    "KEY_HOME", "KEY_END", "KEY_DELETE", "line", "pos", "displayed_len", "history_nav",
    "saved_line", "done", "key", "llen", "h", "ts", "item", "i", "hlen", "old_color",
    "cmd_path", "res", "running", "old", "logo", "old_fg", "label", "total", "size",
    "head", "start", "count", "idx", "c"
]

hashes = {}
for name in names:
    h = fnv1a(name)
    if h in hashes:
        print(f"COLLISION: {name} and {hashes[h]} both have hash {h:08x}")
    hashes[h] = name
