#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/openTIDAL.h"

login_code_model login_create_code() /* TODO: responseCode 400 when auth is still pending  */
{
  login_code_model Value;
  /* Build UrlEncoded String */
  char *str;
  str = malloc(strlen(client_id)+37);
  strcat(str, "client_id=");
  strcat(str, client_id);
  strcat(str, "&scope=r_usr+w_usr+w_sub");
  /* POST Request */
  curl_model req = curl_post_auth("oauth2/device_authorization", str);
  free(str);
  if (req.status != -1)
  {
    cJSON *input_json = json_parse(req.body);
    if (req.responseCode == 200)
    {
      /* Copy JSON Response */
      cJSON *deviceCode = cJSON_GetObjectItemCaseSensitive(input_json, "deviceCode");
      cJSON *userCode = cJSON_GetObjectItemCaseSensitive(input_json, "userCode");

      strncpy(Value.deviceCode, deviceCode->valuestring, sizeof(Value.deviceCode));
      strncpy(Value.userCode, userCode->valuestring, sizeof(Value.userCode));
      /* Cleanup */
      cJSON_Delete(input_json);
      free(req.body);
      return Value;
    }
    else if (req.responseCode == 400)
    {
      Value.status = parse_badrequest(input_json, 0, "Device Authorization");
      cJSON_Delete(input_json);
      free(req.body);
      return Value;
    }
    else if (req.responseCode == 401)
    {
      Value.status = parse_unauthorized(input_json, 0);
      cJSON_Delete(input_json);
      free(req.body);
      return Value;
    }
    else if (req.responseCode == 404)
    {
      Value.status = parse_notfound(input_json, 0, "Device Authorization");
      cJSON_Delete(input_json);
      free(req.body);
      return Value;
    }
    else
    {
      Value.status = 0;
      cJSON_Delete(input_json);
      free(req.body);
      return Value;
    }
  }
  else
  {
    free(req.body);
    Value.status = -1;
    fprintf(stderr, "[Request Error] Device Authorization: CURLE_OK Check failed.\n");
    return Value; 
  }
}

login_token_model login_create_token(char *device_code) /* TODO: Error Handling */
{
  login_token_model Value;
  char *str;
  str = malloc(strlen(device_code)+strlen(client_id)+11+14+25+57+1);
  strcat(str, "client_id=");
  strcat(str, client_id);
  strcat(str, "&device_code=");
  strcat(str, device_code);
  strcat(str, "&grant_type=urn:ietf:params:oauth:grant-type:device_code");
  strcat(str, "&scope=r_usr+w_usr+w_sub");
  curl_model req = curl_post_auth("oauth2/token", str);
  free(str);
  if (req.status != 1)
  { 
    cJSON *input_json = json_parse(req.body);
    if (req.responseCode == 200)
    {
      cJSON *check_status = cJSON_GetObjectItemCaseSensitive(input_json, "status");
      cJSON *check_error = cJSON_GetObjectItemCaseSensitive(input_json, "error");
      cJSON *access_token = cJSON_GetObjectItemCaseSensitive(input_json, "access_token");
      cJSON *refresh_token = cJSON_GetObjectItemCaseSensitive(input_json, "refresh_token");
      cJSON *expires_in = cJSON_GetObjectItemCaseSensitive(input_json, "expires_in");

      if (cJSON_IsNumber(check_status) != 1)
      {
        Value.status = 1;
        strncpy(Value.access_token, access_token->valuestring, sizeof(Value.access_token));
        strncpy(Value.refresh_token, refresh_token->valuestring, sizeof(Value.refresh_token));
        Value.expires_in = expires_in->valueint;
      }
      else
      {
        if (cJSON_IsNull(check_error) != 1)
        {
          if (strcmp(check_error->valuestring, "authorization_pending") == 0)
          {
            Value.status = 0;
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
      Value.status = 0;
      free(req.body);
      cJSON_Delete(input_json);
      return Value;
    }
  }
  else
  {
    free(req.body);
    Value.status = -1;
    fprintf(stderr, "[Request Error] Token Request: CURLE_OK Check failed.\n");
    return Value;
  }
}
/* TODO: Add isUserId/AppleUid Bools, Improve Error Handling */
login_token_model login_refresh_token(char *refresh_token)
{
  login_token_model Value;
  char *str;
  str = malloc(strlen(refresh_token)+strlen(client_id)+11+16+26+25+1);
  strcat(str, "client_id=");
  strcat(str, client_id);
  strcat(str, "&grant_type=refresh_token");
  strcat(str, "&refresh_token=");
  strcat(str, refresh_token);
  strcat(str, "&scope=r_usr+w_usr+w_sub");
  curl_model req = curl_post_auth("oauth2/token", str);
  free(str);
  if (req.status != 1)
  {
    cJSON *input_json = json_parse(req.body);
    if (req.responseCode == 200)
    {
      cJSON *access_token = cJSON_GetObjectItemCaseSensitive(input_json, "access_token");
      cJSON *expires_in = cJSON_GetObjectItemCaseSensitive(input_json, "expires_in");
      cJSON *user = cJSON_GetObjectItemCaseSensitive(input_json, "user");
      cJSON *countryCode = cJSON_GetObjectItemCaseSensitive(user, "countryCode");
      cJSON *username = cJSON_GetObjectItemCaseSensitive(user, "username");
      cJSON *userId = cJSON_GetObjectItemCaseSensitive(user, "userId");
      cJSON *facebookUid = cJSON_GetObjectItemCaseSensitive(user, "facebookUid");
      cJSON *appleUid = cJSON_GetObjectItemCaseSensitive(user, "appleUid");

      strncpy(Value.access_token, access_token->valuestring, sizeof(Value.access_token));
      Value.expires_in = expires_in->valueint;
      strncpy(Value.countryCode, countryCode->valuestring, sizeof(Value.countryCode));
      strncpy(Value.username, username->valuestring, sizeof(Value.username));
      if (cJSON_IsNull(userId) != 1) /* userId is only present if account was created without third-party services (Apple, FB)  */
      {
        Value.userId = userId->valueint;
      }
      if (cJSON_IsNull(facebookUid) != 1) /* check if facebookUid object is not NULL  */
      {
        Value.facebookUid = facebookUid->valueint;
      }
      if (cJSON_IsNull(appleUid) != 1) /* check if appleUid object is not Null  */
      {
        Value.appleUid = appleUid->valueint;
      }
      /* Cleanup */
      cJSON_Delete(input_json);
      free(req.body);
      return Value;
    }
    else
    {
      Value.status = 0;
      free(req.body);
      cJSON_Delete(input_json);
      return Value;
    }
  }
  else
  {
    free(req.body);
    Value.status = -1;
    fprintf(stderr, "[Request Error] Refresh Token: CURLE_OK Check failed.\n");
    return Value;
  }
}
