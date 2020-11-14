#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/parse.h"
#include "../include/handles.h"
#include "../include/openTIDAL.h"

user_model get_user()
{
  user_model Value;
  char *endpoint = url_cat("users/", userId, "", 0);
   
  char baseparams[20];
  snprintf(baseparams, 20, "countryCode=%s", countryCode);

  curl_model req = curl_get(endpoint, baseparams);
  free(endpoint);
  if (req.status != -1)
  {
    cJSON *input_json = json_parse(req.body);
    if (req.responseCode == 200)
    {
      json_user_model processed_json = json_parse_user(input_json);
      Value = parse_user_values(processed_json);
      Value.status = 1;
    }
    else
    {
      Value.status = parse_status(input_json, req, userId, NULL);
    }
    
    cJSON_Delete(input_json);
    free(req.body);
    return Value; /*return data structure*/

  }
  else
  {
    Value.status = -1;
    free(req.body);
    fprintf(stderr, "[Request Error] User %zu: CURLE_OK Check failed.\n", userId);
    return Value;
  }
}

user_subscription_model get_user_subscription()
{
  user_subscription_model Value;
  char *endpoint = url_cat("users/", userId, "/subscription", 0);

  char baseparams[20];
  snprintf(baseparams, 20, "countryCode=%s", countryCode);

  curl_model req = curl_get(endpoint, baseparams);
  free(endpoint);
  if (req.status != -1)
  {
    cJSON *input_json = json_parse(req.body);
    if (req.responseCode == 200)
    {
      json_user_subscription_model processed_json = json_parse_user_subscription(input_json);
      Value = parse_user_subscription_values(processed_json);
      Value.status = 1;
    }
    else
    {
      Value.status = parse_status(input_json, req, userId, NULL);
    }
    
    cJSON_Delete(input_json);
    free(req.body);
    return Value; /*return data structure*/

  }
  else
  {
    Value.status = -1;
    free(req.body);
    fprintf(stderr, "[Request Error] User %zu: CURLE_OK Check failed.\n", userId);
    return Value;
  }
}
