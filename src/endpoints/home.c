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

#include "../include/parse.h"
#include "../include/handles.h"
#include "../include/openTIDAL.h"

openTIDAL openTIDAL_GetHome()
{
    openTIDAL o;
    char buffer[80];

    openTIDAL_StructInit(&o);
    openTIDAL_StructAlloc(&o, 0);
    openTIDAL_StructAlloc(&o, 1);
    openTIDAL_StructAlloc(&o, 2);
    openTIDAL_StructAlloc(&o, 3);
    openTIDAL_StructAlloc(&o, 4);

    snprintf(buffer, sizeof(buffer), "countryCode=%s&deviceType=%s",
        config.countryCode, "BROWSER");
    
    curl_model req = curl_get("/v1/pages/home/", buffer);

    if (req.status != -1)
    {
        cJSON *input_json = json_parse(req.body);
        if (req.responseCode == 200)
        {
            o.status = 1;
            parse_home(&o, input_json); 
        }
        else
        {
            o.status = parse_status(input_json, req, 0, "Page Home");
        }

        free(req.body);
        o.json = input_json;
        return o;
    }
    else
    {
        o.status = -1;
        free(req.body);
        return o;
    }

    return o;
}
