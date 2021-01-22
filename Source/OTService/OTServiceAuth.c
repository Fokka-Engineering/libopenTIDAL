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

/* openTIDAL auth service.
 */

#include <stdlib.h>

#include "../OTHelper.h"
#include "../OTHttp.h"
#include "../openTIDAL.h"
#include "OTService.h"

/* This service function requests a userCode and deviceCode pair from the TIDAL auth server.
 * It is part of the oAuth2 device authorisation grant (RFC 8628). */
struct OTContentContainer *
OTServiceGetDeviceCode (struct OTSessionContainer *session, void *threadHandle)
{
    int isException = 0;
    struct OTHttpContainer http;
    struct OTContentContainer *content = NULL;
    enum OTHttpTypes reqType = POST;

    /* Initialise values in structure. */
    OTHttpContainerInit (&http);
    http.endpoint = "/v1/oauth2/device_authorization";
    http.type = &reqType;
    http.isAuthRequest = 1;
    if (session->clientId)
        OTConcatenateString (&http.postData, "client_id=%s&scope=%s", session->clientId,
                             session->scopes);
    if (!http.postData)
        {
            isException = 1;
            goto end;
        }

    content = OTServiceRequestStandard (session, &http, threadHandle);
end:
    free (http.postData);
    return content;
}

/* OTServiceGetBearerToken requests a accessToken and refreshToken pair. */
struct OTContentContainer *
OTServiceGetBearerToken (struct OTSessionContainer *session, const char *const deviceCode,
                         void *threadHandle)
{
    int isException = 0;
    struct OTHttpContainer http;
    struct OTContentContainer *content = NULL;
    enum OTHttpTypes reqType = POST;
    const char *grant_type = "urn:ietf:params:oauth:grant-type:device_code";

    /* Initialise values in structure. */
    OTHttpContainerInit (&http);
    http.endpoint = "/v1/oauth2/token";
    http.type = &reqType;
    http.isAuthRequest = 1;
    if (session->clientId)
        OTConcatenateString (&http.postData, "client_id=%s&scope=%s&grant_type=%s&device_code=%s",
                             session->clientId, session->scopes, grant_type, deviceCode);
    if (!http.postData)
        {
            isException = 1;
            goto end;
        }

    content = OTServiceRequestStandard (session, &http, threadHandle);
end:
    free (http.postData);
    return content;
}

/* OTServiceRefreshBearerToken refreshes the accessToken with the refreshToken. */
struct OTContentContainer *
OTServiceRefreshBearerToken (struct OTSessionContainer *session, const char *const refreshToken,
                             void *threadHandle)
{
    int isException = 0;
    struct OTHttpContainer http;
    struct OTContentContainer *content = NULL;
    enum OTHttpTypes reqType = POST;
    const char *grant_type = "refresh_token";

    /* Initialise values in structure. */
    OTHttpContainerInit (&http);
    http.endpoint = "/v1/oauth2/token";
    http.type = &reqType;
    http.isAuthRequest = 1;
    if (session->clientId)
        OTConcatenateString (&http.postData, "client_id=%s&scope=%s&grant_type=%s&refresh_token=%s",
                             session->clientId, session->scopes, grant_type, refreshToken);
    if (!http.postData)
        {
            isException = 1;
            goto end;
        }

    content = OTServiceRequestStandard (session, &http, threadHandle);
end:
    free (http.postData);
    return content;
}

enum OTStatus
OTServiceLogout (struct OTSessionContainer *session, void *threadHandle)
{
    int isException = 0;
    struct OTHttpContainer http;
    enum OTHttpTypes reqType = POST;
    enum OTStatus status = UNKNOWN;

    if (session->restrictedMode)
        return status;

    /* Initialise values in structure. */
    OTHttpContainerInit (&http);
    http.type = &reqType;
    http.isDummy = 1;
    http.isAuthRequest = 1;
    http.endpoint = "/v1/logout";

    status = OTServiceRequestSilent (session, &http, threadHandle);
    return status;
}
