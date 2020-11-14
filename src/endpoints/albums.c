#include <stdio.h>
#include <stdlib.h>
#include "../include/parse.h"
#include "../include/handles.h"
#include "../include/openTIDAL.h"

album_model get_album(size_t albumid)
{
  album_model Value;
  char *endpoint;
  char baseparams[20];
  
  endpoint = url_cat("albums/", albumid, "", 0);
  snprintf(baseparams, 20, "countryCode=%s", countryCode);
  curl_model req = curl_get(endpoint, baseparams);
  free(endpoint);
  if (req.status != -1)
  {
    cJSON *input_json = json_parse(req.body);
    if (req.responseCode == 200)
    {
      json_album_model processed_json = json_parse_album(input_json);
      Value = parse_album_values(processed_json, 0);
      Value.status = 1;
      Value.arraySize = 1;

      free(req.body);
      cJSON_Delete(input_json);
      return Value;
    }
    else
    {
      Value.status = parse_status(input_json, req, albumid, NULL);
      free(req.body);
      cJSON_Delete(input_json);
      return Value;
    }
  }
  else
  {
    free(req.body);
    Value.status = -1;
    fprintf(stderr, "[Request Error] Album %zu: CURLE_OK Check failed.\n", albumid);
    return Value;
  }
}

items_model get_album_items(size_t albumid, size_t limit, size_t offset)
{
  items_model Value;
  char *endpoint;
  char baseparams[50];
  
  endpoint = url_cat("albums/", albumid, "/items", 0);
  snprintf(baseparams, 50, "countryCode=%s&limit=%zu&offset=%zu", countryCode,
            limit, offset);

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
      Value.totalNumberOfItems = totalNumberOfItems->valueint;
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

      Value.arraySize = cJSON_GetArraySize(items);
      Value.status = 1;
      parse_number(limit, &Value.limit);
      parse_number(offset, &Value.offset);
      parse_number(totalNumberOfItems, &Value.totalNumberOfItems);

      cJSON_Delete(input_json);
      free(req.body);
      return Value;
    }
    else
    {
      Value.status = parse_status(input_json, req, albumid, NULL);
      free(req.body);
      cJSON_Delete(input_json);
      return Value;
    }
  }
  else
  {
    free(req.body);
    Value.status = -1;
    fprintf(stderr, "[Request Error] Album %zu: CURLE_OK Check failed.\n", albumid);
    return Value;
  }
}

album_model
get_user_album(size_t limit, size_t offset, char *order, char *orderDirection)
{
  album_model Value;
  char *endpoint = url_cat("users/", userId, "/favorites/albums", 0);
  
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

          json_album_model processed_json = json_parse_album(innerItem);
          Value = parse_album_values(processed_json, i);
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

