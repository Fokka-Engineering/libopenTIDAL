#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/openTIDAL.h"

artist_model parse_artist(cJSON *input_json, int version)
{
  const cJSON *item = NULL;
  const cJSON *innerItem = NULL;
  const cJSON *items = NULL;
  int i = 0;
  artist_model Value;
  if (version == 1) /* Additional Object in "items"-Array */
  {
    items = cJSON_GetObjectItemCaseSensitive(input_json, "items");
    Value.arraySize = cJSON_GetArraySize(items);
    Value.totalNumberOfItems = cJSON_GetObjectItemCaseSensitive(input_json, "totalNumberOfItems")->valueint;
    Value.offset = cJSON_GetObjectItemCaseSensitive(input_json, "offset")->valueint;

    cJSON_ArrayForEach(item, items)
    {
      innerItem = cJSON_GetObjectItemCaseSensitive(item, "item");
      cJSON *id = cJSON_GetObjectItemCaseSensitive(innerItem, "id");
      cJSON *name = cJSON_GetObjectItemCaseSensitive(innerItem, "name");
      cJSON *picture = cJSON_GetObjectItemCaseSensitive(innerItem, "picture");
      cJSON *popularity = cJSON_GetObjectItemCaseSensitive(innerItem, "popularity");
      
      Value.id[i] = id->valueint;
      strncpy(Value.name[i], name->valuestring, sizeof(Value.name[i]));
      strncpy(Value.picture[i], picture->valuestring, sizeof(Value.picture[i]));
      Value.popularity[i] = popularity->valueint;
      i = i + 1;
    }
    return Value;
  }
  else
  {
    cJSON *id = cJSON_GetObjectItemCaseSensitive(input_json, "id");
    cJSON *name = cJSON_GetObjectItemCaseSensitive(input_json, "name");
    cJSON *picture = cJSON_GetObjectItemCaseSensitive(input_json, "picture");
    cJSON *popularity = cJSON_GetObjectItemCaseSensitive(input_json, "popularity");

    Value.id[0] = id->valueint;
    strncpy(Value.name[0], name->valuestring, sizeof(Value.name[0]));
    strncpy(Value.picture[0], picture->valuestring, sizeof(Value.picture[0]));
    Value.popularity[0] = popularity->valueint;
    return Value;
  }
}
