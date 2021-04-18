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

/* openTIDAL libcurl http helper
 */

#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "OTHelper.h"
#include "OTHttp.h"
#include "openTIDAL.h"

/* libcurl callback and writedata structure. */
struct OTHttpMemory
{
    char *memory;
    size_t size;
};

/* libcurl callback */
static size_t
OTHttpCallbackFunction (void *data, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    struct OTHttpMemory *mem = (struct OTHttpMemory *)userp;

    char *ptr = realloc (mem->memory, mem->size + realsize + 1);
    if (ptr == NULL) return 0;

    mem->memory = ptr;
    memcpy (&(mem->memory[mem->size]), data, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

/* libcurl callback dummy skips the reallocation process. */
static size_t
OTHttpCallBackDummyFunction (void *data, size_t size, size_t nmemb, void *userp)
{
    return size * nmemb;
}

void *
OTHttpThreadHandleCreate (void)
{
    void *handle = NULL;
    handle = curl_easy_init ();
    return handle;
}

void
OTHttpThreadHandleCleanup (void *handle)
{
    curl_easy_cleanup ((CURL *)handle);
}

/* Initialise OTHttpContainer structure. */
void
OTHttpContainerInit (struct OTHttpContainer *const http)
{
    http->handle = NULL;
    http->httpOk = -1;
    http->isAuthRequest = 0;
    http->isDummy = 0;
    http->responseCode = 0;
    http->entityTagHeader = NULL;
    http->response = NULL;
    http->endpoint = NULL;
    http->parameter = NULL;
    http->postData = NULL;
}

/* Concatenate libcurl http header ASCII String. */
static char *
OTHttpAuthHeader (const struct OTSessionContainer *const session,
                  struct OTHttpContainer *const http)
{
    char *string = NULL;
    if (!session->restrictedMode)
        {
            if (session->accessToken)
                OTConcatenateString (&string, "Authorization: Bearer %s", session->accessToken);
        }
    else
        OTConcatenateString (&string, "X-Tidal-Token: %s", session->clientId);
    return string;
}

/* Concatenate url ASCII String. */
static char *
OTHttpUrl (const struct OTSessionContainer *const session, struct OTHttpContainer *const http)
{
    char *string = NULL;
    char *base = NULL;
    if (http->endpoint)
        {
            if (!http->isAuthRequest)
                base = session->baseUrl;
            else
                base = session->authUrl;

            if (http->parameter)
                OTConcatenateString (&string, "%s%s?%s", base, http->endpoint, http->parameter);
            else
                OTConcatenateString (&string, "%s%s", base, http->endpoint);
        }
    return string;
}

/* Perform a HTTP(s) request using the libcurl easy api. Configure values with the OTHttpContainer.
 * If successful returns an allocated response. Needs to be deallocated after use!
 */
void
OTHttpRequest (struct OTSessionContainer *const session, struct OTHttpContainer *const http)
{
    /* libcurl header list. */
    struct curl_slist *chunk = NULL;
    struct OTHttpMemory memchunk;
    memchunk.memory = NULL;
    memchunk.size = 0;
    char *url = NULL;
    char *authHeader = NULL;
    int isHeadRequest = 0;

    if (!session->clientId)
        {
            if (session->verboseMode)
                fprintf (stderr, "* ClientId or ClientSecret ASCII string is not allocated.");
            goto end;
        }

    if (session->verboseMode) fprintf (stderr, "* Check if HTTP-HANDLE is initialised...");
    /* Check if allocation of handle failed */
    if (!http->handle) goto end;
    CURLcode res;
    if (session->verboseMode) fprintf (stderr, "OK\n");

    /* Check if handle is the mainHttpHandle.
     * Only the mainHttpHandle should perform oAuth refresh requests to
     * prevent multiple calls. */
    if (session->mainHttpHandle == http->handle && !http->isAuthRequest)
        {
            /* Do TIDAL Session refresh check. */
            if (session->verboseMode >= 1)
                fprintf (stderr, "* Check for expired OAuth2 accessToken timestamp...\n");
            OTSessionRefresh (session);
        }
    /* Concatenate Url & AuthHeader. */
    url = OTHttpUrl (session, http);
    authHeader = OTHttpAuthHeader (session, http);
    if (!url || !authHeader) goto end;
    if (!http->isDummy) /* Allocate memory buffer to grow it later. */
        memchunk.memory = malloc (1);
    /* libcurl doesn't like NULL */
    if (!http->postData) http->postData = "";

    if (session->verboseMode) fprintf (stderr, "* Begin CURLOPT configuration...\n");

    /* Begin curl_easy_opt configuration. */
    curl_easy_setopt (http->handle, CURLOPT_URL, url);
    curl_easy_setopt (http->handle, CURLOPT_NOBODY, 0L);
    curl_easy_setopt (http->handle, CURLOPT_HEADERFUNCTION, NULL);
    curl_easy_setopt (http->handle, CURLOPT_HEADERDATA, NULL);
    curl_easy_setopt (http->handle, CURLOPT_CUSTOMREQUEST, NULL);
    /* Set request specific options. */
    switch (*http->type)
        {
        case GET:
            curl_easy_setopt (http->handle, CURLOPT_HTTPGET, 1L);
            break;
        case POST:
            curl_easy_setopt (http->handle, CURLOPT_HTTPPOST, 1L);
            curl_easy_setopt (http->handle, CURLOPT_POSTFIELDS, http->postData);
            break;
        case DELETE:
            curl_easy_setopt (http->handle, CURLOPT_CUSTOMREQUEST, "DELETE");
            curl_easy_setopt (http->handle, CURLOPT_POSTFIELDS, http->postData);
            break;
        case PUT:
            curl_easy_setopt (http->handle, CURLOPT_CUSTOMREQUEST, "PUT");
            curl_easy_setopt (http->handle, CURLOPT_POSTFIELDS, http->postData);
            break;
        case HEAD:
            curl_easy_setopt (http->handle, CURLOPT_NOBODY, 1L);
            curl_easy_setopt (http->handle, CURLOPT_HEADERFUNCTION, OTHttpCallbackFunction);
            curl_easy_setopt (http->handle, CURLOPT_HEADERDATA, &memchunk);
            isHeadRequest = 1;
            break;
        }
    /* Standard WriteFunction/Data callback. */
    if (!http->isDummy && !isHeadRequest)
        {
            if (session->verboseMode) fprintf (stderr, "* Enabled CURLOPT_WRITEDATA. \n");
            curl_easy_setopt (http->handle, CURLOPT_WRITEFUNCTION, OTHttpCallbackFunction);
            curl_easy_setopt (http->handle, CURLOPT_WRITEDATA, &memchunk);
        }
    else
        curl_easy_setopt (http->handle, CURLOPT_WRITEFUNCTION, OTHttpCallBackDummyFunction);
    /* clientId & clientSecret authorisation. */
    if (http->isAuthRequest)
        {
            curl_easy_setopt (http->handle, CURLOPT_USERNAME, session->clientId);
            if (!session->clientSecret) goto end;
            curl_easy_setopt (http->handle, CURLOPT_PASSWORD, session->clientSecret);
        }
    else
        {
            chunk = curl_slist_append (chunk, authHeader);
            if (http->entityTagHeader) chunk = curl_slist_append (chunk, http->entityTagHeader);
            curl_easy_setopt (http->handle, CURLOPT_HTTPHEADER, chunk);
        }
    if (session->verboseMode) curl_easy_setopt (http->handle, CURLOPT_VERBOSE, 1L);

    /* Perform request. */
    if (session->verboseMode)
        fprintf (stderr, "* End CURLOPT configuration\n* Call curl_easy_perform...\n");

    res = curl_easy_perform (http->handle);
    if (res == CURLE_OK)
        {
            http->httpOk = 0;
            long http_code = 0;
            curl_easy_getinfo (http->handle, CURLINFO_RESPONSE_CODE, &http_code);
            http->responseCode = http_code;
        }

    if (session->verboseMode) fprintf (stderr, "* Call curl_slist_free_all to free chunk\n");
    curl_slist_free_all (chunk);
    http->response = memchunk.memory;
end:
    if (session->verboseMode) fprintf (stderr, "* Free concatenated url and authHeader\n");
    free (url);
    free (authHeader);
}
