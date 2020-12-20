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

openTIDAL openTIDAL_GetPlaylist(const char *playlistid)
{
  openTIDAL o;
  char *endpoint;
  char baseparams[20];
 
  openTIDAL_StructInit(&o);
  openTIDAL_StructAlloc(&o, 3);

  endpoint = url_cat_str("playlists/", playlistid, "");
  snprintf(baseparams, 20, "countryCode=%s", config.countryCode);
  curl_model req = curl_get(endpoint, baseparams);
  free(endpoint);
  
  if (req.status != -1)
  {
    cJSON *input_json = json_parse(req.body);
    if (req.responseCode == 200)
    {
      openTIDAL_PlaylistModel Value;
      json_playlist_model processed_json = json_parse_playlist(input_json);
      parse_playlist_values(&Value, &processed_json); 
      o.status = 1;
      openTIDAL_StructAddPlaylist(&o, Value);
    }
    else
    {
      o.status = parse_status(input_json, req, 0, playlistid);
    }
    
    free(req.body);
    o.json = input_json;
    return o;
  }
  else
  {
    free(req.body);
    o.status = -1;
    fprintf(stderr, "[Request Error] Playlist %s: CURLE_OK Check failed.\n", playlistid);
    return o;
  }
}

openTIDAL openTIDAL_GetPlaylistItems(const char *playlistid, const size_t limit, const size_t offset)
{
  openTIDAL o;
  char *endpoint;
  char baseparams[50];
  
  openTIDAL_StructInit(&o);
  openTIDAL_StructAlloc(&o, 1);

  endpoint = url_cat_str("playlists/", playlistid, "/items");
  snprintf(baseparams, 50, "countryCode=%s&limit=%zu&offset=%zu", config.countryCode,
            limit, offset);
  curl_model req = curl_get(endpoint, baseparams);
  free(endpoint);
  
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
            
      if (cJSON_IsArray(items))
      {
	cJSON_ArrayForEach(item, items)
        {
          openTIDAL_ItemsModel Value;
          cJSON *innerItem = cJSON_GetObjectItem(item, "item");
          json_items_model processed_json = json_parse_items(innerItem);
	  
	  parse_items_values(&Value, &processed_json);
	  parse_number(limit, &Value.limit); 
          parse_number(offset, &Value.offset);
          parse_number(totalNumberOfItems, &Value.totalNumberOfItems);

	  openTIDAL_StructAddItem(&o, Value);
	}
      }
 
      o.status = 1;
    }
    else
    {
      o.status = parse_status(input_json, req, 0, playlistid);
    }
    
    free(req.body);
    o.json = input_json; 
    return o;
  }
  else
  {
    free(req.body);
    o.status = -1;
    fprintf(stderr, "[Request Error] Playlist %s: CURLE_OK Check failed.\n", playlistid);
    return o;
  }
}

openTIDAL
openTIDAL_GetFavoritePlaylists(const size_t limit, const size_t offset, const char *order, const char *orderDirection)
{
  openTIDAL o;
  char *endpoint = url_cat("users/", config.userId, "/playlistsAndFavoritePlaylists", 0);
  char baseparams[150];
  
  openTIDAL_StructInit(&o);
  openTIDAL_StructAlloc(&o, 3);

  snprintf(baseparams, 150, "countryCode=%s&limit=%zu&offset=%zu&order=%s&orderDirection=%s",
             config.countryCode, limit, offset, order, orderDirection);

  curl_model req = curl_get(endpoint, baseparams);
  free(endpoint);
  if (req.status != -1)
  {
    cJSON *input_json = json_parse(req.body);
    if (req.responseCode == 200)
    {
      cJSON *items = cJSON_GetObjectItem(input_json, "items");
      cJSON *item = NULL;
      cJSON *limit = cJSON_GetObjectItem(input_json, "limit");
      cJSON *offset = cJSON_GetObjectItem(input_json, "offset");
      cJSON *totalNumberOfItems = cJSON_GetObjectItem(input_json, "totalNumberOfItems");


      if (cJSON_IsArray(items))
      {
        cJSON_ArrayForEach(item, items)
        {
          openTIDAL_PlaylistModel playlist;
          cJSON *innerItem = cJSON_GetObjectItem(item, "playlist");

          json_playlist_model processed_json = json_parse_playlist(innerItem);
          
	  parse_playlist_values(&playlist, &processed_json);
          parse_number(limit, &playlist.limit);
          parse_number(offset, &playlist.offset);
          parse_number(totalNumberOfItems, &playlist.totalNumberOfItems);

	  openTIDAL_StructAddPlaylist(&o, playlist);
        }
      }
      
      o.status = 1;
    }
    else
    {
      o.status = parse_status(input_json, req, config.userId, NULL);
    }
    
    free(req.body);
    o.json = input_json;
    return o;
  }
  else
  {
    o.status = -1;
    free(req.body);
    fprintf(stderr, "[Request Error] User %zu: CURLE_OK Check failed.", config.userId);
    return o;
  }
}


