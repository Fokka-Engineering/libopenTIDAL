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

#include "../include/parse.h"
#include "../include/handles.h"
#include "../include/openTIDAL.h"

openTIDAL_ContentContainer
openTIDAL_AuthCreateUserCode(openTIDAL_SessionContainer *session)
{
    openTIDAL_ContentContainer o;
    openTIDAL_CurlContainer curl;
    char *endpoint = "/v1/oauth2/device_authorization";
    char data[50];
    char scopes[] = "r_usr+w_usr";

    openTIDAL_StructInit(&o);

    snprintf(data, 50, "client_id=%s&scope=%s", session->clientId, scopes); 
    
    openTIDAL_CurlRequest(session, &curl, "POST", endpoint, NULL, data, 1, 0); 
    if ( curl.status != -1 ) {
        cJSON *input_json = NULL;
        input_json = json_parse(curl.body);
        
        if ( curl.responseCode == 200 ) {
            openTIDAL_LoginCodeContainer Value;

            json_login_code_model processed_json = json_parse_login_code(input_json);
            parse_login_code_values(&Value, &processed_json);
            o.status = 1;
            Value.expires_in = time(NULL) + Value.timeFrame;

            o.code = Value;
        }
        else {
            openTIDAL_ParseVerbose("oAuth2", "Not a 200 Response", 1);
            o.status = 0;
        }

        o.json = input_json;
    }
    else
    {
        o.status = -1;
    }

    openTIDAL_CurlRequestCleanup(&curl);
    return o;
}

openTIDAL_ContentContainer
openTIDAL_AuthCreateBearerToken(openTIDAL_SessionContainer *session, char *device_code)
{
    openTIDAL_ContentContainer o;
    openTIDAL_CurlContainer curl;
    char *data = NULL;
    const char *endpoint = "/v1/oauth2/token";
    const char grant_type[] = "urn:ietf:params:oauth:grant-type:device_code";
    const char scopes[] = "r_usr+w_usr";
    
    data = malloc(strlen(device_code) + strlen(grant_type) + strlen(scopes) + 43);
    if (!data) {
        o.status = -14;
        return o;
    }
    sprintf(data, "client_id=%s&device_code=%s&grant_type=%s&scope=%s",
                        session->clientId, device_code, grant_type, scopes);
   
    openTIDAL_CurlRequest(session, &curl, "POST", endpoint, NULL, data, 1, 0);
    if ( curl.status != -1 ) {
        openTIDAL_LoginTokenContainer Value;
        cJSON *input_json = NULL;
        cJSON *check_status = NULL;
        cJSON *check_error = NULL;
        
        input_json = json_parse(curl.body);
        check_status = cJSON_GetObjectItemCaseSensitive(input_json, "status");
        check_error = cJSON_GetObjectItemCaseSensitive(input_json, "error");
        
        if ( cJSON_IsNumber(check_status) != 1 ) {
            json_login_token_model processed_json = json_parse_login_token(input_json);
            parse_login_token_values(&Value, &processed_json);
            o.status = 1;
            
            Value.expires_in = Value.timeFrame + time(NULL);
            session->expiresIn = Value.expires_in;
            session->countryCode = Value.countryCode;
            session->userId = Value.userId;
            session->accessToken = Value.access_token;
            session->refreshToken = Value.refresh_token;
            session->tokenRequest = o.json;
            
            o.token = Value;
            /* get subscription info */
            openTIDAL_ContentContainer sub = openTIDAL_GetUserSubscription(session);
            if ( sub.status == 1 ) {
                if ( strcmp(sub.subscription.highestSoundQuality, "HIGH") == 0 ) {
                    session->audioQuality = "HIGH";
                }
                else if ( strcmp(sub.subscription.highestSoundQuality, "LOSSLESS") == 0 ) {
                    session->audioQuality = "LOSSLESS";
                }
                else if ( strcmp(sub.subscription.highestSoundQuality, "HI_RES") == 0 ) {
                    session->audioQuality = "HI_RES";
                }
            }
            openTIDAL_StructDelete(&sub);
        }
        else {
            if ( !cJSON_IsNull(check_error) ) {
                if ( strcmp(check_error->valuestring, "authorization_pending") == 0 ) {
                    o.status = 2;
                }
            }
        }
        o.json = input_json;
    }
    else {
        o.status = -1;
    }

    openTIDAL_CurlRequestCleanup(&curl);
    return o;
}

openTIDAL_ContentContainer
openTIDAL_AuthRefreshBearerToken(openTIDAL_SessionContainer *session, char *refresh_token)
{
    openTIDAL_ContentContainer o;
    openTIDAL_CurlContainer curl;
    const char *endpoint = "/v1/oauth2/token";
    char *data = NULL;
    const char grant_type[] = "refresh_token";
    const char scopes[] = "r_usr+w_usr";
    const char keyOne[] = "client_id=";
    const char keyTwo[] = "&refresh_token=";
    const char keyThree[] = "&grant_type=";
    const char keyFour[] = "&scope=";
    
    data = malloc(strlen(keyOne) + strlen(session->clientId) + strlen(keyTwo) + strlen(refresh_token) + 
            strlen(keyThree) + strlen(grant_type) + strlen(keyFour) + strlen(scopes) + 1);
    if ( !data ) {
        o.status = -14;
        return o;
    }
    strcpy(data, keyOne);
    strcat(data, session->clientId);
    strcat(data, keyTwo);
    strcat(data, refresh_token);
    strcat(data, keyThree);
    strcat(data, grant_type);
    strcat(data, keyFour);
    strcat(data, scopes); 
   
    openTIDAL_CurlRequest(session, &curl, "POST", endpoint, NULL, data, 1, 0); 
    if ( curl.status != -1 ) {
        cJSON *input_json = NULL;
        input_json = json_parse(curl.body);
        
        if ( curl.responseCode == 200 ) {
            openTIDAL_LoginTokenContainer Value;

            json_login_token_model processed_json = json_parse_login_token(input_json);
            parse_login_token_values(&Value, &processed_json);

            o.status = 1;

            o.token = Value;
        }
        
        /* Cleanup */
        o.json = input_json;
    }
    else {
        o.status = -1;
    }

    free(data);
    openTIDAL_CurlRequestCleanup(&curl);
    return o;
}

int openTIDAL_AuthLogout(openTIDAL_SessionContainer *session)
{
    openTIDAL_CurlContainer curl;
    char *endpoint = "/v1/logout";
    int status = 0;

    openTIDAL_CurlRequest(session, &curl, "POST", endpoint, NULL, NULL, 0, 1);
    if ( curl.status != 1 ) {
        if ( curl.responseCode == 204) {
            status = 1;
        }

    }
    else
    {
        status = -1;
    }

    openTIDAL_CurlRequestCleanup(&curl);
    return status;
}
