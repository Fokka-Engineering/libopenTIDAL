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

#include "../../include/openTIDAL.h"
#include <stdio.h>
#include <stdlib.h>

static int init_capacity = 10;

int
openTIDAL_ParseModuleAllocHelper (openTIDAL_ContentContainer *o)
{
    o->modules.arrayCapacity = init_capacity;
    o->modules.arraySize = 0;

    o->modules.moduleType = malloc (sizeof (char *) * init_capacity);
    o->modules.arrayType = malloc (sizeof (int) * init_capacity);
    o->modules.offset = malloc (sizeof (int) * init_capacity);
    o->modules.total = malloc (sizeof (int) * init_capacity);
    if (!o->modules.moduleType || !o->modules.arrayType || !o->modules.offset || !o->modules.total)
        return -1;
    return 0;
}

static int
openTIDAL_ParseModuleResize (openTIDAL_ContentContainer *o, int capacity)
{
    o->modules.moduleType = realloc (o->modules.moduleType, sizeof (char *) * capacity);
    o->modules.arrayType = realloc (o->modules.arrayType, sizeof (int) * capacity);
    o->modules.offset = realloc (o->modules.offset, sizeof (int) * capacity);
    o->modules.total = realloc (o->modules.total, sizeof (int) * capacity);
    if (!o->modules.moduleType || !o->modules.arrayType || !o->modules.offset || !o->modules.total)
        return -1;
    return 0;
}

int
openTIDAL_ParseModuleAdd (openTIDAL_ContentContainer *o, int index, char *str, int number)
{
    int status;

    if (o->modules.arraySize == o->modules.arrayCapacity) {
        o->modules.arrayCapacity = o->modules.arrayCapacity * 2;
        status = openTIDAL_ParseModuleResize (o, o->modules.arrayCapacity);
        if (status == -1) return -1;
    }

    switch (index) {
    case 0: // moduleType
        if (str) o->modules.moduleType[o->modules.arraySize] = str;
        break;
    case 1: // arrayType
        o->modules.arrayType[o->modules.arraySize] = number;
        break;
    case 2: // offset
        o->modules.offset[o->modules.arraySize] = number;
        break;
    case 3: // total
        o->modules.total[o->modules.arraySize] = number;
        break;
    }
    return 0;
}
