#include "runtime.h"
#include "sage_port.h"
#include "value.h"
#include "module.h"
#include "env.h"
#include "interpreter.h"
#include "compiler.h"
#include "parser.h"
#include "lexer.h"
#include "vm.h"
#include "gc.h"

#include "console.h"
#include "keyboard.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

extern void register_sageos_natives(ModuleCache* cache);

static ModuleCache* g_kernel_module_cache = NULL;
static Env* g_kernel_global_env = NULL;

void sage_kernel_early_init(void) {
    sage_repl_init();
}

void sage_repl_init(void) {
    if (g_kernel_module_cache != NULL) return;

    gc_init();
    g_kernel_module_cache = create_module_cache();
    g_kernel_global_env = env_create(NULL);

    // Register SageOS kernel natives
    register_sageos_natives(g_kernel_module_cache);

    // Load standard library if possible
    // init_stdlib(g_kernel_global_env);
}

void sage_execute(const char* line) {
    if (line == NULL || line[0] == '\0') return;

    sage_repl_init();

    init_lexer(line, "<inline>");
    parser_init();

    while (1) {
        Stmt* stmt = parse();
        if (stmt == NULL) break;

        ExecResult result = interpret(stmt, g_kernel_global_env);
        if (result.is_throwing) {
            printf("\nRuntime error (exception thrown)\n");
        }
    }
}

void sage_run_file(const char* path) {
    if (path == NULL) return;
    sage_repl_init();
    
    Module* mod = load_module(g_kernel_module_cache, path);
    if (mod == NULL) {
        printf("\nsage: failed to load module: %s\n", path);
    }
}

void sage_import_module(const char* name) {
    sage_repl_init();
    load_module(g_kernel_module_cache, name);
}

// The old REPL VM global
void* g_repl_vm = NULL;
