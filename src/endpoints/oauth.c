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
#include <time.h>
#include <unistd.h>

#include "../include/handles.h"
#include "../include/openTIDAL.h"
#include "../include/parse.h"

openTIDAL_ContentContainer
openTIDAL_AuthCreateUserCode (openTIDAL_SessionContainer *session)
{
    openTIDAL_ContentContainer o;
    openTIDAL_CurlContainer curl;
    const char *endpoint = "/v1/oauth2/device_authorization";

    openTIDAL_StructInit (&o);

    openTIDAL_StringHelper (&curl.postData, "client_id=%s&scope=%s", session->clientId,
                            session->scopes);
    if (!curl.postData) {
        o.status = -14;
        return o;
    }

    openTIDAL_CurlRequest (session, &curl, "POST", endpoint, NULL, curl.postData, 1, 0);
    if (curl.status != -1) {
        o.json = openTIDAL_cJSONParseHelper (curl.body);
        if (!o.json) {
            o.status = -14;
            return o;
        }

        if (curl.responseCode == 200) {
            openTIDAL_LoginCodeContainer Value;

            json_login_code_model processed_json = json_parse_login_code ((cJSON *)o.json);
            parse_login_code_values (&Value, &processed_json);
            o.status = 1;
            Value.expires_in = time (NULL) + Value.timeFrame;

            o.code = Value;
        }
        else {
            openTIDAL_ParseVerbose ("oAuth2", "Not a 200 Response", 1);
            o.status = 0;
        }
    }
    openTIDAL_CurlRequestCleanup (&curl);
    return o;
}

openTIDAL_ContentContainer
openTIDAL_AuthCreateBearerToken (openTIDAL_SessionContainer *session, char *device_code)
{
    openTIDAL_ContentContainer o;
    openTIDAL_CurlContainer curl;
    const char *endpoint = "/v1/oauth2/token";
    const char grant_type[] = "urn:ietf:params:oauth:grant-type:device_code";

    openTIDAL_StructInit (&o);

    openTIDAL_StringHelper (&curl.postData, "client_id=%s&device_code=%s&grant_type=%s&scope=%s",
                            session->clientId, device_code, grant_type, session->scopes);
    if (!curl.postData) {
        o.status = -14;
        return o;
    }

    openTIDAL_CurlRequest (session, &curl, "POST", endpoint, NULL, curl.postData, 1, 0);
    if (curl.status != -1) {
        cJSON *check_status = NULL;
        cJSON *check_error = NULL;

        o.json = openTIDAL_cJSONParseHelper (curl.body);
        if (!o.json) {
            o.status = -14;
            return o;
        }

        check_status = cJSON_GetObjectItemCaseSensitive ((cJSON *)o.json, "status");
        check_error = cJSON_GetObjectItemCaseSensitive ((cJSON *)o.json, "error");

        if (cJSON_IsNumber (check_status) != 1) {
            json_login_token_model processed_json = json_parse_login_token ((cJSON *)o.json);
            parse_login_token_values (&o.token, &processed_json);
            o.status = 1;

            o.token.expires_in = o.token.timeFrame + time (NULL);
            session->expiresIn = o.token.expires_in;
            session->countryCode = o.token.countryCode;
            session->userId = o.token.userId;
            session->accessToken = o.token.access_token;
            session->username = o.token.username;
            session->refreshToken = o.token.refresh_token;
            session->tokenRequest = o.json;

            /* get subscription info */
            openTIDAL_ContentContainer sub = openTIDAL_GetUserSubscription (session);
            if (sub.status == 1) {
                if (strcmp (sub.subscription.highestSoundQuality, "HIGH") == 0) {
                    session->audioQuality = "HIGH";
                }
                else if (strcmp (sub.subscription.highestSoundQuality, "LOSSLESS") == 0) {
                    session->audioQuality = "LOSSLESS";
                }
                else if (strcmp (sub.subscription.highestSoundQuality, "HI_RES") == 0) {
                    session->audioQuality = "HI_RES";
                }
            }
            openTIDAL_StructDelete (&sub);
        }
        else {
            if (!cJSON_IsNull (check_error)) {
                if (strcmp (check_error->valuestring, "authorization_pending") == 0) {
                    o.status = 2;
                }
            }
        }
    }
    openTIDAL_CurlRequestCleanup (&curl);
    return o;
}

openTIDAL_ContentContainer
openTIDAL_AuthRefreshBearerToken (openTIDAL_SessionContainer *session, char *refresh_token)
{
    openTIDAL_ContentContainer o;
    openTIDAL_CurlContainer curl;
    const char *endpoint = "/v1/oauth2/token";

    openTIDAL_StructInit (&o);

    openTIDAL_StringHelper (&curl.postData,
                            "client_id=%s&refresh_token=%s&grant_type=refresh_token&scope=%s",
                            session->clientId, refresh_token, session->scopes);
    if (!curl.postData) {
        o.status = -14;
        return o;
    }

    openTIDAL_CurlRequest (session, &curl, "POST", endpoint, NULL, curl.postData, 1, 0);
    if (curl.status != -1) {
        o.json = openTIDAL_cJSONParseHelper (curl.body);
        if (!o.json) {
            o.status = -14;
            return o;
        }

        if (curl.responseCode == 200) {
            json_login_token_model processed_json = json_parse_login_token ((cJSON *)o.json);
            parse_login_token_values (&o.token, &processed_json);

            o.status = 1;
        }
    }
    openTIDAL_CurlRequestCleanup (&curl);
    return o;
}

int
openTIDAL_AuthLogout (openTIDAL_SessionContainer *session)
{
    openTIDAL_CurlContainer curl;
    char *endpoint = "/v1/logout";
    int status = -1;

    openTIDAL_CurlRequest (session, &curl, "POST", endpoint, NULL, NULL, 0, 1);
    if (curl.status != 1) {
        if (curl.responseCode == 200 || curl.responseCode == 204) {
            status = 1;
        }
    }
    openTIDAL_CurlRequestCleanup (&curl);
    return status;
}
