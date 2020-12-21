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
#include "../include/openTIDAL.h"

static void resize_int_array(int *array, int capacity)
{
  int *a = NULL;
  a = realloc(array, sizeof(int) * capacity);
  if (a)
  {
    array = a;
  }
}

static void resize_char_array(char **array, int capacity)
{
  char **c = NULL;
  c = realloc(array, sizeof(char *) * capacity);
  if (c)
  {
    array = c;
  }
}

void parse_module_recentlyPlayed(openTIDAL *o, cJSON *directObject)
{
  cJSON *type = NULL;
  cJSON *item = NULL;
  char *typeString = NULL;
  int total = 0;

  type = cJSON_GetObjectItemCaseSensitive(directObject, "type");
  item = cJSON_GetObjectItem(directObject, "item");
  total = o->home.recentlyPlayed_Total;

  parse_string(type, &typeString);

  if (strncmp(typeString, "ALBUM", 6) == 0)
  {
    openTIDAL_AlbumModel album;
    json_album_model processed_json = json_parse_album(item);
    parse_album_values(&album, &processed_json);
    openTIDAL_StructAddAlbum(o, album);

    /* reference type (album = 0) and current position of albums array */
    o->home.recentlyPlayed_ArrayTypes[total] = 0;
    o->home.recentlyPlayed_ArrayPosition[total] = o->total[0] - 1;
    o->home.recentlyPlayed_Total += 1;
  }
  else if (strncmp(typeString, "TRACK", 5) == 0 || strncmp(typeString, "VIDEO", 6) == 0)
  {
    openTIDAL_ItemsModel track;
    json_items_model processed_json = json_parse_items(item);
    parse_items_values(&track, &processed_json);
    openTIDAL_StructAddItem(o, track);
    
    o->home.recentlyPlayed_ArrayTypes[total] = 1;
    o->home.recentlyPlayed_ArrayPosition[total] = o->total[1] - 1;
    o->home.recentlyPlayed_Total += 1;
  }
  else if (strncmp(typeString, "ARTIST", 7) == 0)
  {
    openTIDAL_ArtistModel artist;
    json_artist_model processed_json = json_parse_artist(item);
    parse_artist_values(&artist, &processed_json);
    openTIDAL_StructAddArtist(o, artist);
    
    o->home.recentlyPlayed_ArrayTypes[total] = 2;
    o->home.recentlyPlayed_ArrayPosition[total] = o->total[2] - 1;
    o->home.recentlyPlayed_Total += 1;
  }
  else if (strncmp(typeString, "PLAYLIST", 9) == 0)
  {
    openTIDAL_PlaylistModel playlist;
    json_playlist_model processed_json = json_parse_playlist(item);
    parse_playlist_values(&playlist, &processed_json);
    openTIDAL_StructAddPlaylist(o, playlist);
    
    o->home.recentlyPlayed_ArrayTypes[total] = 3;
    o->home.recentlyPlayed_ArrayPosition[total] = o->total[3] - 1;
    o->home.recentlyPlayed_Total += 1;
  }
  else if (strncmp(typeString, "MIX", 4) == 0)
  {
    openTIDAL_MixModel mix;
    json_mix_model processed_json = json_parse_mix(item);
    parse_mix_values(&mix, &processed_json);
    openTIDAL_StructAddMix(o, mix);
    
    o->home.recentlyPlayed_ArrayTypes[total] = 4;
    o->home.recentlyPlayed_ArrayPosition[total] = o->total[4] - 1;
    o->home.recentlyPlayed_Total += 1;
  }
}

