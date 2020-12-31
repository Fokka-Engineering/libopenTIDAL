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

#include "../../external/cJSON.h"
#include "../../openTIDAL.h"

#ifndef PARSEMODULES__h
#define PARSEMODULES__h

struct moduleStruct;

/* TIDALs page endpoints (home, explore, videos, mix) have a different json structure.
 * These pages have their content seperated in segments. Each segment has a different type
 * of media. A segment is called a module in the TIDAL API.
 * In this case the ContentArrays inside the openTIDAL_ContentContainer structure are
 * pools. The correct data is accessed by defined offsets. */
int openTIDAL_ParseModules (openTIDAL_ContentContainer *o, cJSON *input_json);

#endif // PARSEMODULES__h
