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

/* openTIDAL feed service (v2 api)
 */

#include <stdlib.h>
#include <string.h>

#include "../OTHelper.h"
#include "../OTHttp.h"
#include "../openTIDAL.h"
#include "OTService.h"

struct OTContentContainer *
OTServiceGetFeedActivities (struct OTSessionContainer *session, void *threadHandle)
{
    int isException = 0;
    struct OTHttpContainer http;
    struct OTContentContainer *content = NULL;
    enum OTHttpTypes reqType = GET;

    /* Initialise values in structure. */
    OTHttpContainerInit (&http);

    if (session->restrictedMode)
        {
            isException = 1;
            goto end;
        }

    http.type = &reqType;
    http.endpoint = "/v2/feed/activities";
    OTConcatenateString (&http.parameter, "countryCode=%s&locale=%s&userId=%s",
                         session->countryCode, session->locale, session->userId);
    if (!http.parameter)
        {
            isException = 1;
            goto end;
        }
    content = OTServiceRequestStandard (session, &http, threadHandle);
end:
    free (http.parameter);
    return content;
}

struct OTContentContainer *
OTServiceGetFeedActivityUnseenExists (struct OTSessionContainer *session, void *threadHandle)
{
    int isException = 0;
    struct OTHttpContainer http;
    struct OTContentContainer *content = NULL;
    enum OTHttpTypes reqType = GET;

    /* Initialise values in structure. */
    OTHttpContainerInit (&http);

    if (session->restrictedMode)
        {
            isException = 1;
            goto end;
        }

    http.type = &reqType;
    http.endpoint = "/v2/feed/activities/unseen/exists";
    OTConcatenateString (&http.parameter, "countryCode=%s&locale=%s&userId=%s",
                         session->countryCode, session->locale, session->userId);
    if (!http.parameter)
        {
            isException = 1;
            goto end;
        }

    content = OTServiceRequestStandard (session, &http, threadHandle);
end:
    free (http.parameter);
    return content;
}

enum OTStatus
OTServiceFeedActivitySeen (struct OTSessionContainer *session, void *threadHandle)
{
    int isException = 0;
    struct OTHttpContainer http;
    enum OTHttpTypes reqType = PUT;
    enum OTStatus status = UNKNOWN;

    /* Initialise values in structure. */
    OTHttpContainerInit (&http);

    if (session->restrictedMode)
        return status;

    http.type = &reqType;
    http.isDummy = 1;
    http.endpoint = "/v2/feed/activities/seen";
    OTConcatenateString (&http.parameter, "userId=%s&countryCode=%s", session->userId,
                         session->countryCode);
    if (!http.parameter)
        {
            isException = 1;
            goto end;
        }

    status = OTServiceRequestSilent (session, &http, threadHandle);
end:
    free (http.parameter);
    return status;
}
