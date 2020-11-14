#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/base64.h"
#include "../include/parse.h"
#include "../include/handles.h"
#include "../include/openTIDAL.h"

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
      json_items_model processed_json = json_parse_items(input_json);
      Value = parse_items_values(processed_json, 0);
      Value.status = 1;
      Value.arraySize = 1;

      cJSON_Delete(input_json);
      free(req.body);
      return Value;
    }
    else
    {
      Value.status = parse_status(input_json, req, trackid, NULL);
      cJSON_Delete(input_json);
      free(req.body);
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

items_model
get_user_tracks(size_t limit, size_t offset, char *order, char *orderDirection)
{
  items_model Value;
  char *endpoint = url_cat("users/", userId, "/favorites/tracks", 0);
  
  char baseparams[150];
  snprintf(baseparams, 150, "countryCode=%s&limit=%zu&offset%zu&order%s&orderDirection=%s",
             countryCode, limit, offset, order, orderDirection);

  curl_model req = curl_get(endpoint, baseparams);
  free(endpoint);
  if (req.status != -1)
  {
    cJSON *input_json = json_parse(req.body);
    if (req.responseCode == 200)
    {
      cJSON *items = cJSON_GetObjectItem(input_json, "items");
      cJSON *item = NULL;
      cJSON *limit = cJSON_GetObjectItem(input_json, "limit");
      cJSON *offset = cJSON_GetObjectItem(input_json, "offset");
      cJSON *totalNumberOfItems = cJSON_GetObjectItem(input_json, "totalNumberOfItems");

      size_t i = 0;

      if (cJSON_IsArray(items))
      {
        cJSON_ArrayForEach(item, items)
        {
          cJSON *innerItem = cJSON_GetObjectItem(item, "item");

          json_items_model processed_json = json_parse_items(innerItem);
          Value = parse_items_values(processed_json, i);
  	  i += 1;
        }
      }
      
      parse_number(limit, &Value.limit);
      parse_number(offset, &Value.offset);
      parse_number(totalNumberOfItems, &Value.totalNumberOfItems);
      Value.arraySize = cJSON_GetArraySize(items);
      Value.status = 1;
 
      free(req.body);
      cJSON_Delete(input_json);
      return Value;
    }
    else
    {
      Value.status = parse_status(input_json, req, userId, NULL);
      free(req.body);
      cJSON_Delete(input_json);
      return Value;
    }
  }
  else
  {
    Value.status = -1;
    free(req.body);
    fprintf(stderr, "[Request Error] User %zu: CURLE_OK Check failed.", userId);
    return Value;
  }
}

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
      cJSON *limit = cJSON_GetObjectItem(input_json, "limit");
      cJSON *offset = cJSON_GetObjectItem(input_json, "offset");
      cJSON *totalNumberOfItems = cJSON_GetObjectItem(input_json, "totalNumberOfItems");
      cJSON *items = cJSON_GetObjectItem(input_json, "items");
      cJSON *item = NULL;
      size_t i = 0;

      if (cJSON_IsArray(items))
      {
        Value.arraySize = cJSON_GetArraySize(items);
	cJSON_ArrayForEach(item, items)
        {
          json_contributor_model processed_json = json_parse_contributors(item);
	  Value = parse_contributor_values(processed_json, i);
	  i += 1;
	}
      }
      
      parse_number(limit, &Value.limit); 
      parse_number(offset, &Value.offset);
      parse_number(totalNumberOfItems, &Value.totalNumberOfItems);
      Value.status = 1;
      Value.arraySize = cJSON_GetArraySize(items);

      free(req.body);
      cJSON_Delete(input_json);
      return Value;
    }
    else
    {
      Value.status = parse_status(input_json, req, trackid, NULL);
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
      json_mix_model processed_json = json_parse_mix(input_json);
      Value = parse_mix_values(processed_json);
      Value.status = 1;

      free(req.body);
      cJSON_Delete(input_json);
      return Value;
    }
    else
    {
      Value.status = parse_status(input_json, req, trackid, NULL);
      free(req.body);
      cJSON_Delete(input_json);
      return Value;
    }
  }
  else
  {
    free(req.body);
    Value.status = -1;
    fprintf(stderr, "[Request Error] Track %zu: CURLE_OK Check failed.\n", trackid);
    return Value;
  }
}

