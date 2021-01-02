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

#include "parse.h"
#include <stdio.h>
#include <string.h>

/* Parse libcURL allocated http headerdata callback.
 * HTTP Header Scheme:
 * {key}: {value}
 *
 * Usage:
 * Specify the buffer and key values. If the key is equal to a key in the buffer, the Value pointer
 * will point to the value. If the comparison results in no match, the Value pointer is NULL. */
void
openTIDAL_ParseHeaderKeyHelper (char *buffer, char *key, char **Value)
{
    const char delim[] = "\n";
    char *token, *sep;
    sep = NULL;
    int i;

    token = strtok (buffer, delim);
    while (token != NULL) {
        i = 0;
        if (strncmp (token, key, strlen (key)) == 0) {
            while ((sep = strsep (&token, ":")) != NULL) {
                if (i == 1) *Value = sep + 1;
                i += 1;
            }
        }
        token = strtok (NULL, delim);
    }
}
