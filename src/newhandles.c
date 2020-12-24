/*
    Copyright (c) 2020 Hugo Melder and openTIDAL contributors
    
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#include "include/handles.h"
#include "include/parse.h"
#include "include/openTIDAL.h"

CURL *curl = NULL;

struct openTIDAL_CurlMemory
{
    char *response;
    size_t size;
};

static size_t 
openTIDAL_CurlCallbackFunction(void *data, size_t size, size_t nmemb, void *userp)
{
    size_t realsize = size * nmemb;
    struct openTIDAL_CurlMemory *mem = (struct openTIDAL_CurlMemory *)userp;
  
    char *ptr = realloc(mem->response, mem->size + realsize + 1);
    if(ptr == NULL)
            return 0;  /* out of memory! */

    mem->response = ptr;
    memcpy(&(mem->response[mem->size]), data, realsize);
    mem->size += realsize;
    mem->response[mem->size] = 0;

    return realsize;
}

static size_t
openTIDAL_CurlCallBackDummyFunction(void *data, size_t size, size_t nmemb, void *userp)
{
    return size * nmemb;
}

static void openTIDAL_CurlConcatenateAuthHeader(openTIDAL_SessionContainer *session, char *buffer)
{
    const char key[] = "Authentication: ";
    const char keyDemo[] = "X-Tidal-Token: ";
    const char authType[] = "Bearer ";

    if ( !config.demoEnabled ) {
        buffer = malloc(strlen(key) + strlen(authType) + strlen(session->accessToken) + 3);
        strcpy(buffer, key);
        strcat(buffer, authType);
        strcat(buffer, session->accessToken);
    }
    else {
        buffer = malloc(strlen(keyDemo) + strlen(session->clientId) + 2);
        strcpy(buffer, keyDemo);
        strcat(buffer, session->clientId);
    }
}

static void
openTIDAL_CurlConcatenateUrl(char *buffer, const char *endpoint,
        const char *parameter, const int isAuth)
{
    const char parameterEncoding[] = "?";
    
    if ( !isAuth ) {
        if ( parameter ) {
            buffer = malloc(strlen(config.baseUrl) + strlen(endpoint)
                    + strlen(parameterEncoding) + strlen(parameter) + 4);
            strcpy(buffer, config.baseUrl);
            strcat(buffer, endpoint);
            strcat(buffer, parameterEncoding);
            strcat(buffer, parameter);
        }
        else {
            buffer = malloc(strlen(config.baseUrl) + strlen(endpoint) + 2);
            strcpy(buffer, config.baseUrl);
            strcat(buffer, endpoint);
        }
    }
    else {
        if ( parameter ) {
            buffer = malloc(strlen(config.authUrl) + strlen(endpoint)
                    + strlen(parameterEncoding) + strlen(parameter) + 4);
            strcpy(buffer, config.authUrl);
            strcat(buffer, endpoint);
            strcat(buffer, parameterEncoding);
            strcat(buffer, parameter); 
        } 
        else {
            buffer = malloc(strlen(config.authUrl) + strlen(endpoint) + 2);
            strcpy(buffer, config.authUrl);
            strcat(buffer, endpoint);
        }
    }
}

static void openTIDAL_CurlModelInit(openTIDAL_CurlContainer *model)
{
    model->body = NULL;
    model->header = NULL;
    model->responseCode = 0;
    model->status = -1;
}

void openTIDAL_CurlCleanup()
{
    curl_easy_cleanup(curl);
    curl_global_cleanup();
}

void openTIDAL_CurlRequestCleanup(openTIDAL_CurlContainer *model)
{
    free(model->body);
    free(model->header);
}

void 
openTIDAL_CurlRequest(openTIDAL_SessionContainer *config, openTIDAL_CurlContainer *model, const char *type, const char *endpoint,
        const char *parameter, const char *postData, const int isAuth, const int isDummy)
{
    CURLcode res;
    
    if ( !curl ) {
        curl_global_init(CURL_GLOBAL_ALL);
        curl = curl_easy_init();
    }
    
    if ( curl ) {
        struct curl_slist *chunk = NULL;
        struct openTIDAL_CurlMemory memchunk;
        char *url = NULL;
        char *header = NULL;
       
        openTIDAL_CurlConcatenateUrl(url, endpoint, parameter, isAuth);
        openTIDAL_CurlConcatenateAuthHeader(header);
        openTIDAL_CurlModelInit(model);

        /* Add auth header */
        chunk = curl_slist_append(chunk, header);
        
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);
        curl_easy_setopt(curl, CURLOPT_URL, url);
        
        if( strncmp(type, "GET", 4) == 0 ) {
            curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, NULL);
        }
        else if ( strncmp(type, "POST", 5 ) == 0 ) {
            curl_easy_setopt(curl, CURLOPT_HTTPPOST, 1L);
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData);
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, NULL);
        }
        else if ( strncmp(type, "DELETE", 7 ) == 0) {
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData);
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
        }
        else if ( strncmp(type, "PUT", 4 ) == 0) {
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData);
            curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");
        }
       
        if ( !isDummy ) { 
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, openTIDAL_CurlCallbackFunction);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &memchunk);
        }
        else {
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, openTIDAL_CurlCallBackDummyFunction);
        }

        if ( isAuth ) {
            curl_easy_setopt(curl, CURLOPT_USERNAME, config->clientId);
            curl_easy_setopt(curl, CURLOPT_PASSWORD, config->clientSecret);
        }

        if ( openTIDAL_GetLogLevel() == 3 ) {
            curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        }

        res = curl_easy_perform(curl);

        if ( res != CURLE_OK ) {
            openTIDAL_ParseVerbose("cURL Handle", "CURLE_OK check failed", 2);
        }
        else {
            model->status = 0;
            model->body = memchunk.response;
            curl_easy_setopt(curl, CURLINFO_RESPONSE_CODE, &model->responseCode); 
        }

        /* cleanup */
        curl_slist_free_all(chunk);
        free(url);
        free(header);
    }
}
