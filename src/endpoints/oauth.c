#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "../include/parse.h"
#include "../include/handles.h"
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
      char *audio_quality;
      char *video_quality;

      json_login_token_model processed_json = json_parse_login_token(input_json);
      Value = parse_login_token_values(processed_json);
      Value.status = 1;
      Value.expires_in = Value.timeFrame + time(NULL);
      expires_in = Value.expires_in;
      countryCode = Value.countryCode;
      userId = Value.userId;
      access_token = Value.access_token;
      refresh_token = Value.refresh_token;

      /* get subscription info */
      user_subscription_model subscription = get_user_subscription();
      if (subscription.status == 1)
      { 
        if (strcmp(subscription.highestSoundQuality, "\0") == 0)
        {
          /* free subscription without streaming privileges */
          audio_quality = "LOW";
	  video_quality = "LOW";
        }
        else
        {
          /* active subscription with streaming privileges */
          audio_quality = subscription.highestSoundQuality;
  	  video_quality = "HIGH";
        }
      }
      else
      {
        /* failed to retrieve acurate settings use minimum */
        audio_quality = "LOW";
	video_quality = "LOW";
      }

      create_persistent(Value.username, audio_quality, video_quality);
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

int logout()
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
    fprintf(stderr, "[Request Error] Logout: CURLE_OK Check failed.\n");
    return status;
  }
}
