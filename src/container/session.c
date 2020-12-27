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
#include <time.h>
#include "../include/openTIDAL.h"

struct openTIDAL_SessionContainer {
    /* File location of persistent session config.
     * If the pointer is NULL restricted authorization
     * is enabled. */
    char *location;
    
    /* Stream of a newly created json session config.
     * Allocated in heap if a new persistent config is
     * created or updated.  */
    char *newStream;
    /* Void pointer to allocated session config. Typecasted
     * cJSON Structure. If a persistent session config is
     * specified the file is loaded into heap and parsed with
     * cJSON. The cJSON Structure gets deallocated with
     * openTIDAL_SessionCleanup */
    void *stream;
    /* Part of the SessionAuthAutoRenewal process. Typecasted
     * cJSON Structure. Allocated after refresh request is performed.
     * Old cJSON Structure from previous request will be deallocated
     * prior. */
    void *refreshRequest;
    /* Part of the AuthCreateBearerToken process. Typecasted cJSON
     * Structure. Allocated after request is performed. Only used
     * after persistent config creation. */ 
    void *tokenRequest;

    /* Boolean to specifiy state of authorization.
     * 
     * If no file location is specified cURL performs
     * a clientId-based authorization (x-tidal-token: {client_id}). 
     * 
     * User-based and stream requests are disabled and result in a 401.
     * A 30s preview of a track (64kbps 22.05kHz MP3) or
     * a video (640x360 1116kbps MP4) can be requested.
     * This mode is primarly useful for metadata requests. */
    int demoEnabled;

    /* Constant pointers initialised with openTIDAL_SessionInit */
    const char *clientId;
    const char *clientSecret;
    const char *baseUrl;
    const char *authUrl;
   
    /* Pointers to allocated cJSON Values. Either from the persistent
     * session config, the tokenRequest or the refreshRequest. NULL if
     * demoEnabled is TRUE. */ 
    char *username;
    size_t userId;
    time_t expiresIn;
    char *countryCode;
    
    char *accessToken;
    char *refreshToken;

    char *audioQuality;
    char *videoQuality;

    /* Typecasted CURL Structure. Persistent cURL request handle. */
    void *curlHandle;
};

