#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/openTIDAL.h"

/* TODO: Provide direct image url with pages/mix endpoint or find an alternative 
 * (pages endpoints are bloated, slow and hard to parse)
 * Requesting User Mixes (Ids and Images) works with get_user_mixes*/

items_model get_mix_items(char *mixid)
{
  items_model Value;
  char buffer[50];
  
  snprintf(buffer, 50, "mixes/%s/items", mixid);
  char *baseparams = param_cat("100", "", "");
  
  curl_model req = curl_get(buffer, baseparams);
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
      Value.status = parse_unauthorized(input_json, 0);
      cJSON_Delete(input_json);
      free(req.body);
      return Value;
    }
    else if (req.responseCode == 404)
    {
      Value.status = parse_notfound(input_json, 0, mixid);
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
    fprintf(stderr, "[Request Error] Mix %s: CURLE_OK Check failed.", mixid);
    return Value;
  }
}