openTIDAL_ETagModel openTIDAL_GetPlaylistETag(const char *playlistid)
{
  openTIDAL_ETagModel Value;
  /* Request playlist endpoint to scrape eTag Header  */
  char *endpoint;
  char baseparams[20];

  endpoint = url_cat_str("playlists/", playlistid, "");
  snprintf(baseparams, 20, "countryCode=%s", config.countryCode);
  curl_model req = curl_head(endpoint, baseparams); /* Returns Header with eTag */
  free(endpoint);
  
  if (req.status != -1)
  {
    if (req.responseCode == 200)
    {
      size_t i = 0; /* Counter for Buffer (Header) Splitter  */
      size_t e; /* Counter for ETag-String Extraction  */
      char *p = strtok (req.header, "\n");
      char eTag[32];
      /* Buffer for splitted HTTP-Header  */ 
      char *array[30];
      while (p != NULL)
      {
        array[i++] = p;
	/* Split if char is \n  */
        p = strtok (NULL, "\n");
      }
      for (e = 0; e < i; ++e)
      {
        /* If String begins with ETag  */
        if (strncmp(array[e], "ETag", 4) == 0)
        {
          size_t charCounter;
          size_t eTagCounter = 0;
	  /* Extract ETag Values */
          for (charCounter = 7; charCounter < strlen(array[e]); ++charCounter)
          {
            if (array[e][charCounter] != '"')
            {
	      strcpy(&eTag[eTagCounter], &array[e][charCounter]);
	      eTagCounter = eTagCounter + 1;
            }
          }
          strcat(&eTag[eTagCounter + 1], "\0");
        }
      }
      /* copy eTag to struct */
      Value.id = eTag;
      Value.status = 1;
    }
    else
    {
      fprintf(stderr, "[Request Error] Could not parse eTag-Header. Not a 200 Response.\n");
      Value.status = 0;
    }
    free(req.header);
    return Value;
  }
  else
  {
    free(req.header);
    fprintf(stderr, "[Request Error] Playlist %s: CURLE_OK Check failed.\n", playlistid);
    Value.status = -1;
    return Value;
  }
}

int openTIDAL_DeletePlaylist(const char *playlistid)
{
  char buffer[80];
  snprintf(buffer, 80, "playlists/%s?countryCode=%s", playlistid, config.countryCode);

  curl_model req = curl_delete(buffer, "", "");
  free(req.body); 
  if (req.status != -1)
  {
    if (req.responseCode == 200 || req.responseCode == 204)
    {
      return 1;
    }
    else if (req.responseCode == 400)
    {
      fprintf(stderr, "[400] Bad Request (Invalid Indices)\n");
      return -9;
    }
    else if (req.responseCode == 401)
    {
      fprintf(stderr, "[401] Unauthorized\n");
      return -8;
    }
    else if (req.responseCode == 404)
    {
      fprintf(stderr, "[404] Resource %s not found\n", playlistid);
      return -2;
    }
    else if (req.responseCode == 412)
    {
      fprintf(stderr, "[412] Resource %s eTag invalid\n", playlistid);
      return -4;
    }
    else
    {
      return 0;
    }
  }
  else
  {
    fprintf(stderr, "[Request Error] Playlist %s: CURLE_OK Check failed.\n", playlistid);
    return -1;
  }  
}

