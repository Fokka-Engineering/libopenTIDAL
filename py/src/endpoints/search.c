#include <stdio.h>
#include <stdlib.h>
#include "../include/openTIDAL.h"

/* Search */

search_model get_search(char *term, size_t limit)
{
  search_model Value;
  char *encodedTerm = url_encode(term);
  char *endpoint = "search/";
  char baseparams[128];
  
  snprintf(baseparams, 128, "countryCode=%s&limit=%zu&query=%s", countryCode,
            limit, encodedTerm);
  curl_model req = curl_get(endpoint, baseparams);
  free(encodedTerm);

  if (req.status != -1)
  {
    cJSON *input_json = json_parse(req.body);
    if (req.responseCode == 200)
    {
      search_model parse = parse_search(input_json);
      free(req.body);
      cJSON_Delete(input_json);
      return parse;
    }
    else if (req.responseCode == 400)
    {
      Value.status = parse_badrequest(input_json, 0, "Search");
      cJSON_Delete(input_json);
      free(req.body);
      return Value;
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
      Value.status = parse_notfound(input_json, 0, "Search");
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
    fprintf(stderr, "[Request Error] CURLE_OK Check failed.\n");
    return Value;
  }
}
