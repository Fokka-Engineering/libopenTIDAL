/*
    Copyright (c) 2020-2021 Hugo Melder and openTIDAL contributors

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

/* String helper functions
 */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "OTBase64.h"
#include "OTHelper.h"

/* Allocates and encodes an ASCII String. Similar to sprintf.
 * Needs to be freed after use! */
int
OTConcatenateString (char **str, char *format, ...)
{
    va_list argp;
    va_start (argp, format);
    char one_char[1];
    int len = vsnprintf (one_char, 1, format, argp);
    if (len < 1)
        {
            *str = NULL;
            va_end (argp);
            return len;
        }
    va_end (argp);

    *str = malloc (len + 1);
    if (!str)
        return -1;

    va_start (argp, format);
    vsnprintf (*str, len + 1, format, argp);
    va_end (argp);
    return len;
}

/* Iterates over the array and allocates its content in one string. */
int
OTArrayToString (char **str, const char **array, const int length)
{
    size_t memsize = 0;
    int i;
    /* Pointer to new allocated string. */
    char *ptr;

    /* Iterates over the array and increments the calculated size. */
    for (i = 0; i < length; i++)
        /* Length of ASCII string + separator ", ". */
        memsize += strlen (array[i]) + 2;
    memsize += 1;

    ptr = malloc (memsize);
    if (!ptr)
        return -1;

    strcpy (ptr, "");
    for (i = 0; i < length; i++)
        {
            strcat (ptr, array[i]);
            if (i != length - 1)
                strcat (ptr, ", ");
        }
    *str = ptr;
    return 0;
}

char *
OTStringDecodeBase64 (const char *const enc)
{
    size_t size = OTBase64DecodeLen (enc);
    char *dec = malloc (size);
    if (!dec)
        return NULL;
    OTBase64Decode (dec, enc);
    return dec;
}
