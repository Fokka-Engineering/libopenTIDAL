#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/base64.h"
#include "../include/parse.h"
#include "../include/openTIDAL.h"

contributor_model get_video_contributors(size_t videoid, size_t limit, size_t offset)
{
  contributor_model Value;
  char buffer[50];
  char baseparams[50];

  snprintf(buffer, 50, "videos/%zu/contributors", videoid);
  snprintf(baseparams, 50, "countryCode=%s&limit=%zu&offset=%zu", countryCode, 
            limit, offset);
  curl_model req = curl_get(buffer, baseparams);
  
  if(req.status != -1)
  {
    cJSON *input_json = json_parse(req.body);
    if (req.responseCode == 200)
    {
      Value.status = 1;
      size_t i = 0;
      cJSON *items = cJSON_GetObjectItem(input_json, "items");
      cJSON *limit = cJSON_GetObjectItem(input_json, "limit");
      cJSON *offset = cJSON_GetObjectItem(input_json, "offset");
      cJSON *totalNumberOfItems = cJSON_GetObjectItem(input_json, "totalNumberOfItems");
      cJSON *item = NULL;

      Value.limit = limit->valueint;
      Value.offset = offset->valueint;
      Value.totalNumberOfItems = totalNumberOfItems->valueint;
      Value.arraySize = cJSON_GetArraySize(items);
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
    else
    {
      Value.status = parse_status(input_json, req, videoid, NULL);
      free(req.body);
      cJSON_Delete(input_json);
      return Value;
    }
  }
  else
  {
    Value.status = -1;
    free(req.body);
    fprintf(stderr, "[Request Error] Video %zu: CURLE_OK Check failed.\n", videoid);
    return Value;
  }
}

items_model get_video(size_t videoid)
{
  items_model Value;
  char buffer[20];
  snprintf(buffer, 20, "videos/%zu", videoid);
  
  char baseparams[20];
  snprintf(baseparams, 20, "countryCode=%s", countryCode);
  
  curl_model req = curl_get(buffer, baseparams);
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
    else
    {
      Value.status = parse_status(input_json, req, videoid, NULL);
      cJSON_Delete(input_json);
      free(req.body);
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

stream_model get_video_stream(size_t videoid)
{
  stream_model Value;
  char endpoint[80];
  char baseparams[100];

  snprintf(endpoint, 80, "videos/%zu/playbackinfopostpaywall", videoid);
  snprintf(baseparams, 100, "countryCode=%s&videoquality=%s&assetpresentation=%s&playbackmode=%s",
            countryCode, videoQuality, "FULL", "STREAM");
  curl_model req = curl_get(endpoint, baseparams);
  if (req.status != -1)
  {
    cJSON *input_json = json_parse(req.body);
    if (req.responseCode == 200)
    {
      cJSON *videoId = cJSON_GetObjectItem(input_json, "videoId");
      cJSON *assetPresentation = cJSON_GetObjectItemCaseSensitive(input_json, "assetPresentation");
      cJSON *videoQuality = cJSON_GetObjectItemCaseSensitive(input_json, "videoQuality");
      cJSON *manifestMimeType = cJSON_GetObjectItemCaseSensitive(input_json, "manifestMimeType");
      cJSON *manifest = cJSON_GetObjectItemCaseSensitive(input_json, "manifest");
      char manifest_decoded[1024];

      Value.videoId = videoId->valueint;
      strncpy(Value.assetPresentation, assetPresentation->valuestring, sizeof(Value.assetPresentation));
      strncpy(Value.videoQuality, videoQuality->valuestring, sizeof(Value.videoQuality));

      if (strcmp(manifestMimeType->valuestring, "application/vnd.tidal.emu") == 0)
      {
        Value.status = 1;
        Base64decode(manifest_decoded, manifest->valuestring);
        cJSON *manifest_json = json_parse(manifest_decoded);
        cJSON *mimeType = cJSON_GetObjectItemCaseSensitive(manifest_json, "mimeType");
        cJSON *urls = cJSON_GetObjectItemCaseSensitive(manifest_json, "urls");
        cJSON *url = cJSON_GetArrayItem(urls, 0);

        strncpy(Value.mimeType, mimeType->valuestring, sizeof(Value.mimeType));
        strncpy(Value.url, url->valuestring, sizeof(Value.url));
      }
      else
      {
        Value.status = -10;
        fprintf(stderr, "[Request Error] Not a valid manifest. MimeType is not application/vnd.tidal.emu\n");
      }
      return Value;
    }
    else
    {
      Value.status = parse_status(input_json, req, videoid, NULL);
      free(req.body);
      cJSON_Delete(input_json);
      return Value;
    }
  }
  else
  {
    Value.status = -1;
    free(req.body);
    fprintf(stderr, "[Request Error] Video %zu: CURLE_OK Check failed.\n", videoid);
    return Value;
  }
}
