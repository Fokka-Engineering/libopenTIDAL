/*
  Copyright (c) 2020 Hugo Melder and openTIDAL contributors
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:
  
  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.
  
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/parse.h"
#include "../include/handles.h"
#include "../include/openTIDAL.h"

openTIDAL openTIDAL_GetUser()
{
  openTIDAL o;
  char *endpoint = url_cat("/v1/users/", config.userId, "", 0);
  char baseparams[20];
  
  openTIDAL_StructInit(&o);

  snprintf(baseparams, 20, "countryCode=%s", config.countryCode);

  curl_model req = curl_get(endpoint, baseparams);
  free(endpoint);
  if (req.status != -1)
  {
    cJSON *input_json = json_parse(req.body);
    if (req.responseCode == 200)
    {
      openTIDAL_UserModel user;
      json_user_model processed_json = json_parse_user(input_json);
      parse_user_values(&user, &processed_json);
      o.status = 1;
      o.user = user;
    }
    else
    {
      o.status = parse_status(input_json, req, config.userId, NULL);
    }
    
    o.json = input_json;
    free(req.body);
    return o; /*return data structure*/

  }
  else
  {
    o.status = -1;
    free(req.body);
    fprintf(stderr, "[Request Error] User %zu: CURLE_OK Check failed.\n", config.userId);
    return o;
  }
}

openTIDAL openTIDAL_GetUserSubscription()
{
  openTIDAL o;
  char *endpoint = url_cat("/v1/users/", config.userId, "/subscription", 0);
  char baseparams[20];
  
  openTIDAL_StructInit(&o);

  snprintf(baseparams, 20, "countryCode=%s", config.countryCode);

  curl_model req = curl_get(endpoint, baseparams);
  free(endpoint);
  if (req.status != -1)
  {
    cJSON *input_json = json_parse(req.body);
    if (req.responseCode == 200)
    {
      openTIDAL_UserSubscriptionModel Value;
      json_user_subscription_model processed_json = json_parse_user_subscription(input_json);
      parse_user_subscription_values(&Value, &processed_json);
      o.status = 1;
      o.subscription = Value;
    }
    else
    {
      o.status = parse_status(input_json, req, config.userId, NULL);
    }
    
    o.json = input_json;
    free(req.body);
    return o; /*return data structure*/

  }
  else
  {
    o.status = -1;
    free(req.body);
    fprintf(stderr, "[Request Error] User %zu: CURLE_OK Check failed.\n", config.userId);
    return o;
  }
}
