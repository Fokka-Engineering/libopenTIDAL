#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/openTIDAL.h"

album_model parse_album(cJSON *input_json)
{
  album_model Value;
  const cJSON *artistItems = NULL;
  const cJSON *artistItem = NULL;
  const cJSON *items = NULL;

  items = cJSON_GetObjectItemCaseSensitive(input_json, "items");
  if (cJSON_IsArray(items))
  {
    const cJSON *item = NULL;
    int i = 0;
    Value.arraySize = cJSON_GetArraySize(items);
    cJSON_ArrayForEach(item, items)
    {
      int artistCounter = 0;

      cJSON *id = cJSON_GetObjectItem(item, "id");
      cJSON *title = cJSON_GetObjectItemCaseSensitive(item, "title");
      cJSON *duration = cJSON_GetObjectItem(item, "duration");
      cJSON *numberOfTracks = cJSON_GetObjectItemCaseSensitive(item, "numberOfTracks");
      cJSON *numberOfVideos = cJSON_GetObjectItemCaseSensitive(item, "numberOfVideos");
      cJSON *numberOfVolumes = cJSON_GetObjectItemCaseSensitive(item, "numberOfVolumes");
      cJSON *releaseDate = cJSON_GetObjectItemCaseSensitive(item, "releaseDate");
      cJSON *copyright = cJSON_GetObjectItemCaseSensitive(item, "copyright");
      cJSON *cover = cJSON_GetObjectItemCaseSensitive(item, "cover");
      cJSON *popularity = cJSON_GetObjectItemCaseSensitive(item, "popularity");
      cJSON *artist = cJSON_GetObjectItemCaseSensitive(item, "artists");

      Value.id[i] = id->valueint;
      strncpy(Value.title[i], title->valuestring, sizeof(Value.title[i]));
      Value.duration[i] = duration->valueint;
      Value.numberOfTracks[i] = numberOfTracks->valueint;
      Value.numberOfVideos[i] = numberOfVideos->valueint;
      Value.numberOfVolumes[i] = numberOfVolumes->valueint;
      strncpy(Value.releaseDate[i], releaseDate->valuestring, sizeof(Value.releaseDate[i]));
      strncpy(Value.copyright[i], copyright->valuestring, sizeof(Value.copyright[i]));
      strncpy(Value.cover[i], cover->valuestring, sizeof(Value.cover[i]));
      Value.popularity[i] = popularity->valueint;
      
      Value.subArraySize[i] = cJSON_GetArraySize(artist);
      cJSON_ArrayForEach(artistItem, artist)
      {
        cJSON *artistId = cJSON_GetObjectItemCaseSensitive(artistItem, "id");
	cJSON *artistName = cJSON_GetObjectItemCaseSensitive(artistItem, "name");

	Value.artistId[i][artistCounter] = artistId->valueint;
        strncpy(Value.artistName[i][artistCounter], artistName->valuestring, sizeof(Value.artistName[i][artistCounter]));
	artistCounter = artistCounter + 1;
      }
      
      i = i + 1;
    }
  }
  else
  {
    int artistCounter = 0;
    
    cJSON *id = cJSON_GetObjectItem(input_json, "id");
    cJSON *title = cJSON_GetObjectItemCaseSensitive(input_json, "title");
    cJSON *duration = cJSON_GetObjectItem(input_json, "duration");
    cJSON *numberOfTracks = cJSON_GetObjectItemCaseSensitive(input_json, "numberOfTracks");
    cJSON *numberOfVideos = cJSON_GetObjectItemCaseSensitive(input_json, "numberOfVideos");
    cJSON *numberOfVolumes = cJSON_GetObjectItemCaseSensitive(input_json, "numberOfVolumes");
    cJSON *releaseDate = cJSON_GetObjectItemCaseSensitive(input_json, "releaseDate");
    cJSON *copyright = cJSON_GetObjectItemCaseSensitive(input_json, "copyright");
    cJSON *cover = cJSON_GetObjectItemCaseSensitive(input_json, "cover");
    cJSON *popularity = cJSON_GetObjectItemCaseSensitive(input_json, "popularity");
    cJSON *artist = cJSON_GetObjectItemCaseSensitive(input_json, "artists");

    Value.id[0] = id->valueint;
    strncpy(Value.title[0], title->valuestring, sizeof(Value.title[0]));
    Value.duration[0] = duration->valueint;
    Value.numberOfTracks[0] = numberOfTracks->valueint;
    Value.numberOfVideos[0] = numberOfVideos->valueint;
    Value.numberOfVolumes[0] = numberOfVolumes->valueint;
    strncpy(Value.releaseDate[0], releaseDate->valuestring, sizeof(Value.releaseDate[0]));
    strncpy(Value.copyright[0], copyright->valuestring, sizeof(Value.copyright[0]));
    strncpy(Value.cover[0], cover->valuestring, sizeof(Value.cover[0]));
    Value.popularity[0] = popularity->valueint;

    Value.subArraySize[0] = cJSON_GetArraySize(artist);
    cJSON_ArrayForEach(artistItem, artist)
    {
      cJSON *artistId = cJSON_GetObjectItemCaseSensitive(artistItem, "id");
      cJSON *artistName = cJSON_GetObjectItemCaseSensitive(artistItem, "name");

      Value.artistId[0][artistCounter] = artistId->valueint;
      strncpy(Value.artistName[0][artistCounter], artistName->valuestring, sizeof(Value.artistName[0][artistCounter]));
      artistCounter = artistCounter + 1;
    }
  }
  return Value;
}
