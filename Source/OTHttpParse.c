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

/* Parse a http request and create the json structure
 */

#include <string.h>

#include "OTHttp.h"
#include "OTJson.h"
#include "openTIDAL.h"

enum OTStatus
OTHttpParseStatus (struct OTHttpContainer *const http)
{
    enum OTStatus status = UNKNOWN;
    if (http->responseCode <= 299 && http->responseCode >= 200)
        {
            status = SUCCESS;
        }
    else if (http->responseCode <= 399 && http->responseCode >= 300)
        {
            status = REDIRECT;
        }
    else if (http->responseCode <= 499 && http->responseCode >= 400)
        {
            switch (http->responseCode)
                {
                case 400:
                    status = BAD_REQUEST;
                    break;
                case 401:
                    status = UNAUTHORISED;
                    break;
                case 404:
                    status = ARTEFACT_NOT_FOUND;
                    break;
                case 412:
                    status = PRECONDITION_FAILED;
                    break;
                default:
                    status = CLIENT_ERROR;
                    break;
                }
        }
    else if (http->responseCode <= 599 && http->responseCode >= 500)
        {
            status = SERVER_ERROR;
        }
    return status;
}

char *
OTHttpParseHeader (char *buffer, char *key)
{
    const char delim[] = "\n";
    char *value = NULL, *token = NULL, *sep = NULL;
    int i;

    token = strtok (buffer, delim);
    while (token != NULL)
        {
            i = 0;
            if (strncmp (token, key, strlen (key)) == 0)
                {
                    while ((sep = strsep (&token, ":")) != NULL)
                        {
                            if (i == 1)
                                value = sep + 1;
                            i += 1;
                        }
                }
            token = strtok (NULL, delim);
        }
    return value;
}
