#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/openTIDAL.h"

playlist_model parse_playlist(cJSON *input_json)
{
  cJSON *item = NULL;
  int i = 0;
  playlist_model Value;

  cJSON *totalNumberOfItems = cJSON_GetObjectItem(input_json, "totalNumberOfItems");
  cJSON *limit = cJSON_GetObjectItem(input_json, "limit");
  cJSON *offset = cJSON_GetObjectItem(input_json, "offset");
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
    if (cJSON_IsArray(items))
    {
      Value.limit = limit->valueint;
      Value.offset = offset->valueint;
      Value.arraySize = cJSON_GetArraySize(items);
      cJSON_ArrayForEach(item, items)
      {
	cJSON *playlist = cJSON_GetObjectItem(item, "playlist");
	cJSON *playlist_version;
	if (cJSON_IsObject(playlist))
        {
          playlist_version = playlist;
        }
	else
        {
          playlist_version = item;
	}
        cJSON *uuid = cJSON_GetObjectItemCaseSensitive(playlist_version, "uuid");
        cJSON *title = cJSON_GetObjectItemCaseSensitive(playlist_version, "title");
        cJSON *lastUpdated = cJSON_GetObjectItemCaseSensitive(playlist_version, "lastUpdated");
        cJSON *created = cJSON_GetObjectItemCaseSensitive(playlist_version, "created");
        cJSON *image = cJSON_GetObjectItemCaseSensitive(playlist_version, "image");
        cJSON *squareImage = cJSON_GetObjectItem(playlist_version, "squareImage");
        cJSON *numberOfTracks = cJSON_GetObjectItem(playlist_version, "numberOfTracks");
        cJSON *numberOfVideos = cJSON_GetObjectItem(playlist_version, "numberOfVideos");
        cJSON *duration = cJSON_GetObjectItem(playlist_version, "duration");
        cJSON *description = cJSON_GetObjectItemCaseSensitive(playlist_version, "description");
        cJSON *popularity = cJSON_GetObjectItem(playlist_version, "popularity");

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
	Value.popularity[i] = popularity->valueint;

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
      cJSON *popularity = cJSON_GetObjectItem(input_json, "popularity");

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
      Value.popularity[0] = popularity->valueint;
      Value.hasDescription[0] = 0;

      if (cJSON_IsNull(description) != 1)
      {
        Value.hasDescription[0] = 1;
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
