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

openTIDAL openTIDAL_CreateLoginCode()
{
    openTIDAL o;
    char data[50];
    char scopes[] = "r_usr+w_usr";

    openTIDAL_StructInit(&o);

    snprintf(data, 50, "client_id=%s&scope=%s", config.clientId, scopes); 
    
    curl_model req = curl_post_auth("/v1/oauth2/device_authorization", data);
    if (req.status != -1)
    {
        cJSON *input_json = json_parse(req.body);
        if (req.responseCode == 200)
        {
            openTIDAL_LoginCodeModel Value;
            json_login_code_model processed_json = json_parse_login_code(input_json);
            parse_login_code_values(&Value, &processed_json);
            o.status = 1;
            Value.expires_in = time(NULL) + Value.timeFrame;

            o.code = Value;
        }
        else
        {
            openTIDAL_ParseVerbose("oAuth2", "Not a 200 Response", 1);
            o.status = 0;
        }
        /* Cleanup */
        o.json = input_json;
        free(req.body);
        return o;
    }
    else
    {
        o.status = -1;
        free(req.body);
        return o; 
    }
}

openTIDAL openTIDAL_CreateLoginToken(char *device_code)
{
    openTIDAL o;
    char data[256];
    char grant_type[] = "urn:ietf:params:oauth:grant-type:device_code";
    char scopes[] = "r_usr+w_usr"; 
    
    snprintf(data, 256, "client_id=%s&device_code=%s&grant_type=%s&scope=%s",
                        config.clientId, device_code, grant_type, scopes);
    
    curl_model req = curl_post_auth("/v1/oauth2/token", data);
    if (req.status != -1)
    {
        openTIDAL_LoginTokenModel Value;
        cJSON *input_json = json_parse(req.body);
        cJSON *check_status = cJSON_GetObjectItemCaseSensitive(input_json, "status");
        cJSON *check_error = cJSON_GetObjectItemCaseSensitive(input_json, "error");
        
        if (cJSON_IsNumber(check_status) != 1)
        {
            json_login_token_model processed_json = json_parse_login_token(input_json);
            parse_login_token_values(&Value, &processed_json);
            o.status = 1;
            
            Value.expires_in = Value.timeFrame + time(NULL);
            config.expiresIn = Value.expires_in;
            config.countryCode = Value.countryCode;
            config.userId = Value.userId;
            config.accessToken = Value.access_token;
            config.refreshToken = Value.refresh_token;
            config.tokenRequest = o.json;
            
            o.token = Value;
            /* get subscription info */
            openTIDAL sub = openTIDAL_GetUserSubscription();
            if (sub.status == 1)
            {
                if (strcmp(sub.subscription.highestSoundQuality, "HIGH") == 0)
                {
                    config.audioQuality = "HIGH";
                }
                else if (strcmp(sub.subscription.highestSoundQuality, "LOSSLESS") == 0)
                {
                    config.audioQuality = "LOSSLESS";
                }
                else if (strcmp(sub.subscription.highestSoundQuality, "HI_RES") == 0)
                {
                    config.audioQuality = "HI_RES";
                }
            }
            openTIDAL_StructDelete(&sub);
            create_persistent();
        }
        else
        {
            if (cJSON_IsNull(check_error) != 1)
            {
                if (strcmp(check_error->valuestring, "authorization_pending") == 0)
                {
                    o.status = 2;
                }
                else
                {
                    o.status = 0;
                }
            }
            else
            {
                o.status = 0;
            }
        }
        o.json = input_json;
        free(req.body);
        return o;
    }
    else
    {
        free(req.body);
        o.status = -1;
        return o;
    }
}

openTIDAL openTIDAL_RefreshLoginToken(char *refresh_token)
{
    openTIDAL o;
    char data[2048];
    char grant_type[] = "refresh_token";
    char scopes[] = "r_usr+w_usr";

    snprintf(data, 2048, "client_id=%s&refresh_token=%s&grant_type=%s&scope=%s",
                        config.clientId, refresh_token, grant_type, scopes);
    
    curl_model req = curl_post_auth("/v1/oauth2/token", data);
    //printf("%s\n", req.body);
    if (req.status != -1)
    {
        cJSON *input_json = json_parse(req.body);
        if (req.responseCode == 200)
        {
            openTIDAL_LoginTokenModel Value;
            json_login_token_model processed_json = json_parse_login_token(input_json);
            parse_login_token_values(&Value, &processed_json);

            o.status = 1;

            o.token = Value;
        }
        else
        {
            o.status = 0;
        }  
        
        /* Cleanup */
        o.json = input_json;
        free(req.body);
        return o;
    }
    else
    {
        free(req.body);
        o.status = -1;
        return o;
    }
}

int openTIDAL_Logout()
{
    int status = 0;

    curl_model req = curl_post("logout", "", "");
    if (req.status != 1)
    {
        if (req.responseCode == 204)
        {
            status = 1;
        }
        else
        {
            status = 0;
        }

        return status;
    }
    else
    {
        free(req.body);
        status = -1;
        return status;
    }
}
