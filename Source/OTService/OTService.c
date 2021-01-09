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

#include <stdlib.h>

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
            content->content = content->tree;
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
