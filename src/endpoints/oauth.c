#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "../include/parse.h"
#include "../include/handles.h"
#include "../include/openTIDAL.h"

/* TODO: Improve Error Handling with HTTP Codes */

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
    if (req.responseCode == 200)
    {
      json_login_code_model processed_json = json_parse_login_code(input_json);
      Value = parse_login_code_values(processed_json);
      Value.status = 1;
      Value.expires_in = time(NULL) + Value.timeFrame;
    }
    else
    {
      fprintf(stderr, "[Request Error] Not a 200 Response.\n");
      Value.status = 0;
    }
    /* Cleanup */
    cJSON_Delete(input_json);
    free(req.body);
    return Value;
  }
  else
  {
    Value.status = -1;
    fprintf(stderr, "[Request Error] Device Authorization: CURLE_OK Check failed.\n");
    free(req.body);
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
  if (req.status != -1)
  { 
    cJSON *input_json = json_parse(req.body);
    cJSON *check_status = cJSON_GetObjectItemCaseSensitive(input_json, "status");
    cJSON *check_error = cJSON_GetObjectItemCaseSensitive(input_json, "error");
    
    if (cJSON_IsNumber(check_status) != 1)
    {
      json_login_token_model processed_json = json_parse_login_token(input_json);
      Value = parse_login_token_values(processed_json);
      Value.status = 1;
      Value.expires_in = Value.timeFrame + time(NULL);
      expires_in = Value.expires_in;
      countryCode = Value.countryCode;
      userId = Value.userId;
      create_persistent(Value.username, "HIGH", "HIGH"); /* Default Quality Settings  */
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
          Value.status = 0;
        }
      }
      else
      {
        Value.status = 0;
      }
    }
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
  if (req.status != -1)
  {
    cJSON *input_json = json_parse(req.body);
    if (req.responseCode == 200)
    {
      json_login_token_model processed_json = json_parse_login_token(input_json);
      Value = parse_login_token_values(processed_json);

      Value.status = 1;
      access_token = Value.access_token;
      expires_in = Value.expires_in;
      countryCode = Value.countryCode;
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
