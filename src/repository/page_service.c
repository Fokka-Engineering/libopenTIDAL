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

#include "../helper/helper.h"
#include "../helper/struct_helper.h"
#include "../http_connector.h"
#include "../openTIDAL.h"
#include "../parse/modules/modules.h"
#include "../parse/parse.h"

#include <stdio.h>

openTIDAL_ContentContainer *
openTIDAL_GetHome (openTIDAL_SessionContainer *session)
{
    openTIDAL_ContentContainer *o = NULL;
    openTIDAL_CurlContainer curl;
    int status = 0;

    openTIDAL_CurlModelInit (&curl);

    status = openTIDAL_StructMainAlloc (&o);
    if (status == -1) return NULL;
    status = openTIDAL_StructInit (o);
    if (status == -1) goto end;
    status = openTIDAL_StructAlloc (o, 0);
    if (status == -1) goto end;
    status = openTIDAL_StructAlloc (o, 1);
    if (status == -1) goto end;
    status = openTIDAL_StructAlloc (o, 2);
    if (status == -1) goto end;
    status = openTIDAL_StructAlloc (o, 3);
    if (status == -1) goto end;
    status = openTIDAL_StructAlloc (o, 4);
    if (status == -1) goto end;

    openTIDAL_StringHelper (&curl.parameter, "countryCode=%s&deviceType=BROWSER",
                            session->countryCode);
    if (!curl.parameter) {
        status = -14;
        goto end;
    }

    openTIDAL_CurlRequest (session, &curl, "GET", "/v1/pages/home/", curl.parameter, NULL, 0, 0);
    if (curl.status != -1) {
        o->json = openTIDAL_cJSONParseHelper (curl.body);
        if (!o->json) {
            status = -14;
            goto end;
        }

        if (curl.responseCode == 200) {
            o->status = 1;
            status = openTIDAL_ParseModules (o, (cJSON *)o->json);
        }
        else {
            o->status = parse_status ((cJSON *)o->json, &curl, 0, "Page Home");
        }
    }
end:
    if (status == -1) o->status = -14;
    openTIDAL_CurlRequestCleanup (&curl);
    return o;
}
