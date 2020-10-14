#include <stdio.h>
#include <stdlib.h>
#include "../include/openTIDAL.h"

album_model get_album(size_t albumid)
{
  album_model Value;
  char *endpoint = url_cat("albums/", albumid, "", 0);
  char *baseparams = param_cat("100", "", "");
  curl_model req = curl_get(endpoint, baseparams);
  free(endpoint);
  free(baseparams);
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

items_model get_album_items(size_t albumid)
{
  items_model Value;
  char *endpoint = url_cat("albums/", albumid, "/items", 0);
  char *baseparams = param_cat("100", "", "");
  curl_model req = curl_get(endpoint, baseparams);
  free(endpoint);
  free(baseparams);
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
    else if (req.responseCode == 401)
    {
      free(req.body);
      cJSON_Delete(input_json);
      Value.status = parse_unauthorized(input_json, albumid);
      return Value;
    }
    else if (req.responseCode == 404)
    {
      free(req.body);
      cJSON_Delete(input_json);
      Value.status = parse_notfound(input_json, albumid, NULL);
      return Value;
    }
    else
    {
      free(req.body);
      cJSON_Delete(input_json);
      Value.status = 0;
      return Value;
    }
  }
  else
  {
    free(req.body);
    Value.status = -1;
    fprintf(stderr, "[Request Error] Album %zu: CURLE_OK Check failed.", albumid);
    return Value;
  }
}
