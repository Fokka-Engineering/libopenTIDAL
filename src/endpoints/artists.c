#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/parse.h"
#include "../include/openTIDAL.h"

artist_model get_artist(size_t artistid)
{
  artist_model Value;
  char *endpoint;
  char baseparams[20];
  
  endpoint = url_cat("artists/", artistid, "", 0);
  snprintf(baseparams, 20, "countryCode=%s", countryCode);
  curl_model req = curl_get(endpoint, baseparams);
  free(endpoint);
  
  if (req.status != -1)
  {
    cJSON *input_json = json_parse(req.body);
    if (req.responseCode == 200)
    {
      json_artist_model processed_json = json_parse_artist(input_json);
      Value = parse_artist_values(processed_json, 0);
      Value.status = 1;
      Value.arraySize = 1;

      free(req.body);
      cJSON_Delete(input_json);
      return Value;
    }
    else
    {
      Value.status = parse_status(input_json, req, artistid, NULL);
      free(req.body);
      cJSON_Delete(input_json);
      return Value;
    }
  }
  else
  {
    Value.status = -1;
    free(req.body);
    fprintf(stderr, "[Request Error] Artist %zu: CURLE_OK Check failed.\n", artistid);
    return Value;
  }
}

artist_link_model get_artist_link(size_t artistid, size_t limit, size_t offset)
{
  artist_link_model Value;
  int i = 0;
  char *endpoint;
  char baseparams[50];
  
  endpoint = url_cat("artists/", artistid, "/links", 0);
  snprintf(baseparams, 50, "countryCode=%s&limit=%zu&offset=%zu", countryCode,
            limit, offset);
  curl_model req = curl_get(endpoint, baseparams);
  free(endpoint);
  
  if (req.status != -1)
  {
    cJSON *input_json = json_parse(req.body);
    if (req.responseCode == 200)
    {
      Value.status = 1;
      cJSON *limit = cJSON_GetObjectItem(input_json, "limit");
      cJSON *offset = cJSON_GetObjectItem(input_json, "offset");
      cJSON *totalNumberOfItems = cJSON_GetObjectItem(input_json, "totalNumberOfItems");
      cJSON *items = cJSON_GetObjectItem(input_json, "items");
      cJSON *item = NULL;
      
      if (cJSON_IsArray(items))
      {
        Value.arraySize = cJSON_GetArraySize(items);
        cJSON_ArrayForEach(item, items)
        {
          json_links_model processed_json = json_parse_links(item);
          Value = parse_link_values(processed_json, i);
  	  i += 1;
        }
      }
      Value.status = 1;
      parse_number(limit, &Value.limit);
      parse_number(offset, &Value.offset);
      parse_number(totalNumberOfItems, &Value.totalNumberOfItems);
      Value.arraySize = cJSON_GetArraySize(items);

      free(req.body);
      cJSON_Delete(input_json);
      return Value;
    }
    else
    {
      Value.status = parse_status(input_json, req, artistid, NULL);
      free(req.body);
      cJSON_Delete(input_json);
      return Value;
    }
  }
  else
  {
    free(req.body);
    Value.status = -1;
    fprintf(stderr, "[Request Error] Artist %zu: CURLE_OK Check failed.\n", artistid);
    return Value;
  }
}

mix_model get_artist_mix(size_t artistid)
{
  mix_model Value;
  char *endpoint;
  char baseparams[20];

  endpoint = url_cat("artists/", artistid, "/mix", 0);
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

      cJSON_Delete(input_json);
      free(req.body);
      return Value;
    }
    else
    {
      Value.status = parse_status(input_json, req, artistid, NULL);
      free(req.body);
      cJSON_Delete(input_json);
      return Value;
    }
  }
  else
  {
    free(req.body);
    Value.status = -1;
    fprintf(stderr, "[Request Error] Artist %zu: CURLE_OK Check failed.\n", artistid);
    return Value;
  }
}

items_model get_artist_toptracks(size_t artistid, size_t limit, size_t offset)
{
  items_model Value;
  char *endpoint;
  char baseparams[50];
  
  endpoint = url_cat("artists/", artistid, "/toptracks", 0);
  snprintf(baseparams, 50, "countryCode=%s&limit=%zu&offset=%zu", countryCode,
            limit, offset);
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
          json_items_model processed_json = json_parse_items(item);
	  Value = parse_items_values(processed_json, i);
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
      Value.status = parse_status(input_json, req, artistid, NULL);
      free(req.body);
      cJSON_Delete(input_json);
      return Value;
    }
  }
  else
  {
    free(req.body);
    Value.status = -1;
    fprintf(stderr, "[Request Error] Artist %zu: CURLE_OK Check failed.\n", artistid);
    return Value;
  }
}

items_model get_artist_videos(size_t artistid, size_t limit, size_t offset)
{
  items_model Value;
  char *endpoint;
  char baseparams[50];
  
  endpoint = url_cat("artists/", artistid, "/videos", 0);
  snprintf(baseparams, 50, "countryCode=%s&limit=%zu&offset=%zu", countryCode,
            limit, offset);
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
          json_items_model processed_json = json_parse_items(item);
	  Value = parse_items_values(processed_json, i);
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
      Value.status = parse_status(input_json, req, artistid, NULL);
      free(req.body);
      cJSON_Delete(input_json);
      return Value;
    }
  }
  else
  {
    free(req.body);
    Value.status = -1;
    fprintf(stderr, "[Request Error] Artist %zu: CURLE_OK Check failed.\n", artistid);
    return Value;
  }
}

album_model get_artist_albums(size_t artistid, size_t limit, size_t offset)
{
  album_model Value;
  char *endpoint;
  char baseparams[50];
  
  endpoint = url_cat("artists/", artistid, "/albums", 0);
  snprintf(baseparams, 50, "countryCode=%s&limit=%zu&offset=%zu", countryCode,
            limit, offset);
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
          json_album_model processed_json = json_parse_album(item);
	  Value = parse_album_values(processed_json, i);
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
      Value.status = parse_status(input_json, req, artistid, NULL);
      free(req.body);
      cJSON_Delete(input_json);
      return Value;
    }
  }
  else
  {
    free(req.body);
    Value.status = -1;
    fprintf(stderr, "[Request Error] Artist %zu: CURLE_OK Check failed.\n", artistid);
    return Value;
  }
}
