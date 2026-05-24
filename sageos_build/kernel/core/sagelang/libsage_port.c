#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>

/* 
 * libsage_port.c — Definitive bridge and stubs for SageOS Kernel link
 *
 * This file provides ONLY the symbols that are NOT provided by sage_libc_shim.c.
 */

#include "sage_libc_shim.h"
#include "metal_vm.h"
#include "metal_value.h"

/* Helper to provide both standard and sage_ prefixed symbols */
#define PROVIDE_STUB(name, ret, args, body) \
    ret name args body \
    ret sage_##name args body

/* 1. Missing Standard C / POSIX symbols */

#undef strrchr
#undef getenv
#undef system
#undef stat
#undef fopen
#undef fclose
#undef fwrite
#undef fread
#undef fseek
#undef ftell
#undef rewind
#undef remove
#undef opendir
#undef readdir
#undef closedir
#undef getline
#undef nanosleep
#undef gettimeofday
#undef fgets
#undef errno
#undef strerror
#undef fputs
#undef exit
#undef abort
#undef dlopen
#undef dlerror
#undef dlclose
#undef dlsym

int errno = 0;
char* strerror(int e) { (void)e; return "Unknown error"; }
char* sage_strerror(int e) { return strerror(e); }

void* fopen(const char* p, const char* m) { (void)p; (void)m; return (void*)0; }
void* sage_fopen(const char* p, const char* m) { return fopen(p, m); }

int fclose(void* f) { (void)f; return 0; }
int sage_fclose(void* f) { return fclose(f); }

size_t fwrite(const void* p, size_t s, size_t n, void* f) { (void)p; (void)s; (void)n; (void)f; return 0; }
size_t sage_fwrite(const void* p, size_t s, size_t n, void* f) { return fwrite(p, s, n, f); }

size_t fread(void* p, size_t s, size_t n, void* f) { (void)p; (void)s; (void)n; (void)f; return 0; }
size_t sage_fread(void* p, size_t s, size_t n, void* f) { return fread(p, s, n, f); }

int fseek(void* f, long o, int w) { (void)f; (void)o; (void)w; return -1; }
int sage_fseek(void* f, long o, int w) { return fseek(f, o, w); }

long ftell(void* f) { (void)f; return -1; }
long sage_ftell(void* f) { return ftell(f); }

void rewind(void* f) { (void)f; }
void sage_rewind(void* f) { rewind(f); }

int remove(const char* p) { (void)p; return -1; }
int sage_remove(const char* p) { return remove(p); }

char* fgets(char* s, int n, void* f) { (void)s; (void)n; (void)f; return (void*)0; }
char* sage_fgets(char* s, int n, void* f) { return fgets(s, n, f); }

int fputs(const char* s, void* f) { (void)s; (void)f; return 0; }
int sage_fputs(const char* s, void* f) { return fputs(s, f); }

void* opendir(const char* n) { (void)n; return (void*)0; }
void* sage_opendir(const char* n) { return opendir(n); }

void* readdir(void* d) { (void)d; return (void*)0; }
void* sage_readdir(void* d) { return readdir(d); }

int closedir(void* d) { (void)d; return 0; }
int sage_closedir(void* d) { return closedir(d); }

long getline(char** l, size_t* n, void* f) { (void)l; (void)n; (void)f; return -1; }
long sage_getline(char** l, size_t* n, void* f) { return getline(l, n, f); }

char* getenv(const char* n) { (void)n; return (void*)0; }
char* sage_getenv(const char* n) { return getenv(n); }

int system(const char* c) { (void)c; return -1; }
int sage_system(const char* c) { return system(c); }

struct stat;
int stat(const char* p, struct stat* b) { (void)p; (void)b; return -1; }
int sage_stat(const char* p, void* b) { return stat(p, (struct stat*)b); }

struct timespec;
int nanosleep(const struct timespec* req, struct timespec* rem) { (void)req; (void)rem; return -1; }
int sage_nanosleep(const void* req, void* rem) { return nanosleep((const struct timespec*)req, (struct timespec*)rem); }

struct timeval;
int gettimeofday(struct timeval* tv, void* tz) { (void)tv; (void)tz; return -1; }
int sage_gettimeofday(struct timeval* tv, void* tz) { return gettimeofday((struct timeval*)tv, tz); }

char* strrchr(const char* s, int c) {
    char* last = (void*)0;
    do { if (*s == (char)c) last = (char*)s; } while (*s++);
    return last;
}
char* sage_strrchr(const char* s, int c) { return strrchr(s, c); }

/* 2. Math Stubs (ones not in shim) */
#undef cos
#undef tan
#undef asin
#undef acos
#undef atan
#undef atan2
#undef log
#undef log10
#undef exp
#undef round
#undef fabs
#undef floor
#undef ceil
#undef pow
#undef sqrt
#undef fmod