int openTIDAL_DeletePlaylistItem(const char *playlistid, const size_t index)
{ 
  char buffer[100];
  char etag_buffer[50];

  openTIDAL_ETagModel etag = openTIDAL_GetPlaylistETag(playlistid);
  
  snprintf(etag_buffer, 50, "if-none-match: %s", etag.id);
  snprintf(buffer, 80, "playlists/%s/items/%zu?countryCode=%s", playlistid, index, config.countryCode);
  curl_model req = curl_delete(buffer, "", etag_buffer);
  
  /* cleanup */
  free(req.body);

  if (req.status != -1)
  {
    if (req.responseCode == 200)
    {
      return 1;
    }
    else if (req.responseCode == 400)
    {
      fprintf(stderr, "[400] Bad Request (Invalid Indices)\n");
      return -9;
    }
    else if (req.responseCode == 401)
    {
      fprintf(stderr, "[401] Unauthorized\n");
      return -8;
    }
    else if (req.responseCode == 404)
    {
      fprintf(stderr, "[404] Resource %s not found\n", playlistid);
      return -2;
    }
    else if (req.responseCode == 412)
    {
      fprintf(stderr, "[412] Resource %s eTag invalid\n", playlistid);
      return -4;
    }
    else
    {
      return 0;
    }
  }
  else
  {
    fprintf(stderr, "[Request Error] Playlist %s: CURLE_OK Check failed.\n", playlistid);
    return -1;
  }
}

/* TODO: FIX THIS SHIT!  */
int openTIDAL_MovePlaylistItem(const char *playlistid, const size_t index, const size_t toIndex)
{
  char buffer[100];
  char etag_buffer[50];
  char index_buffer[20];

  openTIDAL_ETagModel etag = openTIDAL_GetPlaylistETag(playlistid);
  
  snprintf(etag_buffer, 50, "if-none-match: %s", etag.id);
  snprintf(index_buffer, 20, "toIndex=%zu", toIndex);
  snprintf(buffer, 100, "playlists/%s/items/%zu?countryCode=%s", playlistid, index, config.countryCode);
  
  curl_model req = curl_post(buffer, index_buffer, etag_buffer);
  
  /* cleanup */
  free(req.body);

  if (req.status != -1)
  {
    if (req.responseCode == 200)
    {
      return 1;
    }
    else if (req.responseCode == 400)
    {
      fprintf(stderr, "[400] Bad Request (Invalid Indices)\n");
      return -9;
    }
    else if (req.responseCode == 401)
    {
      fprintf(stderr, "[401] Unauthorized\n");
      return -8;
    }
    else if (req.responseCode == 404)
    {
      fprintf(stderr, "[404] Resource %s not found\n", playlistid);
      return -2;
    }
    else if (req.responseCode == 412)
    {
      fprintf(stderr, "[412] Resource %s eTag invalid\n", playlistid);
      return -4;
    }
    else
    {
      return 0;
    }
  }
  else
  {
    fprintf(stderr, "[Request Error] Playlist %s: CURLE_OK Check failed.\n", playlistid);
    return -1;
  }
}

int openTIDAL_AddPlaylistItem(const char *playlistid, const size_t trackid, const char *onDupes)
{
  char buffer[100];
  char etag_buffer[50];
  char index_buffer[100];

  openTIDAL_ETagModel etag = openTIDAL_GetPlaylistETag(playlistid);
  
  snprintf(etag_buffer, 50, "if-none-match: %s", etag.id);
  snprintf(index_buffer, 100, "trackIds=%zu&onArtifactNotFound=%s&onDupes=%s", trackid, "FAIL", onDupes);
  snprintf(buffer, 100, "playlists/%s/items?countryCode=%s", playlistid, config.countryCode);
   
  curl_model req = curl_post(buffer, index_buffer, etag_buffer);
  /* cleanup */
  free(req.body);

  if (req.status != -1)
  {
    if (req.responseCode == 200)
    {
      return 1;
    }
    else if (req.responseCode == 400)
    {
      fprintf(stderr, "[400] Bad Request (Invalid Indices)\n");
      return -9;
    }
    else if (req.responseCode == 401)
    {
      fprintf(stderr, "[401] Unauthorized\n");
      return -8;
    }
    else if (req.responseCode == 404)
    {
      fprintf(stderr, "[404] Resource %s not found\n", playlistid);
      return -2;
    }
    else if (req.responseCode == 412)
    {
      fprintf(stderr, "[412] Resource %s eTag invalid\n", playlistid);
      return -4;
    }
    else
    {
      return 0;
    }
  }
  else
  {
    fprintf(stderr, "[Request Error] Playlist %s: CURLE_OK Check failed.\n", playlistid);
    return -1;
  }

}

