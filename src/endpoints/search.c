#include <stdio.h>
#include <stdlib.h>
#include "../include/openTIDAL.h"

/* Search */

search_model get_search(char *term, char *limit)
{
  char *encodedTerm = url_encode(term);
  char *endpoint = "search/";
  char *baseparams = param_cat(limit, encodedTerm, "");
  curl_model req = curl_get(endpoint, baseparams);
  free(endpoint);
  free(baseparams);
  if (req.status != -1)
  {
    cJSON *input_json = json_parse(req.body);
    search_model parse = parse_search(input_json);
    free(req.body);
    free(encodedTerm);
    cJSON_Delete(input_json);
    return parse;
  }
  else
  {
    printf("%s\n", "Request Error: cURL returned a 4xx status code. Authorization Error.");
    search_model Value;
    Value.status = -1;
    free(req.body);
    free(encodedTerm);
    return Value;
  }
}
