#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/openTIDAL.h"

album_model parse_album(cJSON *input_json)
{
  album_model Value;
  const cJSON *artistItem = NULL;
  cJSON *totalNumberOfItems = cJSON_GetObjectItem(input_json, "totalNumberOfItems");
  cJSON *limit = cJSON_GetObjectItem(input_json, "limit");
  cJSON *offset = cJSON_GetObjectItem(input_json, "offset");
  cJSON *items = cJSON_GetObjectItem(input_json, "items");
  if (cJSON_IsArray(items))
  {
    cJSON *item = NULL;
    int i = 0;
    Value.arraySize = cJSON_GetArraySize(items);
    Value.limit = limit->valueint;
    Value.offset = offset->valueint;
    Value.totalNumberOfItems = totalNumberOfItems->valueint;
    cJSON_ArrayForEach(item, items)
    {
      int artistCounter = 0;
      cJSON *innerItem = cJSON_GetObjectItem(item, "item");
      cJSON *item_version = NULL;
      if (cJSON_IsObject(innerItem))
      {
        item_version = innerItem;
      }
      else
      {
        item_version = item;
      }

      cJSON *id = cJSON_GetObjectItem(item_version, "id");
      cJSON *title = cJSON_GetObjectItemCaseSensitive(item_version, "title");
      cJSON *duration = cJSON_GetObjectItem(item_version, "duration");
      cJSON *numberOfTracks = cJSON_GetObjectItemCaseSensitive(item_version, "numberOfTracks");
      cJSON *numberOfVideos = cJSON_GetObjectItemCaseSensitive(item_version, "numberOfVideos");
      cJSON *numberOfVolumes = cJSON_GetObjectItemCaseSensitive(item_version, "numberOfVolumes");
      cJSON *releaseDate = cJSON_GetObjectItemCaseSensitive(item_version, "releaseDate");
      cJSON *copyright = cJSON_GetObjectItemCaseSensitive(item_version, "copyright");
      cJSON *cover = cJSON_GetObjectItemCaseSensitive(item_version, "cover");
      cJSON *audioQuality = cJSON_GetObjectItemCaseSensitive(item_version, "audioQuality");
      cJSON *popularity = cJSON_GetObjectItemCaseSensitive(item_version, "popularity");
      cJSON *artist = cJSON_GetObjectItemCaseSensitive(item_version, "artists");

      Value.status = 1;
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
      
      if (cJSON_IsString(audioQuality) == 1 && cJSON_IsNull(audioQuality) != 1)
      {
        strncpy(Value.quality[i], audioQuality->valuestring, sizeof(Value.quality[i]));
      }
      
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
    cJSON *audioQuality = cJSON_GetObjectItemCaseSensitive(input_json, "audioQuality");
    cJSON *cover = cJSON_GetObjectItemCaseSensitive(input_json, "cover");
    cJSON *popularity = cJSON_GetObjectItemCaseSensitive(input_json, "popularity");
    cJSON *artist = cJSON_GetObjectItemCaseSensitive(input_json, "artists");

    Value.status = 1;
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
    
    if (cJSON_IsString(audioQuality) == 1 && cJSON_IsNull(audioQuality) != 1)
    {
      strncpy(Value.quality[0], audioQuality->valuestring, sizeof(Value.quality[0]));
    }

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
