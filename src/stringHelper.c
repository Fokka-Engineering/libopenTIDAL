/*
    Copyright (c) 2020 Hugo Melder and openTIDAL contributors

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/

#include "include/helper.h"
#include "include/openTIDAL.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

int
openTIDAL_StringHelper (char **str, char *format, ...)
{
    va_list argp;
    va_start (argp, format);
    char one_char[1];
    int len = vsnprintf (one_char, 1, format, argp);
    if (len < 1) {
        openTIDAL_VerboseHelper ("StringHelper",
                                 "An encoding error occurred. Setting pointer to NULL", 1);
        *str = NULL;
        va_end (argp);
        return len;
    }
    va_end (argp);

    *str = malloc (len + 1);
    if (!str) {
        openTIDAL_VerboseHelper ("StringHelper",
                                 "Couldn't allocate encoded string into heap. Return -1", 1);
        return -1;
    }
    va_start (argp, format);
    vsnprintf (*str, len + 1, format, argp);
    va_end (argp);
    return len;
}

