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

#include "../include/handles.h"
#include "../include/helper.h"
#include "../include/openTIDAL.h"
#include "../include/parse.h"
#include "../include/struct.h"

#include <stdio.h>

openTIDAL_ContentContainer *
openTIDAL_GetHome (openTIDAL_SessionContainer *session)
{
    openTIDAL_ContentContainer *o = NULL;
    openTIDAL_CurlContainer curl;

    openTIDAL_StructMainAlloc (&o);
    openTIDAL_StructInit (o);
    openTIDAL_StructAlloc (o, 0);
    openTIDAL_StructAlloc (o, 1);
    openTIDAL_StructAlloc (o, 2);
    openTIDAL_StructAlloc (o, 3);
    openTIDAL_StructAlloc (o, 4);

    openTIDAL_StringHelper (&curl.parameter, "countryCode=%s&deviceType=BROWSER",
                            session->countryCode);
    if (!curl.parameter) {
        o->status = -14;
        return o;
    }

    openTIDAL_CurlRequest (session, &curl, "GET", "/v1/pages/home/", curl.parameter, NULL, 0, 0);
    if (curl.status != -1) {
        o->json = openTIDAL_cJSONParseHelper (curl.body);
        if (!o->json) {
            o->status = -14;
            return o;
        }

        if (curl.responseCode == 200) {
            o->status = 1;
            parse_home (o, (cJSON *)o->json);
        }
        else {
            o->status = parse_status ((cJSON *)o->json, &curl, 0, "Page Home");
        }
    }
    openTIDAL_CurlRequestCleanup (&curl);
    return o;
}
