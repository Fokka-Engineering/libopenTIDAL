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

#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "external/cJSON.h"
#include "helper/helper.h"
#include "http_connector.h"
#include "openTIDAL.h"
#include "parse/parse.h"

struct openTIDAL_CurlMemory {
    char *memory;
    size_t size;
};

static size_t
openTIDAL_CurlCallbackFunction (void *data, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    struct openTIDAL_CurlMemory *mem = (struct openTIDAL_CurlMemory *)userp;

    char *ptr = realloc (mem->memory, mem->size + realsize + 1);
    if (ptr == NULL) {
        openTIDAL_VerboseHelper ("cURL Handle", "Not enough memory (realloc returned NULL)", 1);
        return 0;
    }

    mem->memory = ptr;
    memcpy (&(mem->memory[mem->size]), data, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

static size_t
openTIDAL_CurlCallBackDummyFunction (void *data, size_t size, size_t nmemb, void *userp)
{
    return size * nmemb;
}

static void
openTIDAL_CurlConcatenateAuthHeader (openTIDAL_SessionContainer *session, char **header)
{
    openTIDAL_VerboseHelper ("cURL Handle", "Concatenate AuthHeader", 2);
    const char key[] = "Authorization: ";
    const char keyDemo[] = "X-Tidal-Token: ";
    const char authType[] = "Bearer ";
    char *buffer = NULL;

    if (!session->demoEnabled) {
        buffer = malloc (strlen (key) + strlen (authType) + strlen (session->accessToken) + 3);
        strcpy (buffer, key);
        strcat (buffer, authType);
        strcat (buffer, session->accessToken);
    }
    else {
        buffer = malloc (strlen (keyDemo) + strlen (session->clientId) + 2);
        strcpy (buffer, keyDemo);
        strcat (buffer, session->clientId);
    }

    *header = buffer;
}

static void
openTIDAL_CurlConcatenateUrl (openTIDAL_SessionContainer *session, char **url, const char *endpoint,
                              const char *parameter, const int isAuth)
{
    openTIDAL_VerboseHelper ("cURL Handle", "Concatenate Url", 2);
    char *buffer = NULL;
    const char parameterEncoding[] = "?";

    if (!isAuth) {
        if (parameter) {
            buffer = malloc (strlen (session->baseUrl) + strlen (endpoint)
                             + strlen (parameterEncoding) + strlen (parameter) + 4);
            strcpy (buffer, session->baseUrl);
            strcat (buffer, endpoint);
            strcat (buffer, parameterEncoding);
            strcat (buffer, parameter);
        }
        else {
            buffer = malloc (strlen (session->baseUrl) + strlen (endpoint) + 2);
            strcpy (buffer, session->baseUrl);
            strcat (buffer, endpoint);
        }
    }
    else {
        if (parameter) {
            buffer = malloc (strlen (session->authUrl) + strlen (endpoint)
                             + strlen (parameterEncoding) + strlen (parameter) + 4);
            strcpy (buffer, session->authUrl);
            strcat (buffer, endpoint);
            strcat (buffer, parameterEncoding);
            strcat (buffer, parameter);
        }
        else {
            buffer = malloc (strlen (session->authUrl) + strlen (endpoint) + 2);
            strcpy (buffer, session->authUrl);
            strcat (buffer, endpoint);
        }
    }
    *url = buffer;
}

void
openTIDAL_CurlModelInit (openTIDAL_CurlContainer *model)
{
    model->body = NULL;
    model->header = NULL;
    model->endpoint = NULL;
    model->parameter = NULL;
    model->postData = NULL;
    model->responseCode = 0;
    model->status = -1;
    openTIDAL_VerboseHelper ("cURL Handle", "Initialised Curl Container", 2);
}

void
openTIDAL_CurlCleanup (openTIDAL_SessionContainer *session)
{
    curl_easy_cleanup (session->curlHandle);
    curl_global_cleanup ();
}

void
openTIDAL_CurlRequestCleanup (openTIDAL_CurlContainer *model)
{
    free (model->body);
    free (model->header);
    free (model->endpoint);
    free (model->parameter);
    free (model->postData);
}

void
openTIDAL_CurlRequest (openTIDAL_SessionContainer *session, openTIDAL_CurlContainer *model,
                       const char *type, const char *endpoint, const char *parameter,
                       const char *postData, const int isAuth, const int isDummy)
{
    CURLcode res;

    if (!session->curlHandle) {
        openTIDAL_VerboseHelper ("cURL Handle", "Initialise cURL Global and cURL Easy Handle", 2);
        curl_global_init (CURL_GLOBAL_ALL);
        session->curlHandle = curl_easy_init ();
    }

    if (session->curlHandle) {
        openTIDAL_VerboseHelper ("cURL Handle", "Handle is initialised. Prepare cURL Easy Request",
                                 2);
        struct curl_slist *chunk = NULL;
        struct openTIDAL_CurlMemory memchunk;
        char *url = NULL;
        char *header = NULL;
        int isHead = 0;

        /* check if access_token has expired  */
        if (!isAuth) {
            openTIDAL_SessionRefresh (session);
        }

        memchunk.memory = malloc (1);
        memchunk.size = 0;

        openTIDAL_CurlConcatenateUrl (session, &url, endpoint, parameter, isAuth);

        openTIDAL_VerboseHelper ("cURL Handle", "Url: %s", 3, url);
        openTIDAL_VerboseHelper ("cURL Handle", "Header: %s", 3, header);
        openTIDAL_VerboseHelper ("cURL Handle", "Begin curl_easy_opt configuration", 2);

        curl_easy_setopt (session->curlHandle, CURLOPT_URL, url);
        curl_easy_setopt (session->curlHandle, CURLOPT_NOBODY, 0L);
        curl_easy_setopt (session->curlHandle, CURLOPT_HEADERFUNCTION, NULL);
        curl_easy_setopt (session->curlHandle, CURLOPT_HEADERDATA, NULL);
        curl_easy_setopt (session->curlHandle, CURLOPT_CUSTOMREQUEST, NULL);

        if (strncmp (type, "GET", 4) == 0) {
            openTIDAL_VerboseHelper ("cURL Handle", "Perform a GET Request", 2);
            curl_easy_setopt (session->curlHandle, CURLOPT_HTTPGET, 1L);
        }
        else if (strncmp (type, "POST", 5) == 0) {
            openTIDAL_VerboseHelper ("cURL Handle", "Perform a POST Request", 2);
            curl_easy_setopt (session->curlHandle, CURLOPT_HTTPPOST, 1L);
            curl_easy_setopt (session->curlHandle, CURLOPT_POSTFIELDS, postData);
        }
        else if (strncmp (type, "DELETE", 7) == 0) {
            openTIDAL_VerboseHelper ("cURL Handle", "Perform a DELETE Request", 2);
            curl_easy_setopt (session->curlHandle, CURLOPT_POSTFIELDS, postData);
            curl_easy_setopt (session->curlHandle, CURLOPT_CUSTOMREQUEST, "DELETE");
        }
        else if (strncmp (type, "PUT", 4) == 0) {
            openTIDAL_VerboseHelper ("cURL Handle", "Perform a PUT Request", 2);
            curl_easy_setopt (session->curlHandle, CURLOPT_POSTFIELDS, postData);
            curl_easy_setopt (session->curlHandle, CURLOPT_CUSTOMREQUEST, "PUT");
        }
        else if (strncmp (type, "HEAD", 5) == 0) {
            openTIDAL_VerboseHelper ("cURL Handle", "Perform a HEAD Request", 1);
            curl_easy_setopt (session->curlHandle, CURLOPT_NOBODY, 1L);
            curl_easy_setopt (session->curlHandle, CURLOPT_HEADERFUNCTION,
                              openTIDAL_CurlCallbackFunction);
            curl_easy_setopt (session->curlHandle, CURLOPT_HEADERDATA, &memchunk);
            isHead = 1;
        }

        if (!isDummy && !isHead) {
            curl_easy_setopt (session->curlHandle, CURLOPT_WRITEFUNCTION,
                              openTIDAL_CurlCallbackFunction);
            curl_easy_setopt (session->curlHandle, CURLOPT_WRITEDATA, &memchunk);
        }
        else {
            curl_easy_setopt (session->curlHandle, CURLOPT_WRITEFUNCTION,
                              openTIDAL_CurlCallBackDummyFunction);
        }

        if (isAuth) {
            curl_easy_setopt (session->curlHandle, CURLOPT_USERNAME, session->clientId);
            curl_easy_setopt (session->curlHandle, CURLOPT_PASSWORD, session->clientSecret);
        }
        else {
            /* Add auth header */
            openTIDAL_CurlConcatenateAuthHeader (session, &header);

            chunk = curl_slist_append (chunk, header);
            curl_easy_setopt (session->curlHandle, CURLOPT_HTTPHEADER, chunk);
        }

        if (openTIDAL_GetLogLevel () == 3) {
            curl_easy_setopt (session->curlHandle, CURLOPT_VERBOSE, 1L);
        }

        openTIDAL_VerboseHelper ("cURL Handle", "End curl_easy_opt configuration", 2);
        openTIDAL_VerboseHelper ("cURL Handle", "Call curl_easy_perform", 2);
        res = curl_easy_perform (session->curlHandle);

        if (res != CURLE_OK) {
            openTIDAL_VerboseHelper ("cURL Handle", "CURLE_OK check failed", 2);
        }
        else {
            long http_code = 0;
            curl_easy_getinfo (session->curlHandle, CURLINFO_RESPONSE_CODE, &http_code);
            openTIDAL_VerboseHelper ("cURL Handle", "CURLE_OK check success", 2);
            model->status = 0;
            model->body = memchunk.memory;
            model->responseCode = http_code;
        }

        /* cleanup */
        curl_slist_free_all (chunk);
        free (url);
        free (header);
        openTIDAL_VerboseHelper ("cURL Handle", "openTIDAL_CurlRequest Finished", 2);
    }
}
