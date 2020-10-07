#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/cTidal.h"

artist_model get_artist(size_t artistid)
{
  artist_model Value;
  char *endpoint = url_cat("artists/", artistid, "", 0);
  char *baseparams = param_cat("100", "", "");
  char *req = curl_get(endpoint, baseparams, 0);
  if (req != 0)
  {
    cJSON *input_json = json_parse(req);
    return parse_artist(input_json, 0);
    /* always cleanup */
    cJSON_Delete(input_json);
  }
  else
  {
    printf("%s\n", "Request Error: cURL returned a 4xx status code. Authorization Error.");
    Value.status = -1;
    return Value;
  }
  /*Cleanup*/
  free(endpoint);
  free(baseparams);
  free(req);
}

artist_link_model get_artist_link(size_t artistid)
{
  const cJSON *item = NULL;
  const cJSON *items = NULL;
  int i = 0;
  artist_link_model Value;
  char *endpoint = url_cat("artists/", artistid, "/links", 0);
  char *baseparams = param_cat("100", "", "");
  char *req = curl_get(endpoint, baseparams, 0);
  if (req != 0)
  {
    cJSON *input_json = json_parse(req);
    Value.status = 1;
    Value.totalNumberOfItems = cJSON_GetObjectItemCaseSensitive(input_json, "totalNumberOfItems")->valueint;
    strcpy(Value.source, cJSON_GetObjectItemCaseSensitive(input_json, "source")->valuestring);
    items = cJSON_GetObjectItemCaseSensitive(input_json, "items");
    Value.arraySize = cJSON_GetArraySize(items);

    cJSON_ArrayForEach(item, items)
    {
      strcpy(Value.url[i], cJSON_GetObjectItemCaseSensitive(item, "url")->valuestring);
      strcpy(Value.siteName[i], cJSON_GetObjectItemCaseSensitive(item, "siteName")->valuestring);
      i = i + 1;
    }
    return Value;
    /* always cleanup */
    cJSON_Delete(input_json);
  }
  else
  {
    printf("%s\n", "Request Error: cURL returned a 4xx status code. Authorization Error.");
    Value.status = -1;
    return Value;
  }
  /*Cleanup*/
  free(endpoint);
  free(baseparams);
  free(req);
}

artist_mix_model get_artist_mix(size_t artistid)
{
  artist_mix_model Value;
  char *endpoint = url_cat("artists/", artistid, "/mix", 0);
  char *baseparams = param_cat("100", "", "");
  char *req = curl_get(endpoint, baseparams, 0);
  if (req != 0)
  {
    cJSON *input_json = json_parse(req);
    strcpy(Value.id, cJSON_GetObjectItemCaseSensitive(input_json, "id")->valuestring);
    return Value;
    /* always cleanup */
    cJSON_Delete(input_json);
  }
  else
  {
    printf("%s\n", "Request Error: cURL returned a 4xx status code. Authorization Error.");
    Value.status = -1;
    return Value;
  }
  /*Cleanup*/
  free(endpoint);
  free(baseparams);
  free(req);
}

items_model get_artist_toptracks(size_t artistid)
{
  items_model Value;
  char *endpoint = url_cat("artists/", artistid, "/toptracks", 0);
  char *baseparams = param_cat("100", "", "");
  char *req = curl_get(endpoint, baseparams, 0);
  if (req != 0)
  {
    cJSON *input_json = json_parse(req);
    return parse_items(input_json, 1, 0);
    /* always cleanup */
    cJSON_Delete(input_json);
  }
  else
  {
    printf("%s\n", "Request Error: cURL returned a 4xx status code. Authorization Error.");
    items_model Value;
    Value.status = -1;
    return Value;
  }
  /*Cleanup*/
  free(endpoint);
  free(baseparams);
  free(req);
}

items_model get_artist_videos(size_t artistid)
{
  items_model Value;
  char *endpoint = url_cat("artists/", artistid, "/videos", 0);
  char *baseparams = param_cat("100", "", "");
  char *req = curl_get(endpoint, baseparams, 0);
  if (req != 0)
  {
    cJSON *input_json = json_parse(req);
    return parse_items(input_json, 1, 1);
    /* always cleanup */
    cJSON_Delete(input_json);
  }
  else
  {
    printf("%s\n", "Request Error: cURL returned a 4xx status code. Authorization Error.");
    items_model Value;
    Value.status = -1;
    return Value;
  }
  /*Cleanup*/
  free(endpoint);
  free(baseparams);
  free(req);
}

album_model get_artist_albums(size_t artistid)
{
  album_model Value;
  char *endpoint = url_cat("artists/", artistid, "/albums", 0);
  char *baseparams = param_cat("100", "", "");
  char *req = curl_get(endpoint, baseparams, 0);
  if (req != 0)
  {
    cJSON *input_json = json_parse(req);
    return parse_album(input_json, 0);
    /* always cleanup */
    cJSON_Delete(input_json);
  }
  else
  {
    printf("%s\n", "Request Error: cURL returned a 4xx status code. Authorization Error.");
    album_model Value;
    Value.status = -1;
    return Value;
  }
  /*Cleanup*/
  free(endpoint);
  free(baseparams);
  free(req);
}
