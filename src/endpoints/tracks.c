#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/base64.h"
#include "../include/openTIDAL.h"

contributor_model get_track_contributors(size_t trackid, size_t limit, size_t offset)
{
  contributor_model Value;
  char *endpoint = url_cat("tracks/", trackid, "/contributors", 0);
  
  char baseparams[50];
  snprintf(baseparams, 50, "countryCode=%s&limit=%zu&offset=%zu", countryCode, limit, offset);

  curl_model req = curl_get(endpoint, baseparams);
  free(endpoint);
  
  if (req.status != -1)
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
    else if (req.responseCode == 401)
    {
      Value.status = parse_unauthorized(input_json, trackid);
      free(req.body);
      cJSON_Delete(input_json);
      return Value;
    }
    else if (req.responseCode == 404)
    {
      Value.status = parse_notfound(input_json, trackid, NULL);
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
    fprintf(stderr, "[Request Error] Track %zu: CURLE_OK Check failed.", trackid);
    return Value;
  }
}

mix_model get_track_mix(size_t trackid)
{
  mix_model Value;
  char *endpoint = url_cat("tracks/", trackid, "/mix", 0);
  
  char baseparams[20];
  snprintf(baseparams, 20, "countryCode=%s", countryCode);
  
  curl_model req = curl_get(endpoint, baseparams);
  free(endpoint);

  if (req.status != -1)
  {
    cJSON *input_json = json_parse(req.body);
    if (req.responseCode == 200)
    {
      Value.status = 1;
      cJSON *id = cJSON_GetObjectItemCaseSensitive(input_json, "id");

      strncpy(Value.id, id->valuestring, sizeof(Value.id));
      free(req.body);
      cJSON_Delete(input_json);
      return Value;
    }
    else if (req.responseCode == 401)
    {
      Value.status = parse_unauthorized(input_json, trackid);
      free(req.body);
      cJSON_Delete(input_json);
      return Value;
    }
    else if (req.responseCode == 404)
    {
      Value.status = parse_notfound(input_json, trackid, NULL);
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
    free(req.body);
    Value.status = -1;
    fprintf(stderr, "[Request Error] Track %zu: CURLE_OK Check failed.", trackid);
    return Value;
  }
}

stream_model get_track_stream(size_t trackid)
{
  stream_model Value;
  char *endpoint = url_cat("tracks/", trackid, "/playbackinfopostpaywall", 0);
  char buffer[100];
  
  snprintf(buffer, 100, "countryCode=%s&audioquality=%s&assetpresentation=%s&playbackmode=%s",
            countryCode, audioQuality, "FULL", "STREAM");
  curl_model req = curl_get(endpoint, buffer);
  free(endpoint);
  
  if (req.status != -1)
  {
    cJSON *input_json = json_parse(req.body);
    if (req.responseCode == 200)
    {
      cJSON *trackId = cJSON_GetObjectItem(input_json, "trackId");
      cJSON *assetPresentation = cJSON_GetObjectItemCaseSensitive(input_json, "assetPresentation");
      cJSON *audioMode = cJSON_GetObjectItemCaseSensitive(input_json, "audioMode");
      cJSON *audioQuality = cJSON_GetObjectItemCaseSensitive(input_json, "audioQuality");
      cJSON *manifestMimeType = cJSON_GetObjectItemCaseSensitive(input_json, "manifestMimeType");
      cJSON *manifest = cJSON_GetObjectItemCaseSensitive(input_json, "manifest");
      char manifest_decoded[1024];

      Value.trackId = trackId->valueint;
      strncpy(Value.assetPresentation, assetPresentation->valuestring, sizeof(Value.assetPresentation));
      strncpy(Value.audioMode, audioMode->valuestring, sizeof(Value.audioMode));
      strncpy(Value.audioQuality, audioQuality->valuestring, sizeof(Value.audioQuality)); 
      
      if (strcmp(manifestMimeType->valuestring, "application/vnd.tidal.bts") == 0)
      {
        Value.status = 1;
        Base64decode(manifest_decoded, manifest->valuestring);
        cJSON *manifest_json = json_parse(manifest_decoded);
	cJSON *mimeType = cJSON_GetObjectItemCaseSensitive(manifest_json, "mimeType");
	cJSON *codecs = cJSON_GetObjectItemCaseSensitive(manifest_json, "codecs");
	cJSON *urls = cJSON_GetObjectItemCaseSensitive(manifest_json, "urls");
	cJSON *url = cJSON_GetArrayItem(urls, 0);
	
	strncpy(Value.mimeType, mimeType->valuestring, sizeof(Value.mimeType));
	strncpy(Value.codec, codecs->valuestring, sizeof(Value.codec));
	strncpy(Value.url, url->valuestring, sizeof(Value.url));
      }
      else
      {
        Value.status = -10;
	fprintf(stderr, "[Request Error] Not a valid manifest. MimeType is not application/vnd.tidal.bts\n");
      }
      return Value;
    }
    else if (req.responseCode == 400)
    {
      Value.status = parse_badrequest(input_json, trackid, NULL);
      free(req.body);
      cJSON_Delete(input_json);
      return Value;
    }
    else if (req.responseCode == 401)
    {
      Value.status = parse_unauthorized(input_json, trackid);
      free(req.body);
      cJSON_Delete(input_json);
      return Value;
    }
    else if (req.responseCode == 404)
    {
      Value.status = parse_notfound(input_json, trackid, NULL);
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
    fprintf(stderr, "[Request Error] Track %zu: CURLE_OK Check failed.\n", trackid);
    return Value;
  }
}

items_model get_track(size_t trackid)
{
  items_model Value;
  char *endpoint = url_cat("tracks/", trackid, "", 0);
  
  char baseparams[20];
  snprintf(baseparams, 20, "countryCode=%s", countryCode);

  curl_model req = curl_get(endpoint, baseparams);
  free(endpoint);
  if (req.status != -1)
  {
    cJSON *input_json = json_parse(req.body);
    if (req.responseCode == 200)
    {
      items_model parse = parse_tracks(input_json);
      cJSON_Delete(input_json);
      free(req.body);
      return parse;
    }
    else if (req.responseCode == 401)
    {
      Value.status = parse_unauthorized(input_json, trackid);
      cJSON_Delete(input_json);
      free(req.body);
      return Value;
    }
    else if (req.responseCode == 404)
    {
      Value.status = parse_notfound(input_json, trackid, NULL);
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
    fprintf(stderr, "[Request Error] Track %zu: CURLE_OK Check failed.", trackid);
    return Value;
  }
}
