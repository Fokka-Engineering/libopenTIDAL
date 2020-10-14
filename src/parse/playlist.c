#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/openTIDAL.h"

playlist_model parse_playlist(cJSON *input_json)
{
  const cJSON *item = NULL;
  int i = 0;
  playlist_model Value;

  cJSON *totalNumberOfItems = cJSON_GetObjectItemCaseSensitive(input_json, "totalNumberOfItems");
  cJSON *items = cJSON_GetObjectItemCaseSensitive(input_json, "items");
  
  if (cJSON_IsObject(totalNumberOfItems))
  {
    Value.totalNumberOfItems = totalNumberOfItems->valueint;
  }
  else
  {
    Value.totalNumberOfItems = 1;
  }

  if (Value.totalNumberOfItems != 0)
  {
    if (cJSON_IsObject(items))
    {
      Value.arraySize = cJSON_GetArraySize(items);

      cJSON_ArrayForEach(item, items)
      {
	cJSON *uuid = cJSON_GetObjectItemCaseSensitive(item, "uuid");
        cJSON *title = cJSON_GetObjectItemCaseSensitive(item, "title");
        cJSON *lastUpdated = cJSON_GetObjectItemCaseSensitive(item, "lastUpdated");
        cJSON *created = cJSON_GetObjectItemCaseSensitive(item, "created");
        cJSON *image = cJSON_GetObjectItemCaseSensitive(item, "image");
        cJSON *squareImage = cJSON_GetObjectItem(item, "squareImage");
        cJSON *numberOfTracks = cJSON_GetObjectItem(item, "numberOfTracks");
        cJSON *numberOfVideos = cJSON_GetObjectItem(item, "numberOfVideos");
        cJSON *duration = cJSON_GetObjectItem(item, "duration");
        cJSON *description = cJSON_GetObjectItemCaseSensitive(item, "description");

        Value.status = 1;
	strncpy(Value.uuid[i], uuid->valuestring, sizeof(Value.uuid[i]));
        strncpy(Value.title[i], title->valuestring, sizeof(Value.title[i]));
        strncpy(Value.lastUpdated[i], lastUpdated->valuestring, sizeof(Value.lastUpdated[i]));
        strncpy(Value.created[i], created->valuestring, sizeof(Value.created[i]));
        strncpy(Value.image[i], image->valuestring, sizeof(Value.image[i]));
        strncpy(Value.squareImage[i], squareImage->valuestring, sizeof(Value.squareImage[i]));
        Value.numberOfTracks[i] = numberOfTracks->valueint;
        Value.numberOfVideos[i] = numberOfVideos->valueint;
        Value.duration[i] = duration->valueint;

        if (cJSON_IsNull(description) != 1)
        {
          strncpy(Value.description[i], description->valuestring, sizeof(Value.description[i]));
        }
        i = i + 1;
      }
    }
    else
    {
      cJSON *uuid = cJSON_GetObjectItemCaseSensitive(input_json, "uuid");
      cJSON *title = cJSON_GetObjectItemCaseSensitive(input_json, "title");
      cJSON *lastUpdated = cJSON_GetObjectItemCaseSensitive(input_json, "lastUpdated");
      cJSON *created = cJSON_GetObjectItemCaseSensitive(input_json, "created");
      cJSON *image = cJSON_GetObjectItemCaseSensitive(input_json, "image");
      cJSON *squareImage = cJSON_GetObjectItemCaseSensitive(input_json, "squareImage");
      cJSON *numberOfTracks = cJSON_GetObjectItem(input_json, "numberOfTracks");
      cJSON *numberOfVideos = cJSON_GetObjectItem(input_json, "numberOfVideos");
      cJSON *duration = cJSON_GetObjectItem(input_json, "duration");
      cJSON *description = cJSON_GetObjectItemCaseSensitive(input_json, "description");

      Value.status = 1;
      strncpy(Value.uuid[0], uuid->valuestring, sizeof(Value.uuid[0]));
      strncpy(Value.title[0], title->valuestring, sizeof(Value.title[0]));
      strncpy(Value.lastUpdated[0], lastUpdated->valuestring, sizeof(Value.lastUpdated[0]));
      strncpy(Value.created[0], created->valuestring, sizeof(Value.created[0]));
      strncpy(Value.image[0], image->valuestring, sizeof(Value.image[0]));
      strncpy(Value.squareImage[0], squareImage->valuestring, sizeof(Value.squareImage[0]));
      Value.numberOfTracks[0] = numberOfTracks->valueint;
      Value.numberOfVideos[0] = numberOfVideos->valueint;
      Value.duration[0] = duration->valueint;

      if (cJSON_IsNull(description) != 1)
      {
        strncpy(Value.description[0], description->valuestring, sizeof(Value.description[0]));
      }
    }
  }
  else
  {
    printf("%s\n", "No Items in Response");
  }
  return Value;
}
