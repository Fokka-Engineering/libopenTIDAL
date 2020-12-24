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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/parse.h"
#include "../include/handles.h"
#include "../include/openTIDAL.h"

/* Search */

openTIDAL openTIDAL_SearchAll(char *term, const int limit)
{
    openTIDAL o;
    char *encodedTerm = url_encode(term);
    char *endpoint = "/v1/search/";
    size_t mallocSize = 0;
    char *baseparams = NULL;
 
    openTIDAL_StructInit(&o);
    openTIDAL_StructAlloc(&o, 0);
    openTIDAL_StructAlloc(&o, 1);
    openTIDAL_StructAlloc(&o, 2);
    openTIDAL_StructAlloc(&o, 3);

    mallocSize = strlen(encodedTerm) + 40;
    baseparams = malloc(mallocSize); 
    snprintf(baseparams, mallocSize, "countryCode=%s&limit=%d&query=%s", config.countryCode,
                        limit, encodedTerm);
    curl_model req = curl_get(endpoint, baseparams);
    free(encodedTerm);
    free(baseparams);

    if (req.status != -1)
    {
        cJSON *input_json = json_parse(req.body);
        if (req.responseCode == 200)
        {
            parse_search(&o, input_json);
            o.status = 1;
        }
        else
        {
            o.status = parse_status(input_json, req, 0, "Search");
        }
        o.json = input_json;
        free(req.body);
        return o;
    }
    else
    {
        o.status = -1;
        free(req.body);
        return o;
    }
}
