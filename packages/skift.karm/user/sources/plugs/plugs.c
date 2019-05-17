/* Copyright © 2018-2019 N. Van Bossuyt.                                      */
/* This code is licensed under the MIT License.                               */
/* See: LICENSE.md                                                            */

/* plugs.c: Plugs functions for using the skift Framework in the userspace.   */

#include <skift/cstring.h>
#include <skift/lock.h>
#include <skift/process.h>
#include <skift/logger.h>
#include <skift/__plugs__.h>

lock_t memlock;
lock_t loglock;

iostream_t *in_stream;
iostream_t *out_stream;
iostream_t *err_stream;
iostream_t *log_stream;

void __plug_init(void)
{
    lock_init(memlock);
    lock_init(loglock);

    // Open io stream
    in_stream = NULL; // FIXME: no stdin,
    out_stream = iostream_open("/dev/console", IOSTREAM_WRITE | IOSTREAM_BUFFERED_WRITE);
    err_stream = iostream_open("/dev/console", IOSTREAM_WRITE | IOSTREAM_BUFFERED_WRITE);
    log_stream = iostream_open("/dev/serial", IOSTREAM_WRITE | IOSTREAM_BUFFERED_WRITE);
}

void __plug_fini(int exit_code)
{
    iostream_flush(out_stream);
    iostream_flush(err_stream);
    iostream_flush(log_stream);

    __syscall(SYS_PROCESS_EXIT, exit_code, 0, 0, 0, 0);
}

void __plug_assert_failed(const char *expr, const char *file, const char *function, int line)
{
    log(LOG_FATAL, "assert failed: %s in %s:%s() ln%d!", (char *)expr, (char *)file, (char *)function, line);
    process_exit(-1);
}

int __plug_logger_lock()
{
    lock_acquire(loglock);
    return 0;
}

int __plug_logger_unlock()
{
    lock_release(loglock);
    return 0;
}

int __plug_memalloc_lock()
{
    lock_acquire(memlock);
    return 0;
}

int __plug_memalloc_unlock()
{
    lock_release(memlock);
    return 0;
}

void *__plug_memalloc_alloc(uint size)
{
    uint addr = process_alloc(size);
    return (void *)addr;
}

int __plug_memalloc_free(void *memory, uint size)
{
    return process_free((unsigned int)memory, size);
}

/* --- Threads -------------------------------------------------------------- */

int __plug_thread_this(void)
{
    return __syscall(SYS_THREAD_SELF, 0, 0, 0, 0, 0);
}

/* --- Processes ------------------------------------------------------------ */

int __plug_process_this(void)
{
    return __syscall(SYS_PROCESS_SELF, 0, 0, 0, 0, 0);
}

bool __plug_process_set_workdir(const char *path)
{
    return __syscall(SYS_PROCESS_SET_CWD, (int)path, 0, 0, 0, 0);
}

void __plug_process_get_workdir(const char *path, uint size)
{
    __syscall(SYS_PROCESS_GET_CWD, (int)path, size, 0, 0, 0);
}