void parse_home(openTIDAL *o, cJSON *input_json)
{
  cJSON *rows = NULL;
  cJSON *row = NULL;
  
  o->home.recentlyPlayed_Total = 0;
  o->home.mixesForYou_Total = 0;
  o->home.yourHistory_Total = 0;
  o->home.featuredPlaylists_Total = 0;
  o->home.becauseYouListenedTo_ArraySize = 0;
  o->home.radioStationsForYou_ArraySize = 0;
  
  int RecentlyPlayed_Occurrence = 0;
  int MixesForYou_Occurrence = 0;
  int YourHistory_Occurrence = 0;
  int FeaturedPlaylists_Occurrence = 0;
  int radioStationsForYou_Occurrence = 0;
  int becauseYouListenedTo_Occurrence = 0;

  int becauseYouListenedTo_Capacity = 0;
  int radioStationsForYou_Capacity = 0;
  const int init_capacity = 3;

  rows = cJSON_GetObjectItem(input_json, "rows");

  cJSON_ArrayForEach(row, rows)
  {
    cJSON *modules = NULL;
    cJSON *module = NULL;
    cJSON *moduleTitle = NULL;
    cJSON *modulePreTitle = NULL;
    cJSON *moduleType = NULL;
    cJSON *pagedList = NULL;
    cJSON *items = NULL;
    cJSON *item = NULL;
    char *moduleTitleString = NULL;
    char *modulePreTitleString = NULL;
    char *moduleTypeString = NULL;
    int arraySize = 0;
    int mallocError = 0;
    
    modules = cJSON_GetObjectItem(row, "modules");
    module = cJSON_GetArrayItem(modules, 0);
    moduleTitle = cJSON_GetObjectItemCaseSensitive(module, "title");
    modulePreTitle = cJSON_GetObjectItemCaseSensitive(module, "preTitle");
    moduleType = cJSON_GetObjectItemCaseSensitive(module, "type");
    pagedList = cJSON_GetObjectItem(module, "pagedList");
    items = cJSON_GetObjectItem(pagedList, "items");

    parse_string(moduleTitle, &moduleTitleString);
    parse_string(modulePreTitle, &modulePreTitleString);
    parse_string(moduleType, &moduleTypeString);

    if (strncmp(moduleTitleString, "Recently Played", 16) == 0)
    {
      /* overwrite protection if another module with the same name 
       * exists */
      if (RecentlyPlayed_Occurrence < 1)
      {
        arraySize = cJSON_GetArraySize(items);
      
        o->home.recentlyPlayed_ArrayTypes = malloc(sizeof(int) * arraySize);
        if (o->home.recentlyPlayed_ArrayTypes == NULL)
        {
          mallocError = 1;
          goto end;     
        }
        o->home.recentlyPlayed_ArrayPosition = malloc(sizeof(int) * arraySize);
        if (o->home.recentlyPlayed_ArrayPosition == NULL)
        {
          mallocError = 1;
          goto end;
        }
      
        cJSON_ArrayForEach(item, items)
        {
          parse_module_recentlyPlayed(o, item);
        }
      }

      RecentlyPlayed_Occurrence += 1;
    }

    else if (strncmp(moduleTitleString, "Mixes For You", 14) == 0)
    {
      /* overwrite protection if another module with the same name 
       * exists */
      if (MixesForYou_Occurrence < 1)
      {
        o->home.mixesForYou_Start = o->total[4];
      
        cJSON_ArrayForEach(item, items)
        {
          openTIDAL_MixModel mix;
	  json_mix_model processed_json = json_parse_mix(item);
	  parse_mix_values(&mix, &processed_json);
  	  openTIDAL_StructAddMix(o, mix);
        
	  o->home.mixesForYou_Total += 1;
        }
      }

      MixesForYou_Occurrence += 1;
    }

    else if (strncmp(moduleTitleString, "Radio Stations for You", 23) == 0)
    {
      int index = radioStationsForYou_Occurrence;
      
      /* allocate dynamic arrays */
      if (radioStationsForYou_Occurrence < 1)
      {
	o->home.radioStationsForYou_Start = malloc(sizeof(int) * init_capacity);
       	if (o->home.radioStationsForYou_Start == NULL)
        {
          mallocError = 1;
	  goto end;
	}

        o->home.radioStationsForYou_Total = malloc(sizeof(int) * init_capacity);
        if (o->home.radioStationsForYou_Total == NULL)
        {
          mallocError = 1;
	  goto end;
	}

	radioStationsForYou_Capacity = init_capacity;
      }
     
      /* prevent writing to an uninitialised array */ 
      if (radioStationsForYou_Capacity > 0 && strncmp(moduleTypeString, "ALBUM_LIST", 11) == 0)
      {
        if (radioStationsForYou_Capacity == radioStationsForYou_Occurrence)
	{  
	  int capacity = radioStationsForYou_Capacity * 2;
	  
	  resize_int_array(o->home.radioStationsForYou_Start, capacity);
	  resize_int_array(o->home.radioStationsForYou_Total, capacity);

	  radioStationsForYou_Capacity = capacity;
	}
 
       	o->home.radioStationsForYou_Start[index] = o->total[4];

        cJSON_ArrayForEach(item, items)
        {
          openTIDAL_MixModel mix;
	  json_mix_model processed_json = json_parse_mix(item);
	  parse_mix_values(&mix, &processed_json);
	  openTIDAL_StructAddMix(o, mix);

	  o->home.radioStationsForYou_Total[index] += 1;
        }
      }

      radioStationsForYou_Occurrence += 1;

    }
    
    else if (strncmp(moduleTitleString, "Your History", 13) == 0)
    {
      /* overwrite protection if another module with the same name
       * exists */
      if (YourHistory_Occurrence < 1)
      {
        o->home.yourHistory_Start = o->total[4];
      
        cJSON_ArrayForEach(item, items)
        {
          openTIDAL_MixModel mix;
	  json_mix_model processed_json = json_parse_mix(item);
	  parse_mix_values(&mix, &processed_json);
  	  openTIDAL_StructAddMix(o, mix);
        
	  o->home.yourHistory_Total += 1;
        }
      }

      YourHistory_Occurrence += 1;
    }
    
    else if (strncmp(moduleTitleString, "Featured Playlists", 19) == 0)
    {
      if (FeaturedPlaylists_Occurrence < 1)
      {
        o->home.featuredPlaylists_Start = o->total[3];
      
        cJSON_ArrayForEach(item, items)
        {
          openTIDAL_PlaylistModel playlist;
	  json_playlist_model processed_json = json_parse_playlist(item);
	  parse_playlist_values(&playlist, &processed_json);
  	  openTIDAL_StructAddPlaylist(o, playlist);
        
	  o->home.featuredPlaylists_Total += 1;
        }
      }

      FeaturedPlaylists_Occurrence += 1;
    }

    else if (strncmp(modulePreTitleString, "Because You Listened to", 24) == 0)
    {
      int index = becauseYouListenedTo_Occurrence;
      
      /* allocate dynamic arrays */
      if (becauseYouListenedTo_Occurrence < 1)
      {
	o->home.becauseYouListenedTo_Start = malloc(sizeof(int) * init_capacity);
       	if (o->home.becauseYouListenedTo_Start == NULL)
        {
          mallocError = 1;
	  goto end;
	}

        o->home.becauseYouListenedTo_Total = malloc(sizeof(int) * init_capacity);
        if (o->home.becauseYouListenedTo_Total == NULL)
        {
          mallocError = 1;
	  goto end;
	}

	becauseYouListenedTo_Capacity = init_capacity;
      }
     
      /* prevent writing to an uninitialised array */ 
      if (becauseYouListenedTo_Capacity > 0 && strncmp(moduleTypeString, "ALBUM_LIST", 11) == 0)
      {
        if (becauseYouListenedTo_Capacity == becauseYouListenedTo_Occurrence)
	{  
	  int capacity = becauseYouListenedTo_Capacity * 2;
	  
	  resize_int_array(o->home.becauseYouListenedTo_Start, capacity);
	  resize_int_array(o->home.becauseYouListenedTo_Total, capacity);
	  resize_char_array(o->home.becauseYouListenedTo_Title, capacity);

	  becauseYouListenedTo_Capacity = capacity;
	}
 
       	o->home.becauseYouListenedTo_Start[index] = o->total[0];
        o->home.becauseYouListenedTo_Title[index] = moduleTitleString; 

        cJSON_ArrayForEach(item, items)
        {
          openTIDAL_AlbumModel album;
	  json_album_model processed_json = json_parse_album(item);
	  parse_album_values(&album, &processed_json);
	  openTIDAL_StructAddAlbum(o, album);

	  o->home.becauseYouListenedTo_Total[index] += 1;
        }
      }

      becauseYouListenedTo_Occurrence += 1;
    }

end:
    if (mallocError)
    {
      fprintf(stderr, "Failed to Allocate RecentlyPlayed Arrays\n");
    }
  }
}
