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

/* openTIDAL service request
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../OTHelper.h"
#include "../OTHttp.h"
#include "../OTJson.h"
#include "../openTIDAL.h"

struct OTContentContainer *
OTServiceRequestStandard (struct OTSessionContainer *session, struct OTHttpContainer *http,
                          void *threadHandle)
{
    int isException = 0;
    struct OTContentContainer *content;
    enum OTTypes containerType = CONTENT_CONTAINER;

    /* Allocate OTContentContainer. Needs to be freed after use! */
    content = OTAllocContainer (containerType);
    if (!content)
        return NULL;
    content->tree = NULL;
    /* Use the threadHandle if not NULL. */
    if (threadHandle)
        http->handle = threadHandle;
    else
        http->handle = session->mainHttpHandle;

    /* Perform http request. */
    OTHttpRequest (session, http);
    if (http->httpOk != -1)
        {
            content->status = OTHttpParseStatus (http);
            content->tree = OTJsonParse (http->response);
            if (!content->tree)
                {
                    isException = 1;
                    goto end;
                }
        }
    else
        content->status = CURL_NOT_OK;
end:
    free (http->response);
    if (isException)
        {
            free (content);
            return NULL;
        }
    return content;
}

struct OTContentStreamContainer *
OTServiceRequestStream (struct OTSessionContainer *session, struct OTHttpContainer *http,
                        void *threadHandle)
{
    int isException = 0;
    struct OTContentStreamContainer *content;
    struct OTJsonContainer *manifestMimeType;
    struct OTJsonContainer *manifest;
    char *manifestMimeTypeString;
    char *manifestEncString;
    char *manifestDecString;
    enum OTTypes containerType = CONTENT_STREAM_CONTAINER;

    /* Allocate OTContentContainer. Needs to be freed after use! */
    content = OTAllocContainer (containerType);
    if (!content)
        return NULL;
    content->manifest = NULL;
    content->tree = NULL;
    /* Use the threadHandle if not NULL. */
    if (threadHandle)
        http->handle = threadHandle;
    else
        http->handle = session->mainHttpHandle;

    /* Perform http request. */
    OTHttpRequest (session, http);
    if (http->httpOk != -1)
        {
            content->status = OTHttpParseStatus (http);
            content->tree = OTJsonParse (http->response);
            if (!content->tree)
                {
                    isException = 1;
                    goto end;
                }

            /* Decode and parse manifest. */
            manifestMimeType = OTJsonGetObjectItem (content->tree, "manifestMimeType");
            manifest = OTJsonGetObjectItem (content->tree, "manifest");
            manifestMimeTypeString = OTJsonGetStringValue (manifestMimeType);
            manifestEncString = OTJsonGetStringValue (manifest);
            if (manifestMimeTypeString)
                {
                    if (strcmp (manifestMimeTypeString, "application/vnd.tidal.bts") == 0
                        || strcmp (manifestMimeTypeString, "application/vnd.tidal.emu") == 0)
                        {
                            manifestDecString = OTStringDecodeBase64 (manifestEncString);
                            if (!manifestDecString)
                                {
                                    isException = 1;
                                    goto end;
                                }
                            content->manifest = OTJsonParse (manifestDecString);
                            free (manifestDecString);
                            if (!content->manifest)
                                {
                                    isException = 1;
                                    goto end;
                                }
                        }
                    else
                        content->status = UNKNOWN_MANIFEST_MIMETYPE;
                }
        }
    else
        content->status = CURL_NOT_OK;
end:
    free (http->response);
    if (isException)
        {
            free (content);
            return NULL;
        }
    return content;
}

/* Free http response after use. */
enum OTStatus
OTServiceRequestSilent (struct OTSessionContainer *session, struct OTHttpContainer *http,
                        void *threadHandle)
{
    int isException = 0;
    enum OTStatus status = UNKNOWN;
    enum OTTypes containerType = CONTENT_CONTAINER;

    /* Use the threadHandle if not NULL. */
    if (threadHandle)
        http->handle = threadHandle;
    else
        http->handle = session->mainHttpHandle;

    /* Perform http request. */
    OTHttpRequest (session, http);
    if (http->httpOk != -1)
        {
            status = OTHttpParseStatus (http);
        }
    else
        status = CURL_NOT_OK;
    return status;
}

