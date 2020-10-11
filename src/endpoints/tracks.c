#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/cTidal.h"

tracks_contributor_model get_track_contributors(size_t trackid)
{
  tracks_contributor_model Value;
  char *endpoint = url_cat("tracks/", trackid, "/contributors", 0);
  char *baseparams = param_cat("100", "", "");
  curl_model req = curl_get(endpoint, baseparams);
  
  if (req.status != -1)
  {
    cJSON *input_json = json_parse(req.body);
    if (req.responseCode == 200)
    {
      Value.status = 1;
      return Value;
    }
    else if (req.responseCode == 401)
    {
      Value.status = parse_unauthorized(input_json, trackid);
      return Value;
    }
    else if (req.responseCode == 404)
    {
      return Value;
    }
    else
    {
      Value.status = 0;
      return Value;
    }
    /* always cleanup */
    cJSON_Delete(input_json);
  }
  else
  {
    Value.status = -1;
    return Value;
    fprintf(stderr, "[Request Error] Track %zu: CURLE_OK Check failed.", trackid);
  }
 
  /*Cleanup*/
  free(endpoint);
  free(baseparams);
  free(req.body);
}

mix_model get_track_mix(size_t trackid) /* TODO: Improve Error Handling (Differentiate between Status Codes)  */
{
  mix_model Value;
  char *endpoint = url_cat("tracks/", trackid, "/mix", 0);
  char *baseparams = param_cat("100", "", "");
  curl_model req = curl_get(endpoint, baseparams);
  
  if (req.status != -1)
  {
    cJSON *input_json = json_parse(req.body);
    if (req.responseCode == 200)
    {
      Value.status = 1;
      return Value;
    }
    else if (req.responseCode == 401)
    {
      Value.status = parse_unauthorized(input_json, trackid);
      return Value;
    }
    else if (req.responseCode == 404)
    {
      return Value;
    }
    else
    {
      Value.status = 0;
      return Value;
    }
    /* always cleanup */
    cJSON_Delete(input_json);
  }
  else
  {
    Value.status = -1;
    return Value;
    fprintf(stderr, "[Request Error] Track %zu: CURLE_OK Check failed.", trackid);
  }

  /*Cleanup*/
  free(endpoint);
  free(baseparams);
  free(req.body);
}

stream_model get_track_streamUrl(size_t trackid)
{
  stream_model Value;
  char *endpoint = url_cat("tracks/", trackid, "/streamUrl", 0);
  char *baseparams = param_cat("100", "", "");
  curl_model req = curl_get(endpoint, baseparams);
  
  if (req.status != -1)
  {
    cJSON *input_json = json_parse(req.body);
    if (req.responseCode == 200)
    {
      Value.status = 1;
      strcpy(Value.url, cJSON_GetObjectItemCaseSensitive(input_json, "url")->valuestring);
      Value.trackId = cJSON_GetObjectItem(input_json, "trackId")->valueint;
      strcpy(Value.soundQuality, cJSON_GetObjectItemCaseSensitive(input_json, "soundQuality")->valuestring);
      strcpy(Value.codec, cJSON_GetObjectItemCaseSensitive(input_json, "codec")->valuestring);
      return Value;
    }
    else if (req.responseCode == 401)
    {
      Value.status = parse_unauthorized(input_json, trackid);
      return Value;
    }
    else if (req.responseCode == 404)
    {
      return Value;
    }
    else
    {
      Value.status = 0;
      return Value;
    }
    /* always cleanup */
    cJSON_Delete(input_json);
  }
  else
  {
    Value.status = -1;
    return Value;
    fprintf(stderr, "[Request Error] Track %zu: CURLE_OK Check failed.", trackid);
  }
  /*Cleanup*/
  free(endpoint);
  free(baseparams);
  free(req.body);
}

items_model get_track(size_t trackid)
{
  items_model Value;
  char *endpoint = url_cat("tracks/", trackid, "", 0);
  char *baseparams = param_cat("100", "", "");
  curl_model req = curl_get(endpoint, baseparams);
  
  if (req.status != -1)
  {
    cJSON *input_json = json_parse(req.body);
    if (req.responseCode == 200)
    {
      return parse_tracks(input_json);
    }
    else if (req.responseCode == 401)
    {
      Value.status = parse_unauthorized(input_json, trackid);
      return Value;
    }
    else if (req.responseCode == 404)
    {
      Value.status = parse_notfound(input_json, trackid, NULL);
      return Value;
    }
    else
    {
      Value.status = 0;
      return Value;
    }
    /* always cleanup */
    cJSON_Delete(input_json);
  }
  else
  {
    Value.status = -1;
    return Value;
    fprintf(stderr, "[Request Error] Track %zu: CURLE_OK Check failed.", trackid);
  }
  /*Cleanup*/
  free(endpoint);
  free(baseparams);
  free(req.body);
}
