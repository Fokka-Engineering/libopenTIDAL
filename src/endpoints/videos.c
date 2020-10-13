#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/openTIDAL.h"

contributor_model get_video_contributors(size_t videoid)
{
  contributor_model Value;
  char buffer[50];
  snprintf(buffer, 50, "videos/%zu/contributors", videoid);
  char *baseparams = param_cat("100", "", "");
  
  curl_model req = curl_get(buffer, baseparams);
  free(baseparams);
  if(req.status != -1)
  {
    cJSON *input_json = json_parse(req.body);
    if (req.responseCode == 200)
    {
      Value.status = 1;
      size_t i = 0;
      cJSON *items = cJSON_GetObjectItemCaseSensitive(input_json, "items");
      cJSON *item = NULL;
      cJSON_ArrayForEach(item, items)
      {
        cJSON *name = cJSON_GetObjectItemCaseSensitive(item, "name");
        cJSON *role = cJSON_GetObjectItemCaseSensitive(item, "role");
        
	if (cJSON_IsString(name))
	{
          strncpy(Value.name[i], name->valuestring, sizeof(Value.name[i]));
	}
	if (cJSON_IsString(role))
	{
	  strncpy(Value.role[i], role->valuestring, sizeof(Value.role[i]));
	}
	i = i + 1;
      }
      free(req.body);
      cJSON_Delete(input_json);
      return Value;
    }
    else if (req.responseCode == 401)
    {
      Value.status = parse_unauthorized(input_json, videoid);
      free(req.body);
      cJSON_Delete(input_json);
      return Value;
    }
    else if (req.responseCode == 404)
    {
      Value.status = parse_notfound(input_json, videoid, NULL);
      free(req.body);
      cJSON_Delete(input_json);
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
    Value.status = -1;
    free(req.body);
    fprintf(stderr, "[Request Error] Video %zu: CURLE_OK Check failed.", videoid);
    return Value;
  }
}
stream_model get_video_streamUrl(size_t videoid)
{
  stream_model Value;
  char buffer[50];
  snprintf(buffer, 50, "videos/%zu/streamUrl", videoid);
  char *baseparams = param_cat("100", "", "");
  
  curl_model req = curl_get(buffer, baseparams);
  free(baseparams);
  if (req.status != -1)
  {
    cJSON *input_json = json_parse(req.body);
    if (req.responseCode == 200)
    {
      Value.status = 1;
      cJSON *url = cJSON_GetObjectItemCaseSensitive(input_json, "url");
      cJSON *videoQuality = cJSON_GetObjectItemCaseSensitive(input_json, "videoQuality");

      strncpy(Value.url, url->valuestring, sizeof(Value.url));
      strncpy(Value.videoQuality, videoQuality->valuestring, sizeof(Value.videoQuality));
      return Value;
    }
    else if (req.responseCode == 401)
    {
      Value.status = parse_unauthorized(input_json, videoid);
      free(req.body);
      cJSON_Delete(input_json);
      return Value;
    }
    else if (req.responseCode == 404)
    {
      Value.status = parse_notfound(input_json, videoid, NULL);
      free(req.body);
      cJSON_Delete(input_json);
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
    Value.status = -1;
    free(req.body);
    fprintf(stderr, "[Request Error] Track %zu: CURLE_OK Check failed.\n", videoid);
    return Value;
  }
}

items_model get_video(size_t videoid)
{
  items_model Value;
  char buffer[20];
  snprintf(buffer, 20, "videos/%zu/", videoid);
  char *baseparams = param_cat("100", "", "");
  curl_model req = curl_get(buffer, baseparams);
  free(baseparams);
  if (req.status != -1)
  {
    cJSON *input_json = json_parse(req.body);
    if (req.responseCode == 200)
    {
      items_model parse = parse_videos(input_json);
      cJSON_Delete(input_json);
      free(req.body);
      return parse;
    }
    else if (req.responseCode == 401)
    {
      Value.status = parse_unauthorized(input_json, videoid);
      cJSON_Delete(input_json);
      free(req.body);
      return Value;
    }
    else if (req.responseCode == 404)
    {
      Value.status = parse_notfound(input_json, videoid, NULL);
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
    Value.status = -1;
    free(req.body);
    fprintf(stderr, "[Request Error] Track %zu: CURLE_OK Check failed.", videoid);
    return Value;
  }
}
