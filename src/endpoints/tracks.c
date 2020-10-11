#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/cTidal.h"

tracks_contributor_model get_track_contributors(size_t trackid)
{
  tracks_contributor_model Value;
  char *endpoint = url_cat("tracks/", trackid, "/contributors", 0);
  char *baseparams = param_cat("100", "", "");
  char *req = curl_get(endpoint, baseparams);
  if (req != 0)
  {
    cJSON *input_json = json_parse(req);
    
    
    return Value;
    /* always cleanup */
    cJSON_Delete(input_json);
  }
  else
  {
    Value.status = -1;
    return Value;
    printf("%s\n", "Request Error: cURL returned a 4xx status code. Authorization Error.");
  }
  /*Cleanup*/
  free(endpoint);
  free(baseparams);
  free(req);
}

mix_model get_track_mix(size_t trackid) /* TODO: Improve Error Handling (Differentiate between Status Codes)  */
{
  mix_model Value;
  char *endpoint = url_cat("tracks/", trackid, "/mix", 0);
  char *baseparams = param_cat("100", "", "");
  char *req = curl_get(endpoint, baseparams);
  if (req != 0)
  {
    cJSON *input_json = json_parse(req);
    return Value;
    /* always cleanup */
    cJSON_Delete(input_json);
  }
  else
  {
    Value.status = -1;
    return Value;
    printf("%s\n", "Request Error: cURL returned a 4xx status code. Authorization Error."); /* or 404  */
  }
  /*Cleanup*/
  free(endpoint);
  free(baseparams);
  free(req);
}

stream_model get_track_streamUrl(size_t trackid)
{
  stream_model Value;
  char *endpoint = url_cat("tracks/", trackid, "/streamUrl", 0);
  char *baseparams = param_cat("100", "", "");
  char *req = curl_get(endpoint, baseparams);
  if (req != 0)
  {
    cJSON *input_json = json_parse(req);
    strcpy(Value.url, cJSON_GetObjectItemCaseSensitive(input_json, "url")->valuestring);
    Value.trackId = cJSON_GetObjectItem(input_json, "trackId")->valueint;
    strcpy(Value.soundQuality, cJSON_GetObjectItemCaseSensitive(input_json, "soundQuality")->valuestring);
    strcpy(Value.codec, cJSON_GetObjectItemCaseSensitive(input_json, "codec")->valuestring);
    return Value;
    /* always cleanup */
    cJSON_Delete(input_json);
  }
  else
  {
    Value.status = -1;
    return Value;
    printf("%s\n", "Request Error: cURL returned a 4xx status code. Authorization Error.");
  }
  /*Cleanup*/
  free(endpoint);
  free(baseparams);
  free(req);
}

items_model get_track(size_t trackid)
{
  char *endpoint = url_cat("tracks/", trackid, "", 0);
  char *baseparams = param_cat("100", "", "");
  char *req = curl_get(endpoint, baseparams);
  if (req != 0)
  {
    cJSON *input_json = json_parse(req);
    return parse_tracks(input_json);
    /* always cleanup */
    cJSON_Delete(input_json);
  }
  else
  {
    items_model Value;
    Value.status = -1;
    return Value;
    printf("%s\n", "Request Error: cURL returned a 4xx status code. Authorization Error.");
  }
  /*Cleanup*/
  free(endpoint);
  free(baseparams);
  free(req);
}