double sin(double x) { (void)x; return 0; }
double cos(double x) { (void)x; return 0; }
double tan(double x) { (void)x; return 0; }
double asin(double x) { (void)x; return 0; }
double acos(double x) { (void)x; return 0; }
double atan(double x) { (void)x; return 0; }
double atan2(double y, double x) { (void)y; (void)x; return 0; }
double log(double x) { (void)x; return 0; }
double log10(double x) { (void)x; return 0; }
double exp(double x) { (void)x; return 0; }
double round(double x) { return (x >= 0) ? (long)(x + 0.5) : (long)(x - 0.5); }
double fabs(double x) { return (x < 0) ? -x : x; }
double floor(double x) { return (long)x - (x < 0 && x != (long)x); }
double ceil(double x) { return (long)x + (x > 0 && x != (long)x); }
double pow(double x, double y) { (void)x; (void)y; return 0; }
double sqrt(double x) { (void)x; return 0; }
double fmod(double x, double y) { (void)x; (void)y; return 0; }

double sage_sin(double x) { return sin(x); }
double sage_cos(double x) { return cos(x); }
double sage_tan(double x) { return tan(x); }

int rand(void) { return 42; }

/* 3. Dynamic Loading Stubs */
void* dlopen(const char* f, int fl) { (void)f; (void)fl; return (void*)0; }
char* dlerror(void) { return "No DL"; }
int dlclose(void* h) { (void)h; return -1; }
void* dlsym(void* h, const char* s) { (void)h; (void)s; return (void*)0; }

/* 4. Global Variables & Threading */
int g_repl_mode = 0;
int sage_cpu_count = 1;
int sage_cpu_physical_cores = 1;
int sage_cpu_has_hyperthreading = 0;

int sage_thread_set_affinity(int c) { (void)c; return -1; }
int sage_thread_get_core(void) { return 0; }
int sage_thread_join(int t) { (void)t; return -1; }
int sage_thread_create(void** t, void* f, void* a) { (void)t; (void)f; (void)a; return -1; }
int sage_thread_id(void) { return 0; }
void sage_sleep_secs(int s) { (void)s; }

int sage_sem_init(void** s, int v) { (void)s; (void)v; return -1; }
int sage_sem_wait(void* s) { (void)s; return -1; }
int sage_sem_post(void* s) { (void)s; return -1; }
int sage_sem_trywait(void* s) { (void)s; return -1; }

int sage_mutex_init(void** m) { (void)m; return 0; }
int sage_mutex_lock(void* m) { (void)m; return 0; }
int sage_mutex_unlock(void* m) { (void)m; return 0; }
int sage_mutex_trylock(void* m) { (void)m; return 0; }
void sage_mutex_destroy(void* m) { (void)m; }

/* 5. Module Creation Stubs */
void* create_net_module(void) { return (void*)0; }
void* create_socket_module(void) { return (void*)0; }
void* create_tcp_module(void) { return (void*)0; }
void* create_http_module(void) { return (void*)0; }
void* create_ssl_module(void) { return (void*)0; }
void* create_heartbeat_module(void) { return (void*)0; }
void* create_gpu_module(void) { return (void*)0; }
void* create_gpu_api_module(void) { return (void*)0; }
void* create_ml_module(void) { return (void*)0; }
void* create_lsp_module(void) { return (void*)0; }

/* 6. SageOS Natives for old VM bridge */
MetalValue n_len(MetalVM* vm, MetalValue* args, int argc) { (void)vm; (void)args; (void)argc; return mv_nil(); }
MetalValue n_os_strlen(MetalVM* vm, MetalValue* args, int argc) { (void)vm; (void)args; (void)argc; return mv_nil(); }
MetalValue n_os_starts_with(MetalVM* vm, MetalValue* args, int argc) { (void)vm; (void)args; (void)argc; return mv_nil(); }
MetalValue n_os_array_len(MetalVM* vm, MetalValue* args, int argc) { (void)vm; (void)args; (void)argc; return mv_nil(); }
MetalValue n_os_array_push(MetalVM* vm, MetalValue* args, int argc) { (void)vm; (void)args; (void)argc; return mv_nil(); }
MetalValue n_os_write_str(MetalVM* vm, MetalValue* args, int argc) { (void)vm; (void)args; (void)argc; return mv_nil(); }
MetalValue n_os_num_to_str(MetalVM* vm, MetalValue* args, int argc) { (void)vm; (void)args; (void)argc; return mv_nil(); }
MetalValue n_os_stat(MetalVM* vm, MetalValue* args, int argc) { (void)vm; (void)args; (void)argc; return mv_nil(); }