int openTIDAL_AddPlaylistItems(const char *playlistid, const char *trackids, const char *onDupes)
{
  char buffer[100];
  char etag_buffer[50];
  char index_buffer[100];

  openTIDAL_ETagModel etag = openTIDAL_GetPlaylistETag(playlistid);
  
  snprintf(etag_buffer, 50, "if-none-match: %s", etag.id);
  snprintf(index_buffer, 100, "trackIds=%s&onArtifactNotFound=%s&onDupes=%s", trackids, "FAIL", onDupes);
  snprintf(buffer, 100, "playlists/%s/items?countryCode=%s", playlistid, config.countryCode);
  
  curl_model req = curl_post(buffer, index_buffer, etag_buffer);
  
  /* cleanup */
  free(req.body);

  if (req.status != -1)
  {
    if (req.responseCode == 200)
    {
      return 1;
    }
    else if (req.responseCode == 400)
    {
      fprintf(stderr, "[400] Bad Request (Invalid Indices)\n");
      return -9;
    }
    else if (req.responseCode == 401)
    {
      fprintf(stderr, "[401] Unauthorized\n");
      return -8;
    }
    else if (req.responseCode == 404)
    {
      fprintf(stderr, "[404] Resource %s not found\n", playlistid);
      return -2;
    }
    else if (req.responseCode == 412)
    {
      fprintf(stderr, "[412] Resource %s eTag invalid\n", playlistid);
      return -4;
    }
    else
    {
      return 0;
    }
  }
  else
  {
    fprintf(stderr, "[Request Error] Playlist %s: CURLE_OK Check failed.\n", playlistid);
    return -1;
  }

}

/* create & delete favourites */

openTIDAL openTIDAL_CreatePlaylist(const char *title, const char *description)
{
  openTIDAL o;
  char *endpoint = url_cat("users/", config.userId, "/playlists", 1);
  char *data = malloc(strlen(title)+strlen(description)+7+14+1);
  strcpy(data, "title=");
  strcat(data, title);
  strcat(data, "&description=");
  strcat(data, description);

  openTIDAL_StructInit(&o);
  openTIDAL_StructAlloc(&o, 3);

  curl_model req = curl_post(endpoint, data, "");
  /*Cleanup*/
  free(endpoint);
  free(data);

  if (req.status != -1)
  {
    cJSON *input_json = json_parse(req.body);
    if (req.responseCode == 201 || req.responseCode == 200)
    {
      openTIDAL_PlaylistModel playlist;

      json_playlist_model processed_json = json_parse_playlist(input_json);
      parse_playlist_values(&playlist, &processed_json);
      o.status = 1;
      openTIDAL_StructAddPlaylist(&o, playlist);
    }
    else
    {
      o.status = parse_status(input_json, req, config.userId, NULL);
    }

    free(req.body);
    o.json = input_json;
    return o;
  }
  else
  {
    printf("[Request Error] CURLE_OK Check failed.\n");
    o.status = -1;
    free(req.body);
    return o;
  }
}


int openTIDAL_AddFavoritePlaylist(const char *playlistid)
{
  char *endpoint = url_cat("users/", config.userId, "/favorites/playlists", 1);
  int status;
  char *data = malloc(strlen(playlistid)+10+25+1);
  strcpy(data, "uuids=");
  strcat(data, playlistid);
  strcat(data, "&onArtifactNotFound=FAIL");

  curl_model req = curl_post(endpoint, data, "");
  free(endpoint);
  free(data);
  free(req.body);
  if (req.status != -1)
  {
    if (req.responseCode == 200)
    {
      return 1;
    }
    else if (req.responseCode == 400)
    {
      status = -11;
      return status;
    }
    else if (req.responseCode == 401)
    {
      status = -8;
      return status;
    }
    else if (req.responseCode == 404)
    {
      status = -2;
      return status;
    }
    else
    {
      status = 0;
      return status;
    }
  }
  else
  {
    return -1;
  }
}

int openTIDAL_DeleteFavoritePlaylist(const char *playlistid)
{
  int status;
  char buffer[80];
  snprintf(buffer, 80, "users/%zu/favorites/playlists/%s?countryCode=%s", config.userId, playlistid, config.countryCode);

  curl_model req = curl_delete(buffer, "", "");
  /*Cleanup*/
  free(req.body);

  if (req.status != -1)
  {
    if (req.responseCode == 200)
    {
      return 1;
    }
    else if (req.responseCode == 400)
    {
      status = -11;
      return status;
    }
    else if (req.responseCode == 401)
    {
      status = -8;
      return status;
    }
    else if (req.responseCode == 404)
    {
      status = -2;
      return status;
    }
    else
    {
      status = 0;
      return status;
    }
  }
  else
  {
    return -1;
  }
}

