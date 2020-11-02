#include <libsystem/core/CString.h>
#include <libsystem/core/CType.h>
#include <libsystem/core/Scanf.h>
#include <libsystem/utils/Lexer.h>
#include <libsystem/utils/NumberParser.h>


#include <libsystem/Common.h>

int __scanf(const char * const fmt, va_list va)
{
    va_list args_copy;
    va_copy(args_copy, va);

    void *const ptr = va_arg(args_copy, void *);

    printf("Format is: %s\nThe first arg is: %p", fmt, ptr);

    return 1;
}