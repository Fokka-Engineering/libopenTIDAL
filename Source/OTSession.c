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

/* openTIDAL session helper
 */

#include "OTHelper.h"
#include "OTJson.h"
#include "OTPersistent.h"
#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Predeclare function prototypes. */
static void OTSessionContainerInit (struct OTSessionContainer *session);

/* Allocate a session container and initialise its values. */
struct OTSessionContainer *
OTSessionInit (void)
{
    struct OTSessionContainer *ptr;
    enum OTTypes type;

    type = SESSION_CONTAINER;
    ptr = OTAllocContainer (type);
    if (!ptr)
        return NULL;
    OTSessionContainerInit (ptr);
    /* One time libcurl global init. */
    curl_global_init (CURL_GLOBAL_ALL);
    ptr->mainHttpHandle = curl_easy_init ();
    return ptr;
}

/* Initialise session container values. */
static void
OTSessionContainerInit (struct OTSessionContainer *const session)
{
    session->persistentFileLocation = NULL;
    session->accessToken = NULL;
    session->refreshToken = NULL;
    session->clientId = NULL;
    session->clientSecret = NULL;
    session->scopes = "r_usr+w_usr";
    session->baseUrl = "https://api.tidal.com";
    session->authUrl = "https://auth.tidal.com";
    session->audioQuality = "LOW";
    session->videoQuality = "LOW";
    session->countryCode = "US";
    session->userId = NULL;
    session->locale = "en_US";
    session->deviceType = "BROWSER";
    session->expiresIn = 0;
    session->timeFrame = 0;
    session->tree = NULL;
    session->renewalTree = NULL;
    session->restrictedMode = 1;
    session->verboseMode = 0;
    session->mainHttpHandle = NULL;
}

/* Allocate the OAuth2 clientId and clientSecret into heap */
int
OTSessionClientPair (struct OTSessionContainer *const session, const char *const clientId,
                     const char *const clientSecret)
{
    session->clientId = strdup (clientId);
    session->clientSecret = strdup (clientSecret);
    if (!session->clientId || !session->clientSecret)
        return -1;
    return 0;
}

/* Allocate persistent file location path ASCII string and
 * opens a file handle. */
int
OTSessionLogin (struct OTSessionContainer *const session, const char *const location)
{
    int status = -1;
    if (location)
        {
            char *stream;

            session->persistentFileLocation = strdup (location);
            if (!session->persistentFileLocation)
                return -1;
            stream = OTPersistentLoad (session);
            if (!stream)
                return -1;
            status = OTPersistentParse (session, stream);
            if (status == 0)
                session->restrictedMode = 0;
            free (stream);
        }
    return status;
}

void
OTSessionVerbose (struct OTSessionContainer *const session, const int enabled)
{
    session->verboseMode = enabled;
}

/* Change audioQuality and videoQuality pointer. */
void
OTSessionChangeQuality (struct OTSessionContainer *const session, enum OTQuality quality)
{
    switch (quality)
        {
        case AUDIO_LOW:
            session->audioQuality = "LOW";
            break;
        case AUDIO_HIGH:
            session->audioQuality = "HIGH";
            break;
        case AUDIO_LOSSLESS:
            session->audioQuality = "LOSSLESS";
            break;
        case AUDIO_HI_RES:
            session->audioQuality = "HI_RES";
            break;
        case VIDEO_AUDIO_ONLY:
            session->videoQuality = "AUDIO_ONLY";
            break;
        case VIDEO_LOW:
            session->videoQuality = "LOW";
            break;
        case VIDEO_MEDIUM:
            session->videoQuality = "MEDIUM";
            break;
        case VIDEO_HIGH:
            session->videoQuality = "HIGH";
            break;
        }
}

int
OTSessionWriteChanges (const struct OTSessionContainer *session)
{
    if (session->persistentFileLocation)
        {
            int status = OTPersistentCreate (session, session->persistentFileLocation);
            return status;
        }
    else
        return -1;
}

void
OTSessionCleanup (struct OTSessionContainer *session)
{
    if (session)
        {
            if (session->verboseMode)
                printf ("* Free OTSessionContainer\n");
            free (session->clientId);
            free (session->clientSecret);
            curl_easy_cleanup (session->mainHttpHandle);
            curl_global_cleanup ();
            enum OTTypes type = SESSION_CONTAINER;
            OTDeallocContainer (session, type);
        }
}
