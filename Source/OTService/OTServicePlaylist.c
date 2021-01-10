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

/* openTIDAL playlist manipulation service
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../OTHelper.h"
#include "../OTHttp.h"
#include "../openTIDAL.h"
#include "OTService.h"

struct OTContentContainer *
OTServiceCreatePlaylist (struct OTSessionContainer *session, char *title, char *description,
                         void *threadHandle)
{
    int isException = 0;
    struct OTHttpContainer http;
    struct OTContentContainer *content = NULL;
    char *encTitle = NULL;
    char *encDescription = NULL;
    enum OTHttpTypes reqType = POST;

    /* UrlEncode strings. */
    encTitle = OTUrlEncode (title);
    encDescription = OTUrlEncode (description);
    if (!encTitle || !encDescription)
        {
            isException = 1;
            goto end;
        }
    /* Initialise values in structure. */
    OTHttpContainerInit (&http);
    http.type = &reqType;
    OTConcatenateString (&http.endpoint, "/v1/users/%s/playlists", session->userId);
    OTConcatenateString (&http.parameter, "countryCode=%s", session->countryCode);
    OTConcatenateString (&http.postData, "title=%s&description=%s", encTitle, encDescription);
    if (!http.parameter || !http.endpoint || !http.postData)
        {
            isException = 1;
            goto end;
        }

    content = OTServiceRequestStandard (session, &http, threadHandle);
end:
    free (http.endpoint);
    free (http.parameter);
    free (http.postData);
    free (encTitle);
    free (encDescription);
    return content;
}

/* Free returned value after use. */
char *
OTServiceGetPlaylistEntityTag (struct OTSessionContainer *session, const char *const id,
                               void *threadHandle)
{
    int isException = 0;
    struct OTHttpContainer http;
    char *value = NULL;
    enum OTHttpTypes reqType = HEAD;
    enum OTStatus status = UNKNOWN;

    /* Initialise values in structure. */
    OTHttpContainerInit (&http);
    http.type = &reqType;
    OTConcatenateString (&http.endpoint, "/v1/playlists/%s", id);
    OTConcatenateString (&http.parameter, "countryCode=%s", session->countryCode);
    if (!http.parameter || !http.endpoint)
        {
            isException = 1;
            goto end;
        }

    status = OTServiceRequestSilent (session, &http, threadHandle);
    if (status == SUCCESS)
        {
            char *tmp = OTHttpParseHeader (http.response, "etag");
            if (tmp)
                {
                    if (tmp[strlen (tmp) - 1] == '\x20')
                        tmp[strlen (tmp) - 1] = '\0';
                    value = strdup (tmp);
                }
        }
end:
    free (http.endpoint);
    free (http.parameter);
    free (http.response);
    return value;
}
