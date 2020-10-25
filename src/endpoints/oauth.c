#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "../include/openTIDAL.h"

login_code_model login_create_code()
{
  login_code_model Value;
  char data[50];
  char scopes[] = "r_usr+w_usr";
 
  snprintf(data, 50, "client_id=%s&scope=%s", client_id, scopes); 
  
  curl_model req = curl_post_auth("oauth2/device_authorization", data);
  if (req.status != -1)
  {
    cJSON *input_json = json_parse(req.body);
    Value.status = 0;
    /* Copy JSON Response */
    cJSON *deviceCode = cJSON_GetObjectItemCaseSensitive(input_json, "deviceCode");
    cJSON *userCode = cJSON_GetObjectItemCaseSensitive(input_json, "userCode");
    
    if (cJSON_IsString(deviceCode))
    {
      Value.status = 1;
      Value.timestamp = time(NULL);
      strncpy(Value.deviceCode, deviceCode->valuestring, sizeof(Value.deviceCode));
      strncpy(Value.userCode, userCode->valuestring, sizeof(Value.userCode));
    }
    /* Cleanup */
    cJSON_Delete(input_json);
    free(req.body);
    return Value;
  }
  else
  {
    free(req.body);
    Value.status = -1;
    fprintf(stderr, "[Request Error] Device Authorization: CURLE_OK Check failed.\n");
    return Value; 
  }
}

login_token_model login_create_token(char *device_code)
{
  login_token_model Value;
  char data[256];
  char grant_type[] = "urn:ietf:params:oauth:grant-type:device_code";
  char scopes[] = "r_usr+w_usr"; 
  
  snprintf(data, 256, "client_id=%s&device_code=%s&grant_type=%s&scope=%s",
            client_id, device_code, grant_type, scopes);
  
  curl_model req = curl_post_auth("oauth2/token", data);
  if (req.status != 1)
  { 
    cJSON *input_json = json_parse(req.body);
    cJSON *check_status = cJSON_GetObjectItemCaseSensitive(input_json, "status");
    cJSON *check_error = cJSON_GetObjectItemCaseSensitive(input_json, "error");
    cJSON *access_token_json = cJSON_GetObjectItemCaseSensitive(input_json, "access_token");
    cJSON *refresh_token_json = cJSON_GetObjectItemCaseSensitive(input_json, "refresh_token");
    cJSON *expires_in_json = cJSON_GetObjectItemCaseSensitive(input_json, "expires_in");
    cJSON *user = cJSON_GetObjectItemCaseSensitive(input_json, "user");
    cJSON *countryCode_json = cJSON_GetObjectItemCaseSensitive(user, "countryCode");
    cJSON *username = cJSON_GetObjectItemCaseSensitive(user, "username");
    cJSON *userId_json = cJSON_GetObjectItemCaseSensitive(user, "userId");
    
    if (cJSON_IsNumber(check_status) != 1)
    {
      Value.status = 1;
      Value.timestamp = time(NULL);
      strncpy(Value.access_token, access_token_json->valuestring, sizeof(Value.access_token));
      access_token = Value.access_token;
      strncpy(Value.refresh_token, refresh_token_json->valuestring, sizeof(Value.refresh_token));
      refresh_token = Value.refresh_token;
      Value.expires_in = expires_in_json->valueint + time(NULL);
      expires_in = expires_in_json->valueint + time(NULL);
      strncpy(Value.countryCode, countryCode_json->valuestring, sizeof(Value.countryCode));
      countryCode = Value.countryCode;
      strncpy(Value.username, username->valuestring, sizeof(Value.username));
      Value.userId = userId_json->valueint;
      userId = userId_json->valueint;
      create_persistent(Value.username);
    }
    else
    {
      if (cJSON_IsNull(check_error) != 1)
      {
        if (strcmp(check_error->valuestring, "authorization_pending") == 0)
        {
          Value.status = 2;
        }
        else
        {
          Value.status = -1;
        }
      }
      else
      {
        Value.status = -1;
      }
    }
    /* Cleanup */
    cJSON_Delete(input_json);
    free(req.body);
    return Value;
  }
  else
  {
    free(req.body);
    Value.status = -1;
    fprintf(stderr, "[Request Error] Token Request: CURLE_OK Check failed.\n");
    return Value;
  }
}

login_token_model login_refresh_token(char *refresh_token)
{
  login_token_model Value;
  char data[2048];
  char grant_type[] = "refresh_token";
  char scopes[] = "r_usr+w_usr";

  snprintf(data, 2048, "client_id=%s&refresh_token=%s&grant_type=%s&scope=%s",
            client_id, refresh_token, grant_type, scopes);
  
  curl_model req = curl_post_auth("oauth2/token", data);
  //printf("%s\n", req.body);
  if (req.status != 1)
  {
    cJSON *input_json = json_parse(req.body);
    cJSON *access_token_json = cJSON_GetObjectItemCaseSensitive(input_json, "access_token");
    cJSON *expires_in_json = cJSON_GetObjectItemCaseSensitive(input_json, "expires_in");
    cJSON *user = cJSON_GetObjectItemCaseSensitive(input_json, "user");
    cJSON *countryCode_json = cJSON_GetObjectItemCaseSensitive(user, "countryCode");
    cJSON *username = cJSON_GetObjectItemCaseSensitive(user, "username");
    cJSON *userId_json = cJSON_GetObjectItemCaseSensitive(user, "userId");
    if (cJSON_IsString(access_token_json) == 1)
    {
      Value.status = 1;
      strncpy(Value.access_token, access_token_json->valuestring, sizeof(Value.access_token));
      access_token = Value.access_token;
      Value.expires_in = expires_in_json->valueint;
      expires_in = Value.expires_in;
      strncpy(Value.countryCode, countryCode_json->valuestring, sizeof(Value.countryCode));
      countryCode = Value.countryCode;
      strncpy(Value.username, username->valuestring, sizeof(Value.username));
      Value.userId = userId_json->valueint;
      userId = Value.userId;
    }
    else
    {
      Value.status = 0;
    }  
    /* Cleanup */
    cJSON_Delete(input_json);
    free(req.body);
    return Value;
  }
  else
  {
    free(req.body);
    Value.status = -1;
    fprintf(stderr, "[Request Error] Refresh Token: CURLE_OK Check failed.\n");
    return Value;
  }
}
