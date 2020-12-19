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
#include "../include/parse.h"
#include "../include/handles.h"
#include "../include/openTIDAL.h"

openTIDAL openTIDAL_GetArtist(const size_t artistid)
{
  openTIDAL o; 
  char *endpoint;
  char baseparams[20];
 
  openTIDAL_StructInit(&o);
  openTIDAL_StructAlloc(&o, 2);

  endpoint = url_cat("artists/", artistid, "", 0);
  snprintf(baseparams, 20, "countryCode=%s", countryCode);
  curl_model req = curl_get(endpoint, baseparams);
  free(endpoint);
  
  if (req.status != -1)
  {
    cJSON *input_json = json_parse(req.body);
    if (req.responseCode == 200)
    {
      openTIDAL_ArtistModel artist;
      json_artist_model processed_json = json_parse_artist(input_json);
      parse_artist_values(&artist, &processed_json);
      
      o.status = 1;
      openTIDAL_StructAddArtist(&o, artist);
    }
    else
    {
      o.status = parse_status(input_json, req, artistid, NULL);
    }

    free(req.body);
    o.json = input_json;
    return o; 
  }
  else
  {
    o.status = -1;
    free(req.body);
    fprintf(stderr, "[Request Error] Artist %zu: CURLE_OK Check failed.\n", artistid);
    return o;
  }
}

openTIDAL openTIDAL_GetArtistLink(const size_t artistid, const size_t limit, const size_t offset)
{
  openTIDAL o;
  char *endpoint;
  char baseparams[50];
 
  openTIDAL_StructInit(&o);
  openTIDAL_StructAlloc(&o, 7); 

  endpoint = url_cat("artists/", artistid, "/links", 0);
  snprintf(baseparams, 50, "countryCode=%s&limit=%zu&offset=%zu", countryCode,
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
      cJSON *source = cJSON_GetObjectItemCaseSensitive(input_json, "source");
      cJSON *items = cJSON_GetObjectItem(input_json, "items");
      cJSON *item = NULL;
      
      if (cJSON_IsArray(items))
      {
        cJSON_ArrayForEach(item, items)
        {
          openTIDAL_LinkModel Value;
          json_links_model processed_json = json_parse_links(item);
          parse_link_values(&Value, &processed_json);

          parse_number(limit, &Value.limit);
          parse_number(offset, &Value.offset);
          parse_number(totalNumberOfItems, &Value.totalNumberOfItems);
          parse_string(source, &Value.source);

	  openTIDAL_StructAddLink(&o, Value);
        }
      }
      o.status = 1;
    }
    else
    {
      o.status = parse_status(input_json, req, artistid, NULL);
    }

    free(req.body);
    o.json = input_json;
    return o; 
  }
  else
  {
    free(req.body);
    o.status = -1;
    fprintf(stderr, "[Request Error] Artist %zu: CURLE_OK Check failed.\n", artistid);
    return o;
  }
}

openTIDAL openTIDAL_GetArtistMix(const size_t artistid)
{
  openTIDAL o;
  char *endpoint;
  char baseparams[20];

  openTIDAL_StructInit(&o);
  openTIDAL_StructAlloc(&o, 4);

  endpoint = url_cat("artists/", artistid, "/mix", 0);
  snprintf(baseparams, 20, "countryCode=%s", countryCode);
  curl_model req = curl_get(endpoint, baseparams);
  free(endpoint);
  if (req.status != -1)
  {
    cJSON *input_json = json_parse(req.body);
    if (req.responseCode == 200)
    {
      openTIDAL_MixModel Value;
      
      json_mix_model processed_json = json_parse_mix(input_json);
      parse_mix_values(&Value, &processed_json);
      o.status = 1;
      openTIDAL_StructAddMix(&o, Value);
    }
    else
    {
      o.status = parse_status(input_json, req, artistid, NULL);
    }

    free(req.body);
    o.json = input_json;
    return o; 
  }
  else
  {
    free(req.body);
    o.status = -1;
    fprintf(stderr, "[Request Error] Artist %zu: CURLE_OK Check failed.\n", artistid);
    return o;
  }
}

openTIDAL openTIDAL_GetArtistTopTracks(const size_t artistid, const size_t limit, const size_t offset)
{
  openTIDAL o;
  char *endpoint;
  char baseparams[50];
 
  openTIDAL_StructInit(&o);
  openTIDAL_StructAlloc(&o, 1);

  endpoint = url_cat("artists/", artistid, "/toptracks", 0);
  snprintf(baseparams, 50, "countryCode=%s&limit=%zu&offset=%zu", countryCode,
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
          json_items_model processed_json = json_parse_items(item);
	  
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
      o.status = parse_status(input_json, req, artistid, NULL);
    }

    free(req.body);
    o.json = input_json;
    return o; 
  }
  else
  {
    free(req.body);
    o.status = -1;
    fprintf(stderr, "[Request Error] Artist %zu: CURLE_OK Check failed.\n", artistid);
    return o;
  }
}