stream_model get_track_stream(size_t trackid)
{
  stream_model Value;
  char *endpoint = url_cat("tracks/", trackid, "/playbackinfopostpaywall", 0);
  char buffer[200];
  
  snprintf(buffer, 200, "countryCode=%s&audioquality=%s&assetpresentation=%s&playbackmode=%s",
            countryCode, audioQuality, "FULL", "STREAM");
  curl_model req = curl_get(endpoint, buffer);
  free(endpoint);
  
  if (req.status != -1)
  {
    cJSON *input_json = json_parse(req.body);
    if (req.responseCode == 200)
    {
      json_stream_model processed_json = json_parse_stream(input_json);
      Value = parse_stream_values(processed_json);     
      Value.status = 0;
      char manifest_decoded[1024];

      if (strcmp(Value.manifestMimeType, "application/vnd.tidal.bts") == 0)
      {
        Value.status = 1;
	Base64decode(manifest_decoded, Value.manifest);
	cJSON *manifest_json = json_parse(manifest_decoded);
	json_manifest_model processed_manifest = json_parse_manifest(manifest_json);
        
        parse_string(processed_manifest.mimeType, Value.manifestMimeType, sizeof(Value.manifestMimeType));
        parse_string(processed_manifest.codec, Value.codec, sizeof(Value.codec));
        parse_string(processed_manifest.encryptionType, Value.encryptionType, sizeof(Value.encryptionType));
        parse_string(processed_manifest.url, Value.url, sizeof(Value.url));
	
	cJSON_Delete(manifest_json);
      }
      else
      {
        Value.status = -10;
	fprintf(stderr, "[Request Error] Not a valid manifest. MimeType is not application/vnd.tidal.bts\n");
      }

      cJSON_Delete(input_json);
      free(req.body);
      return Value;
    }
    else
    {
      Value.status = parse_status(input_json, req, trackid, NULL);
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

/* create & delete favourites */

int add_user_track(size_t trackid)
{
  char *endpoint = url_cat("users/", userId, "/favorites/tracks", 1);
  int status;
  char buffer[60];
  snprintf(buffer, 60, "trackIds=%zu&onArtifactNotFound=FAIL", trackid);

  curl_model req = curl_post(endpoint, buffer, "");
  free(endpoint);
  free(req.body);
  if (req.status != -1)
  {
    if (req.responseCode == 200)
    {
      return 1;
    }
    else if (req.responseCode == 400)
    {
      status = -11;
      return status;
    }
    else if (req.responseCode == 401)
    {
      status = -8;
      return status;
    }
    else if (req.responseCode == 404)
    {
      status = -2;
      return status;
    }
    else
    {
      status = 0;
      return status;
    }
  }
  else
  {
    return -1;
  }
}

int delete_user_track(size_t trackid)
{
  int status;
  char buffer[80];
  snprintf(buffer, 80, "users/%zu/favorites/tracks/%zu?countryCode=%s", userId, trackid, countryCode);

  curl_model req = curl_delete(buffer, "", "");
  /*Cleanup*/
  free(req.body);

  if (req.status != -1)
  {
    if (req.responseCode == 200)
    {
      return 1;
    }
    else if (req.responseCode == 400)
    {
      status = -11;
      return status;
    }
    else if (req.responseCode == 401)
    {
      status = -8;
      return status;
    }
    else if (req.responseCode == 404)
    {
      status = -2;
      return status;
    }
    else
    {
      status = 0;
      return status;
    }
  }
  else
  {
    return -1;
  }
}

