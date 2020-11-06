#include <stdio.h>
#include <stdlib.h>
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
      album_model parse = parse_album(input_json);
      free(req.body);
      cJSON_Delete(input_json);
      return parse;
    }
    else if (req.responseCode == 400)
    {
      Value.status = parse_badrequest(input_json, albumid, NULL);
      free(req.body);
      cJSON_Delete(input_json);
      return Value;
    }
    else if (req.responseCode == 401)
    {
      Value.status = parse_unauthorized(input_json, albumid);
      free(req.body);
      cJSON_Delete(input_json);
      return Value;
    }
    else if (req.responseCode == 404)
    {
      Value.status = parse_notfound(input_json, albumid, NULL);
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
      items_model parse = parse_items(input_json);
      cJSON_Delete(input_json);
      free(req.body);
      return parse;
    }
    else if (req.responseCode == 400)
    {
      Value.status = parse_badrequest(input_json, albumid, NULL);
      cJSON_Delete(input_json);
      free(req.body);
      return Value;
    }
    else if (req.responseCode == 401)
    {
      Value.status = parse_unauthorized(input_json, albumid);
      free(req.body);
      cJSON_Delete(input_json);
      return Value;
    }
    else if (req.responseCode == 404)
    {
      Value.status = parse_notfound(input_json, albumid, NULL);
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
    fprintf(stderr, "[Request Error] Album %zu: CURLE_OK Check failed.\n", albumid);
    return Value;
  }
}
