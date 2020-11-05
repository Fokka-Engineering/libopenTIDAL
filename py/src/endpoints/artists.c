#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
      artist_model parse = parse_artist(input_json);
      free(req.body);
      cJSON_Delete(input_json);
      return parse;
    }
    else if (req.responseCode == 400)
    {
      Value.status = parse_badrequest(input_json, artistid, NULL);
      free(req.body);
      cJSON_Delete(input_json);
      return Value;
    }
    else if (req.responseCode == 401)
    {
      Value.status = parse_unauthorized(input_json, artistid);
      free(req.body);
      cJSON_Delete(input_json);
      return Value;
    }
    else if (req.responseCode == 404)
    {
      Value.status = parse_notfound(input_json, artistid, NULL);
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
    fprintf(stderr, "[Request Error] Artist %zu: CURLE_OK Check failed.\n", artistid);
    return Value;
  }
}

artist_link_model get_artist_link(size_t artistid, size_t limit, size_t offset)
{
  artist_link_model Value;
  const cJSON *item = NULL;
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
      cJSON *source = cJSON_GetObjectItemCaseSensitive(input_json, "source");
      cJSON *items = cJSON_GetObjectItem(input_json, "items");
      
      Value.limit = limit->valueint;
      Value.offset = offset->valueint; 
      Value.totalNumberOfItems = totalNumberOfItems->valueint;
      strncpy(Value.source, source->valuestring, sizeof(Value.source));
      Value.arraySize = cJSON_GetArraySize(items);

      cJSON_ArrayForEach(item, items)
      {
        cJSON *url = cJSON_GetObjectItemCaseSensitive(item, "url");
        cJSON *siteName = cJSON_GetObjectItemCaseSensitive(item, "siteName");
        strncpy(Value.url[i], url->valuestring, sizeof(Value.url[i]));
        strncpy(Value.siteName[i], siteName->valuestring, sizeof(Value.siteName[i]));
        i = i + 1;
      }
      free(req.body);
      cJSON_Delete(input_json);
      return Value;
    }
    else if (req.responseCode == 400)
    {
      Value.status = parse_badrequest(input_json, artistid, NULL);
      free(req.body);
      cJSON_Delete(input_json);
      return Value;
    }
    else if (req.responseCode == 401)
    {
      Value.status = parse_unauthorized(input_json, artistid);
      free(req.body);
      cJSON_Delete(input_json);
      return Value;
    }
    else if (req.responseCode == 404)
    {
      Value.status = parse_notfound(input_json, artistid, NULL);
      free(req.body);
      cJSON_Delete(input_json);
      return Value;
    }
    else
    {
      free(req.body);
      Value.status = 0;
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
      strncpy(Value.id, cJSON_GetObjectItemCaseSensitive(input_json, "id")->valuestring, sizeof(Value.id));
      Value.status = 1;
      cJSON_Delete(input_json);
      free(req.body);
      return Value;
    }
    else if (req.responseCode == 400)
    {
      Value.status = parse_badrequest(input_json, artistid, NULL);
      cJSON_Delete(input_json);
      free(req.body);
      return Value;
    }
    else if (req.responseCode == 401)
    {
      Value.status = parse_unauthorized(input_json, artistid);
      cJSON_Delete(input_json);
      free(req.body);
      return Value;
    }
    else if (req.responseCode == 404)
    {
      Value.status = parse_notfound(input_json, artistid, NULL);
      cJSON_Delete(input_json);
      free(req.body);
      return Value;
    }
    else
    {
      free(req.body);
      Value.status = 0;
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
      items_model parse = parse_items(input_json);
      free(req.body);
      cJSON_Delete(input_json);
      return parse;
    }
    else if (req.responseCode == 400)
    {
      Value.status = parse_badrequest(input_json, artistid, NULL);
      free(req.body);
      cJSON_Delete(input_json);
      return Value;
    }
    else if (req.responseCode == 401)
    {
      Value.status = parse_unauthorized(input_json, artistid);
      free(req.body);
      cJSON_Delete(input_json);
      return Value;
    }
    else if (req.responseCode == 404)
    {
      Value.status = parse_notfound(input_json, artistid, NULL);
      free(req.body);
      cJSON_Delete(input_json);
      return Value;
    }
    else
    {
      free(req.body);
      Value.status = 0;
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
      items_model parse = parse_items(input_json);
      free(req.body);
      cJSON_Delete(input_json);
      return parse;
    }
    else if (req.responseCode == 400)
    {
      Value.status = parse_badrequest(input_json, artistid, NULL);
      free(req.body);
      cJSON_Delete(input_json);
      return Value;
    }
    else if (req.responseCode == 401)
    {
      Value.status = parse_unauthorized(input_json, artistid);
      free(req.body);
      cJSON_Delete(input_json);
      return Value;
    }
    else if (req.responseCode == 404)
    {
      Value.status = parse_notfound(input_json, artistid, NULL);
      free(req.body);
      cJSON_Delete(input_json);
      return Value;
    }
    else
    {
      free(req.body);
      Value.status = 0;
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
      album_model parse = parse_album(input_json);
      free(req.body);
      cJSON_Delete(input_json);
      return parse;
    }
    else if (req.responseCode == 400)
    {
      Value.status = parse_badrequest(input_json, artistid, NULL);
      free(req.body);
      cJSON_Delete(input_json);
      return Value;
    }
    else if (req.responseCode == 401)
    {
      Value.status = parse_unauthorized(input_json, artistid);
      free(req.body);
      cJSON_Delete(input_json);
      return Value;
    }
    else if (req.responseCode == 404)
    {
      Value.status = parse_notfound(input_json, artistid, NULL);
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
    fprintf(stderr, "[Request Error] Artist %zu: CURLE_OK Check failed.\n", artistid);
    return Value;
  }
}
