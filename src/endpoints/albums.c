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

openTIDAL openTIDAL_GetAlbum(const size_t albumid)
{
  openTIDAL o;
  char *endpoint;
  char baseparams[20];
 
  openTIDAL_StructInit(&o);
  openTIDAL_StructAlloc(&o, 0);

  endpoint = url_cat("/v1/albums/", albumid, "", 0);
  snprintf(baseparams, 20, "countryCode=%s", config.countryCode);
  curl_model req = curl_get(endpoint, baseparams);
  free(endpoint);
  if (req.status != -1)
  {
    cJSON *input_json = json_parse(req.body);
    if (req.responseCode == 200)
    {
      openTIDAL_AlbumModel album;
      json_album_model processed_json = json_parse_album(input_json);
      parse_album_values(&album, &processed_json);
      
      o.status = 1;
      openTIDAL_StructAddAlbum(&o, album);
    }
    else
    {
      o.status = parse_status(input_json, req, albumid, NULL);
    }
   
    o.json = input_json; 
    free(req.body);
    return o;
  }
  else
  {
    free(req.body);
    o.status = -1;
    fprintf(stderr, "[Request Error] Album %zu: CURLE_OK Check failed.\n", albumid);
    return o;
  }
}

openTIDAL openTIDAL_GetAlbumItems(const size_t albumid, const size_t limit, const size_t offset)
{
  openTIDAL o;

  char *endpoint;
  char baseparams[50];
  
  openTIDAL_StructInit(&o);
  openTIDAL_StructAlloc(&o, 1);

  endpoint = url_cat("/v1/albums/", albumid, "/items", 0);
  snprintf(baseparams, 50, "countryCode=%s&limit=%zu&offset=%zu", config.countryCode,
            limit, offset);

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
      o.status = parse_status(input_json, req, albumid, NULL);
    }
    
    free(req.body);
    o.json = input_json;
    return o;
  }
  else
  {
    free(req.body);
    o.status = -1;
    fprintf(stderr, "[Request Error] Album %zu: CURLE_OK Check failed.\n", albumid);
    return o;
  }
}

openTIDAL
openTIDAL_GetFavoriteAlbums(const size_t limit, const size_t offset, const char *order, const char *orderDirection)
{
  openTIDAL o;
  char *endpoint = url_cat("/v1/users/", config.userId, "/favorites/albums", 0);
  char baseparams[150];
  
  openTIDAL_StructInit(&o);
  openTIDAL_StructAlloc(&o, 0);

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
          openTIDAL_AlbumModel album;
          cJSON *innerItem = cJSON_GetObjectItem(item, "item");

          json_album_model processed_json = json_parse_album(innerItem);
          parse_album_values(&album, &processed_json);
  	  
	  parse_number(limit, &album.limit);
          parse_number(offset, &album.offset);
          parse_number(totalNumberOfItems, &album.totalNumberOfItems);

	  openTIDAL_StructAddAlbum(&o, album);
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

/* create & delete favourites */

int openTIDAL_AddFavoriteAlbum(const size_t albumid)
{
  char *endpoint = url_cat("/v1/users/", config.userId, "/favorites/albums", 1);
  int status;
  char buffer[60];
  snprintf(buffer, 60, "albumIds=%zu&onArtifactNotFound=FAIL", albumid);

  curl_model req = curl_post(endpoint, buffer, "");
  free(endpoint);
  free(req.body);
  if (req.status != -1)
  {
    if (req.responseCode == 200)
    {
      status = 1;
    }
    else if (req.responseCode == 400)
    {
      status = -11;
    }
    else if (req.responseCode == 401)
    {
      status = -8;
    }
    else if (req.responseCode == 404)
    {
      status = -2;
    }
    else
    {
      status = 0;
    }

    return status;
  }
  else
  {
    return -1;
  }
}

int openTIDAL_DeleteFavoriteAlbum(const size_t albumid)
{
  int status;
  char buffer[80];
  snprintf(buffer, 80, "/v1/users/%zu/favorites/albums/%zu?countryCode=%s", config.userId,
            albumid, config.countryCode);
  
  curl_model req = curl_delete(buffer, "", "");

  if (req.status != -1)
  {
    if (req.responseCode == 200)
    {
      status = 1;
    }
    else if (req.responseCode == 400)
    {
      status = -11;
    }
    else if (req.responseCode == 401)
    {
      status = -8;
    }
    else if (req.responseCode == 404)
    {
      status = -2;
    }
    else
    {
      status = 0;
    }
    
    return status;
  }
  else
  {
    return -1;
  }
}

