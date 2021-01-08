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

/* openTIDAL libcurl http helper
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "OTHelper.h"
#include "openTIDAL.h"

/* libcurl callback and writedata structure. */
struct OTHttpMemory
{
    char *memory;
    size_t size;
};

/* libcurl callback */
static size_t
OTHttpCallbackFunction (void *data, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    struct OTHttpMemory *mem = (struct OTHttpMemory *)userp;

    char *ptr = realloc (mem->memory, mem->size + realsize + 1);
    if (ptr == NULL)
        return 0;

    mem->memory = ptr;
    memcpy (&(mem->memory[mem->size]), data, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

/* libcurl callback dummy skips the reallocation process. */
static size_t
OTHttpCallBackDummyFunction (void *data, size_t size, size_t nmemb, void *userp)
{
    return size * nmemb;
}

static char *
OTHttpAuthHeader (const struct OTSessionContainer *const session)
{
    return NULL;
}

static char *
OTHttpUrl (const struct OTSessionContainer *const session)
{
    return NULL;
}
