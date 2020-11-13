#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/parse.h"
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

      cJSON_Delete(input_json);
      free(req.body);
      return Value;
    }
    else
    {
      Value.status = parse_status(input_json, req, 0, mixid);
      cJSON_Delete(input_json);
      free(req.body);
      return Value;
    }
  }
  else
  {
    Value.status = -1;
    free(req.body);
    fprintf(stderr, "[Request Error] Mix %s: CURLE_OK Check failed.\n", mixid);
    return Value;
  }
}