openTIDAL openTIDAL_GetArtistVideos(const size_t artistid, const size_t limit, const size_t offset)
{
  openTIDAL o;
  char *endpoint;
  char baseparams[50];
 
  openTIDAL_StructInit(&o);
  openTIDAL_StructAlloc(&o, 1);

  endpoint = url_cat("artists/", artistid, "/videos", 0);
  snprintf(baseparams, 50, "countryCode=%s&limit=%zu&offset=%zu", countryCode,
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
          json_items_model processed_json = json_parse_items(item);
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
      o.status = parse_status(input_json, req, artistid, NULL);
    }

    free(req.body);
    o.json = input_json;
    return o; 
  }
  else
  {
    free(req.body);
    o.status = -1;
    fprintf(stderr, "[Request Error] Artist %zu: CURLE_OK Check failed.\n", artistid);
    return o;
  }
}

openTIDAL openTIDAL_GetArtistAlbums(const size_t artistid, const size_t limit, const size_t offset)
{
  openTIDAL o;
  char *endpoint;
  char baseparams[50];
 
  openTIDAL_StructInit(&o);
  openTIDAL_StructAlloc(&o, 0);

  endpoint = url_cat("artists/", artistid, "/albums", 0);
  snprintf(baseparams, 50, "countryCode=%s&limit=%zu&offset=%zu", countryCode,
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
          openTIDAL_AlbumModel Value;
          json_album_model processed_json = json_parse_album(item);
	  
	  parse_album_values(&Value, &processed_json);
          parse_number(limit, &Value.limit); 
          parse_number(offset, &Value.offset);
          parse_number(totalNumberOfItems, &Value.totalNumberOfItems);
          
	  openTIDAL_StructAddAlbum(&o, Value);
	}
      }
      
      o.status = 1;
    }
    else
    {
      o.status = parse_status(input_json, req, artistid, NULL);
    }

    free(req.body);
    o.json = input_json;
    return o; 
  }
  else
  {
    free(req.body);
    o.status = -1;
    fprintf(stderr, "[Request Error] Artist %zu: CURLE_OK Check failed.\n", artistid);
    return o;
  }
}

openTIDAL
openTIDAL_GetFavoriteArtists(const size_t limit, const size_t offset, const char *order, const char *orderDirection)
{
  openTIDAL o;
  char *endpoint = url_cat("users/", userId, "/favorites/artists", 0);
  char baseparams[150];
  
  openTIDAL_StructInit(&o);
  openTIDAL_StructAlloc(&o, 2);

  snprintf(baseparams, 150, "countryCode=%s&limit=%zu&offset=%zu&order=%s&orderDirection=%s",
             countryCode, limit, offset, order, orderDirection);

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
          openTIDAL_ArtistModel artist;
          cJSON *innerItem = cJSON_GetObjectItem(item, "item");
          json_artist_model processed_json = json_parse_artist(innerItem);
          
	  parse_artist_values(&artist, &processed_json);
	  parse_number(limit, &artist.limit);
          parse_number(offset, &artist.offset);
          parse_number(totalNumberOfItems, &artist.totalNumberOfItems);

	  openTIDAL_StructAddArtist(&o, artist);
        }
      }
      
      o.status = 1;
    }
    else
    {
      o.status = parse_status(input_json, req, userId, NULL);
    }

    free(req.body);
    o.json = input_json;
    return o; 
  }
  else
  {
    o.status = -1;
    free(req.body);
    fprintf(stderr, "[Request Error] User %zu: CURLE_OK Check failed.", userId);
    return o;
  }
}

/* create & delete favourites */

int openTIDAL_AddFavoriteArtist(const size_t artistid)
{
  char *endpoint = url_cat("users/", userId, "/favorites/artists", 1);
  int status;
  char buffer[60];
  snprintf(buffer, 60, "artistIds=%zu&onArtifactNotFound=FAIL", artistid);

  curl_model req = curl_post(endpoint, buffer, "");
  free(endpoint);
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

int openTIDAL_DeleteFavoriteArtist(const size_t artistid)
{
  int status;
  char buffer[80];
  snprintf(buffer, 80, "users/%zu/favorites/artists/%zu?countryCode=%s", userId, artistid, countryCode);

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

