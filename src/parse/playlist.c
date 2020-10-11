#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/cTidal.h"

playlist_model parse_playlist(cJSON *input_json, int version)
{
  const cJSON *item = NULL;
  const cJSON *items = NULL;
  int i = 0;
  playlist_model Value;
  if (version == 0)
  {
     Value.totalNumberOfItems = 1; /* Quickhack to avoid a Seg Fault (totalNumberOfItems not always present)  */
  }
  else
  {
     cJSON *totalNumberOfItems = cJSON_GetObjectItemCaseSensitive(input_json, "totalNumberOfItems"); 
     Value.totalNumberOfItems = totalNumberOfItems->valueint;
  }
  if (Value.totalNumberOfItems != 0)
  {
    if (version == 1)
    {
      items = cJSON_GetObjectItemCaseSensitive(input_json, "items");
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

        strcpy(Value.uuid[i], uuid->valuestring);
        strcpy(Value.title[i], title->valuestring);
        strcpy(Value.lastUpdated[i], lastUpdated->valuestring);
        strcpy(Value.created[i], created->valuestring);
        strcpy(Value.image[i], image->valuestring);
        strcpy(Value.squareImage[i], squareImage->valuestring);
        Value.numberOfTracks[i] = numberOfTracks->valueint;
        Value.numberOfVideos[i] = numberOfVideos->valueint;
        Value.duration[i] = duration->valueint;

        if (cJSON_IsNull(description) != 1)
        {
          strcpy(Value.description[i], description->valuestring);
        }
        i = i + 1;
      }
    }
    else if (version == 2)
    {
      items = cJSON_GetObjectItemCaseSensitive(input_json, "items");
      Value.arraySize = cJSON_GetArraySize(items);

      cJSON_ArrayForEach(item, items)
      {
        cJSON *playlist = cJSON_GetObjectItemCaseSensitive(item, "playlist");
        cJSON *uuid = cJSON_GetObjectItemCaseSensitive(playlist, "uuid");
        cJSON *title = cJSON_GetObjectItemCaseSensitive(playlist, "title");
        cJSON *lastUpdated = cJSON_GetObjectItemCaseSensitive(playlist, "lastUpdated");
        cJSON *created = cJSON_GetObjectItemCaseSensitive(playlist, "created");
        cJSON *image = cJSON_GetObjectItemCaseSensitive(playlist, "image");
        cJSON *squareImage = cJSON_GetObjectItem(playlist, "squareImage");
        cJSON *numberOfTracks = cJSON_GetObjectItem(playlist, "numberOfTracks");
        cJSON *numberOfVideos = cJSON_GetObjectItem(playlist, "numberOfVideos");
        cJSON *duration = cJSON_GetObjectItem(playlist, "duration");
        cJSON *description = cJSON_GetObjectItemCaseSensitive(playlist, "description");

        strcpy(Value.uuid[i], uuid->valuestring);
        strcpy(Value.title[i], title->valuestring);
        strcpy(Value.lastUpdated[i], lastUpdated->valuestring);
        strcpy(Value.created[i], created->valuestring);
        strcpy(Value.image[i], image->valuestring);
        strcpy(Value.squareImage[i], squareImage->valuestring);
        Value.numberOfTracks[i] = numberOfTracks->valueint;
        Value.numberOfVideos[i] = numberOfVideos->valueint;
        Value.duration[i] = duration->valueint;

        if (cJSON_IsNull(description) != 1)
        {
          strcpy(Value.description[i], description->valuestring);
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

      strcpy(Value.uuid[0], uuid->valuestring);
      strcpy(Value.title[0], title->valuestring);
      strcpy(Value.lastUpdated[0], lastUpdated->valuestring);
      strcpy(Value.created[0], created->valuestring);
      strcpy(Value.image[0], image->valuestring);
      strcpy(Value.squareImage[0], squareImage->valuestring);
      Value.numberOfTracks[0] = numberOfTracks->valueint;
      Value.numberOfVideos[0] = numberOfVideos->valueint;
      Value.duration[0] = duration->valueint;

      if (cJSON_IsNull(description) != 1)
      {
        strcpy(Value.description[0], description->valuestring);
      }
    }
  }
  else
  {
    printf("%s\n", "No Items in Response");
  }
  return Value;
}
