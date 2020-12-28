#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

int openTIDAL_StringHelper(char **str, char* fmt, ...) __attribute__ ((format (printf,2,3)));

int openTIDAL_StringHelper(char **str, char* fmt, ...){
    va_list argp;
    va_start(argp, fmt);
    char one_char[1];
    int len = vsnprintf(one_char, 1, fmt, argp);
    if (len < 1){
        fprintf(stderr, "An encoding error occurred. Setting the input pointer to NULL.\n");
        *str = NULL;
        va_end(argp);
        return len;
    }
    va_end(argp);

    *str = malloc(len+1);
    if (!str) {
        fprintf(stderr, "Couldn't allocate %i bytes.\n", len+1);
        return -1;
    }
    va_start(argp, fmt);
    vsnprintf(*str, len+1, fmt, argp);
    va_end(argp);
    return len;
}

int main()
{
    char *string = NULL;
    openTIDAL_StringHelper(&string, "Testing new %d", 123);
    printf("%s\n", string);
    return 0;
}

