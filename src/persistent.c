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

#include "include/parse.h"
#include "include/handles.h"
#include "include/openTIDAL.h"

openTIDAL_ConfigModel config;

void openTIDAL_ConfigInit(openTIDAL_ConfigModel *config)
{
  config->location = NULL;
  config->newStream = NULL;
  config->stream = NULL;
  config->refreshRequest = NULL;
  config->tokenRequest = NULL;
  config->demoEnabled = 1;

  config->clientId = "8SEZWa4J1NVC5U5Y";
  config->clientSecret = "owUYDkxddz+9FpvGX24DlxECNtFEMBxipU0lBfrbq60=";
  config->baseUrl = "https://api.tidal.com";
  config->authUrl = "https://auth.tidal.com"; 
  
  config->username = NULL;
  config->userId = 0;
  config->expiresIn = 0;
  config->countryCode = "US";

  config->audioQuality = "LOW";
  config->videoQuality = "LOW"; 
}

int openTIDAL_Init(const char *file_location)
{
  int status = 0;
  openTIDAL_ConfigInit(&config);
  if (file_location != NULL)
  {
    config.location = malloc(strlen(file_location) + 1);
    if (config.location != NULL)
    {
      strcpy(config.location, file_location);
      status = scan_persistent();
      config.demoEnabled = 0;
    }
  }
  return status;
}

void openTIDAL_Cleanup()
{
  curl_exit_auth();
  curl_exit();

  free(config.newStream);
  cJSON_Delete((cJSON *) config.refreshRequest);
  cJSON_Delete((cJSON *) config.tokenRequest);
  cJSON_Delete((cJSON *) config.stream);
}

char *create_persistent_stream()
{
  char *string = NULL;
  cJSON *authorization_json = NULL;
  cJSON *user_json = NULL;
  cJSON *preferences_json = NULL;
  cJSON *refresh_token_json = NULL;
  cJSON *access_token_json = NULL;
  cJSON *expires_in_json = NULL;
  cJSON *id_json = NULL;
  cJSON *username_json = NULL;
  cJSON *country_code_json = NULL;
  cJSON *audio_quality_json = NULL;
  cJSON *video_quality_json = NULL;

  cJSON *output_json = cJSON_CreateObject();

  authorization_json = cJSON_CreateObject();
  cJSON_AddItemToObject(output_json, "authorization", authorization_json);
  user_json = cJSON_CreateObject();
  cJSON_AddItemToObject(output_json, "user", user_json);
  preferences_json = cJSON_CreateObject();
  cJSON_AddItemToObject(output_json, "preferences", preferences_json);

  refresh_token_json = cJSON_CreateString(config.refreshToken);
  cJSON_AddItemToObject(authorization_json, "refresh_token", refresh_token_json);
  access_token_json = cJSON_CreateString(config.accessToken);
  cJSON_AddItemToObject(authorization_json, "access_token", access_token_json);
  expires_in_json = cJSON_CreateNumber(config.expiresIn);
  cJSON_AddItemToObject(authorization_json, "expires_in", expires_in_json);

  id_json = cJSON_CreateNumber(config.userId);
  cJSON_AddItemToObject(user_json, "id", id_json);
  username_json = cJSON_CreateString(config.username);
  cJSON_AddItemToObject(user_json, "username", username_json);
  country_code_json = cJSON_CreateString(config.countryCode);
  cJSON_AddItemToObject(user_json, "country_code", country_code_json);

  audio_quality_json = cJSON_CreateString(config.audioQuality);
  cJSON_AddItemToObject(preferences_json, "audio_quality", audio_quality_json);
  video_quality_json = cJSON_CreateString(config.videoQuality);
  cJSON_AddItemToObject(preferences_json, "video_quality", video_quality_json);

  string = cJSON_Print(output_json); /* Allocate Memory (needs to be deallocated)  */
  config.newStream = string;
  cJSON_Delete(output_json);
  return string;
}

