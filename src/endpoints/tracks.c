#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/openTIDAL.h"

tracks_contributor_model get_track_contributors(size_t trackid)
{
  tracks_contributor_model Value;
  char *endpoint = url_cat("tracks/", trackid, "/contributors", 0);
  char *baseparams = param_cat("100", "", "");
  curl_model req = curl_get(endpoint, baseparams);
  free(endpoint);
  free(baseparams); 
  if (req.status != -1)
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

        strncpy(Value.name[i], name->valuestring, sizeof(Value.name[i]));
	strncpy(Value.role[i], role->valuestring, sizeof(Value.role[i]));
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
  char *baseparams = param_cat("100", "", "");
  curl_model req = curl_get(endpoint, baseparams);
  free(endpoint);
  free(baseparams);

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

stream_model get_track_streamUrl(size_t trackid)
{
  stream_model Value;
  char *endpoint = url_cat("tracks/", trackid, "/streamUrl", 0);
  char *baseparams = param_cat("100", "", "");
  curl_model req = curl_get(endpoint, baseparams);
  free(endpoint);
  free(baseparams); 
  if (req.status != -1)
  {
    cJSON *input_json = json_parse(req.body);
    if (req.responseCode == 200)
    {
      Value.status = 1;
      cJSON *url = cJSON_GetObjectItemCaseSensitive(input_json, "url");
      cJSON *trackId = cJSON_GetObjectItem(input_json, "trackId");
      cJSON *soundQuality = cJSON_GetObjectItemCaseSensitive(input_json, "soundQuality");
      cJSON *codec = cJSON_GetObjectItemCaseSensitive(input_json, "codec");

      strncpy(Value.url, url->valuestring, sizeof(Value.url));
      Value.trackId = trackId->valueint;
      strncpy(Value.soundQuality, soundQuality->valuestring, sizeof(Value.soundQuality));
      strncpy(Value.codec, codec->valuestring, sizeof(Value.codec));
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
  char *baseparams = param_cat("100", "", "");
  curl_model req = curl_get(endpoint, baseparams);
  free(endpoint);
  free(baseparams); 
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
