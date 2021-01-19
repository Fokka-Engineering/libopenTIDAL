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

/* openTIDAL standard get service.
 */

#include <stdlib.h>
#include <string.h>

#include "../OTHelper.h"
#include "../OTHttp.h"
#include "../openTIDAL.h"
#include "OTService.h"

/* Request v1 GET metadata. */
struct OTContentContainer *
OTServiceGetStandard (struct OTSessionContainer *session, const char *const prefix,
                      const char *const suffix, const char *const id, const int limit,
                      const int offset, void *threadHandle)
{
    int isException = 0;
    struct OTHttpContainer http;
    struct OTContentContainer *content = NULL;
    enum OTHttpTypes reqType = GET;

    /* Initialise values in structure. */
    OTHttpContainerInit (&http);
    http.type = &reqType;
    if (suffix)
        OTConcatenateString (&http.endpoint, "/v1/%s/%s/%s", prefix, id, suffix);
    else
        OTConcatenateString (&http.endpoint, "/v1/%s/%s", prefix, id);
    OTConcatenateString (&http.parameter, "countryCode=%s&limit=%d&offset=%d", session->countryCode,
                         limit, offset);
    if (!http.parameter || !http.endpoint)
        {
            isException = 1;
            goto end;
        }

    content = OTServiceRequestStandard (session, &http, threadHandle);
end:
    free (http.endpoint);
    free (http.parameter);
    return content;
}

struct OTContentContainer *
OTServiceGetPage (struct OTSessionContainer *session, const char *const suffix,
                  const char *const id, const int limit, const int offset, void *threadHandle)
{
    int isException = 0;
    struct OTHttpContainer http;
    struct OTContentContainer *content = NULL;
    enum OTHttpTypes reqType = GET;

    /* Initialise values in structure. */
    OTHttpContainerInit (&http);
    http.type = &reqType;
    OTConcatenateString (&http.endpoint, "/v1/pages/%s", suffix);
    /* Check for suffix. */
    if (strcmp (suffix, "album") == 0)
        OTConcatenateString (&http.parameter, "countryCode=%s&locale=%s&deviceType=%s&albumId=%s",
                             session->countryCode, session->locale, session->deviceType, id);
    else if (strcmp (suffix, "artist") == 0)
        OTConcatenateString (&http.parameter, "countryCode=%s&locale=%s&deviceType=%s&artistId=%s",
                             session->countryCode, session->locale, session->deviceType, id);
    else if (strcmp (suffix, "mix") == 0)
        OTConcatenateString (&http.parameter, "countryCode=%s&locale=%s&deviceType=%s&mixId=%s",
                             session->countryCode, session->locale, session->deviceType, id);
    else
        OTConcatenateString (
            &http.parameter, "countryCode=%s&locale=%s&deviceType=%s&limit=%d&offset=%d",
            session->countryCode, session->locale, session->deviceType, limit, offset);
    if (!http.parameter || !http.endpoint)
        {
            isException = 1;
            goto end;
        }

    content = OTServiceRequestStandard (session, &http, threadHandle);
end:
    free (http.endpoint);
    free (http.parameter);
    return content;
}

struct OTContentContainer *
OTServiceSearch (struct OTSessionContainer *session, const char *const suffix, char *query,
                 const int limit, const int offset, void *threadHandle)
{
    int isException = 0;
    struct OTHttpContainer http;
    struct OTContentContainer *content = NULL;
    enum OTHttpTypes reqType = GET;
    char *enc = NULL;

    /* Initialise values in structure. */
    OTHttpContainerInit (&http);
    http.type = &reqType;

    /* UrlEncode string. */
    enc = OTUrlEncode (query);
    if (!enc)
        {
            isException = 1;
            goto end;
        }

    /* If NULL: Search all. */
    if (suffix)
        OTConcatenateString (&http.endpoint, "/v1/search/%s", suffix);
    else
        OTConcatenateString (&http.endpoint, "/v1/search");
    OTConcatenateString (&http.parameter, "countryCode=%s&limit=%d&offset=%d&query=%s",
                         session->countryCode, limit, offset, enc);
    if (!http.parameter || !http.endpoint)
        {
            isException = 1;
            goto end;
        }

    content = OTServiceRequestStandard (session, &http, threadHandle);
end:
    free (http.endpoint);
    free (http.parameter);
    free (enc);
    return content;
}

struct OTContentStreamContainer *
OTServiceGetStream (struct OTSessionContainer *session, const char *const prefix,
                    const char *const id, const int isPreview, void *threadHandle)
{
    int isException = 0;
    struct OTHttpContainer http;
    struct OTContentStreamContainer *content = NULL;
    enum OTHttpTypes reqType = GET;

    /* Initialise values in structure. */
    OTHttpContainerInit (&http);
    http.type = &reqType;

    /* Check for preview-mode. */
    if (!isPreview)
        OTConcatenateString (&http.endpoint, "/v1/%s/%s/playbackinfopostpaywall", prefix, id);
    else
        OTConcatenateString (&http.endpoint, "/v1/%s/%s/playbackinfoprepaywall", prefix, id);

    if (strcmp (prefix, "videos") == 0)
        OTConcatenateString (
            &http.parameter,
            "countryCode=%s&videoquality=%s&playbackmode=STREAM&assetpresentation=FULL",
            session->countryCode, session->videoQuality);
    else
        OTConcatenateString (
            &http.parameter,
            "countryCode=%s&audioquality=%s&playbackmode=STREAM&assetpresentation=FULL",
            session->countryCode, session->audioQuality);
    if (!http.parameter || !http.endpoint)
        {
            isException = 1;
            goto end;
        }

    content = OTServiceRequestStream (session, &http, threadHandle);
end:
    free (http.endpoint);
    free (http.parameter);
    return content;
}

