#include <errno.h>
#include <stddef.h>
#include <sys/stat.h>

extern char _end;

__attribute__((weak)) int autonav_platform_write(const char *data, int length)
{
    (void)data;
    return length;
}

int _write(int file, char *data, int length)
{
    (void)file;
    return autonav_platform_write(data, length);
}

int _read(int file, char *data, int length)
{
    (void)file;
    (void)data;
    (void)length;
    return 0;
}

int _close(int file)
{
    (void)file;
    return -1;
}

int _fstat(int file, struct stat *status)
{
    (void)file;
    status->st_mode = S_IFCHR;
    return 0;
}

int _isatty(int file)
{
    (void)file;
    return 1;
}

int _lseek(int file, int offset, int origin)
{
    (void)file;
    (void)offset;
    (void)origin;
    return 0;
}

int _getpid(void)
{
    return 1;
}

int _kill(int pid, int signal)
{
    (void)pid;
    (void)signal;
    errno = EINVAL;
    return -1;
}

void *_sbrk(ptrdiff_t increment)
{
    static char *heap_end;
    register char *stack_pointer __asm("sp");
    char *previous;

    if (heap_end == NULL)
    {
        heap_end = &_end;
    }

    previous = heap_end;
    if ((heap_end + increment) >= stack_pointer)
    {
        errno = ENOMEM;
        return (void *)-1;
    }

    heap_end += increment;
    return previous;
}

void _exit(int status)
{
    (void)status;
    for (;;)
    {
    }
}

