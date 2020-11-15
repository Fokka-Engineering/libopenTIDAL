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

items_model get_mix_items(char *mixid)
{
  items_model Value;
  char buffer[50];
  char baseparams[20];

  snprintf(buffer, 50, "mixes/%s/items", mixid);
  snprintf(baseparams, 20, "countryCode=%s", countryCode); 
  curl_model req = curl_get(buffer, baseparams);

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
          cJSON *innerItem = cJSON_GetObjectItem(item, "item");
          json_items_model processed_json = json_parse_items(innerItem);
	  Value = parse_items_values(processed_json, i);
	  i += 1;
	}
      }
      
      parse_number(limit, &Value.limit); 
      parse_number(offset, &Value.offset);
      parse_number(totalNumberOfItems, &Value.totalNumberOfItems);
      Value.status = 1;
      Value.arraySize = cJSON_GetArraySize(items);
    }
    else
    {
      Value.status = parse_status(input_json, req, 0, mixid);
    }

    free(req.body);
    cJSON_Delete(input_json);
    return Value;
  }
  else
  {
    Value.status = -1;
    free(req.body);
    fprintf(stderr, "[Request Error] Mix %s: CURLE_OK Check failed.\n", mixid);
    return Value;
  }
}

page_mix_model get_favorite_mixes()
{
  page_mix_model Value;
  char *endpoint = "pages/my_collection_my_mixes";
  
  char baseparams[40];
  snprintf(baseparams, 40, "countryCode=%s&deviceType=BROWSER", countryCode);
  
  curl_model req = curl_get(endpoint, baseparams);

  if (req.status != -1)
  {
    cJSON *input_json = json_parse(req.body);
    if (req.responseCode == 200)
    {
      /*cJSON *rows = cJSON_GetObjectItem(input_json, "rows");
      cJSON *rowsArray = cJSON_GetArrayItem(rows, 0);
      cJSON *modules = cJSON_GetObjectItem(rowsArray, "modules");
      cJSON *modulesArray = cJSON_GetArrayItem(modules, 0);
      cJSON *type = cJSON_GetObjectItemCaseSensitive(modulesArray, "type");
      cJSON *pagedList = cJSON_GetObjectItem(modulesArray, "pagedList");
      cJSON *limit = cJSON_GetObjectItem(pagedList, "limit");
      cJSON *offset = cJSON_GetObjectItem(pagedList, "offset");
      cJSON *totalNumberOfItems = cJSON_GetObjectItem(pagedList, "totalNumberOfItems");
      cJSON *items = cJSON_GetObjectItem(pagedList, "items");
      cJSON *item = NULL;
      */
      Value.status = 1;
    }
    else
    {
      Value.status = parse_status(input_json, req, userId, NULL);
    }

    free(req.body);
    cJSON_Delete(input_json);
    return Value;
  }
  else
  {
    printf("[Request Error] User %zu: CURLE_OK Check failed.\n", userId);
    Value.status = -1;
    free(req.body);
    return Value;
  }
}

