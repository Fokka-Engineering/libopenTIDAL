#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/openTIDAL.h"

login_code_model login_create_code()
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
  cJSON *input_json = json_parse(req.body);
  /* Copy JSON Response */
  strcpy(Value.deviceCode, cJSON_GetObjectItemCaseSensitive(input_json, "deviceCode")->valuestring);
  strcpy(Value.userCode, cJSON_GetObjectItemCaseSensitive(input_json, "userCode")->valuestring);
  /* Cleanup */
  cJSON_Delete(input_json);
  free(str);
  free(req.body);

  return Value;
}

login_token_model login_create_token(char *device_code)
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
  cJSON *input_json = json_parse(req.body);

  const cJSON * check_status = cJSON_GetObjectItemCaseSensitive(input_json, "status");
  const cJSON * check_error = cJSON_GetObjectItemCaseSensitive(input_json, "error");
  if (cJSON_IsNumber(check_status) != 1)
  {
    Value.status = 1;
    strcpy(Value.access_token, cJSON_GetObjectItemCaseSensitive(input_json, "access_token")->valuestring);
    strcpy(Value.refresh_token, cJSON_GetObjectItemCaseSensitive(input_json, "refresh_token")->valuestring);
    Value.expires_in = cJSON_GetObjectItemCaseSensitive(input_json, "expires_in")->valueint;
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
  free(str);
  free(req.body);

  return Value;
}

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
  cJSON *input_json = json_parse(req.body);

  strcpy(Value.access_token, cJSON_GetObjectItemCaseSensitive(input_json, "access_token")->valuestring);
  Value.expires_in = cJSON_GetObjectItemCaseSensitive(input_json, "expires_in")->valueint;

  cJSON *user = cJSON_GetObjectItemCaseSensitive(input_json, "user");
  strcpy(Value.countryCode, cJSON_GetObjectItemCaseSensitive(user, "countryCode")->valuestring);
  strcpy(Value.username, cJSON_GetObjectItemCaseSensitive(user, "username")->valuestring);
  cJSON *userId = cJSON_GetObjectItemCaseSensitive(user, "userId");
  if (cJSON_IsNull(userId) != 1) /* userId is only present if account was created without third-party services (Apple, FB)  */
  {
    Value.userId = userId->valueint;
  }
  cJSON *facebookUid = cJSON_GetObjectItemCaseSensitive(user, "facebookUid");
  if (cJSON_IsNull(facebookUid) != 1) /* check if facebookUid object is not NULL  */
  {
    Value.facebookUid = facebookUid->valueint;
  }
  cJSON *appleUid = cJSON_GetObjectItemCaseSensitive(user, "appleUid");
  if (cJSON_IsNull(appleUid) != 1) /* check if appleUid object is not Null  */
  {
    Value.appleUid = appleUid->valueint;
  }
  /* Cleanup */
  cJSON_Delete(input_json);
  free(str);
  free(req.body);
  return Value;
}