void read_persistent_stream(cJSON *input_json)
{
  cJSON *authorization = NULL;
  cJSON *refreshToken = NULL;
  cJSON *accessToken = NULL;
  cJSON *expiresIn = NULL;
  
  cJSON *user = NULL;
  cJSON *id = NULL;
  cJSON *countryCode = NULL;

  cJSON *preferences = NULL;
  cJSON *audioQuality = NULL;
  cJSON *videoQuality = NULL;
   
  authorization = cJSON_GetObjectItem(input_json, "authorization");
  refreshToken = cJSON_GetObjectItemCaseSensitive(authorization, "refresh_token");
  accessToken = cJSON_GetObjectItemCaseSensitive(authorization, "access_token");
  expiresIn = cJSON_GetObjectItem(authorization, "expires_in");

  user = cJSON_GetObjectItem(input_json, "user");
  id = cJSON_GetObjectItem(user, "id");
  countryCode = cJSON_GetObjectItemCaseSensitive(user, "country_code");
  
  preferences = cJSON_GetObjectItem(input_json, "preferences");
  audioQuality = cJSON_GetObjectItemCaseSensitive(preferences, "audio_quality");
  videoQuality = cJSON_GetObjectItemCaseSensitive(preferences, "video_quality");
 
  config.refreshToken = refreshToken->valuestring;
  config.accessToken = accessToken->valuestring;
  config.expiresIn = expiresIn->valueint;
  config.userId = id->valueint;
  config.countryCode = countryCode->valuestring;
  config.audioQuality = audioQuality->valuestring;
  config.videoQuality = videoQuality->valuestring;
}

int scan_persistent()
{
  FILE *persistentJSON;
  long streamSize;
  char *stream;
  int error = 0;

  /* open persistentFile  */
  persistentJSON = fopen(config.location, "rb");
  if (!persistentJSON)
  {
    fprintf(stderr, "[OAuth] File not found. Authenticate!\n");
    error = 1;
    goto end;
  }

  fseek(persistentJSON, 0L, SEEK_END);
  streamSize = ftell(persistentJSON);
  rewind(persistentJSON);

  /* allocate memory for entire content */
  stream = calloc( 1, streamSize+1 );
  if( !stream )
  {
    fclose(persistentJSON);
    fprintf(stderr, "memory alloc fails");
    error = 1;
    goto end;
  }

  /* copy the file into the buffer */
  if( 1!=fread( stream , streamSize, 1 , persistentJSON) )
  {
    fclose(persistentJSON);
    free(stream);
    fprintf(stderr, "entire read fails");
    error = 1;
    goto end;
  }
  fclose(persistentJSON);

  /* parse JSON stream  */
  cJSON *input_json = json_parse(stream);
  read_persistent_stream(input_json);
  config.stream = input_json;
  
end:
  if (error == 1)
  {
    fprintf(stderr, "[OAuth] Scan persistentFile failed!\n");
    return 0;
  }
  return 1;
}

void create_persistent()
{
  FILE *fp;
  fp = fopen(config.location, "w");
  if (fp != NULL)
  {
    char *json = create_persistent_stream();
    fprintf(fp, "%s", json);
  }
  else
  {
    fprintf(stderr, "[OAuth] Failed to create persistentFile!\n");
  }
  fclose(fp);
}

void refresh_persistent()
{
  /* start OAuth refresh routine  */
  time_t currentTime = time(NULL);
  size_t skip = 0;
  double diff_t;
  if (config.demoEnabled == 1)
  {
    goto end;
  }
  /* Check if ExpiryDate is in the future  */
  if (currentTime > config.expiresIn)
  {
    diff_t = difftime(currentTime, config.expiresIn);
  }
  else
  {
    diff_t = difftime(config.expiresIn, currentTime);
  }
  //printf("Difference Size_t: %zu\n", (size_t) diff_t);

  /* If ExpiryDate is in the future with a difference of more than 5min  */
  if (currentTime < config.expiresIn && (size_t) diff_t >= 300)
  {
    //printf("[OAuth] Renewal not necessary.\n");
  }
  /* Start renewal process  */
  else
  {
    openTIDAL res = openTIDAL_RefreshLoginToken(config.refreshToken);
    if (res.status == 1)
    {
      FILE *fp;
      config.accessToken = res.token.access_token;
      config.userId = res.token.userId;
      config.expiresIn = time(NULL) + 604800; /* Calculate new ExpiryDate */

      fp = fopen(config.location, "w");
      if (fp != NULL)
      {
        char *json = create_persistent_stream();
        fprintf(fp, "%s", json);
      }
      else
      {
        fprintf(stderr, "[OAuth] Failed to create persistentFile!\n");
      }
      fclose(fp);
      cJSON_Delete((cJSON *) config.refreshRequest);
      config.refreshRequest = res.json;
    }
    else
    {
      fprintf(stderr, "[OAuth] Auto Refresh failed.\n");
      config.demoEnabled = 1;
    }
  }
end:
  skip = 1;
  if (skip == 1)
  {
    //fprintf(stderr, "[OAuth] Auto Refresh skipped.\n");
  }
}
