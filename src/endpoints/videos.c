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
#include "../include/base64.h"
#include "../include/parse.h"
#include "../include/handles.h"
#include "../include/openTIDAL.h"

openTIDAL openTIDAL_GetVideo(const size_t videoid)
{
  openTIDAL o;
  char buffer[100];
  char baseparams[20];
 
  openTIDAL_StructInit(&o);
  openTIDAL_StructAlloc(&o, 1);

  snprintf(buffer, 100, "/v1/videos/%zu", videoid);
  snprintf(baseparams, 20, "countryCode=%s", config.countryCode);
  
  curl_model req = curl_get(buffer, baseparams);
  if (req.status != -1)
  {
    cJSON *input_json = json_parse(req.body);
    if (req.responseCode == 200)
    {
      openTIDAL_ItemsModel video;
      json_items_model processed_json = json_parse_items(input_json);
      parse_items_values(&video, &processed_json);

      o.status = 1;
      openTIDAL_StructAddItem(&o, video);
    }
    else
    {
      o.status = parse_status(input_json, req, videoid, NULL);
    }

    free(req.body);
    o.json = input_json;
    return o;
  }
  else
  {
    o.status = -1;
    free(req.body);
    return o;
  }
}

openTIDAL
openTIDAL_GetFavoriteVideos(const size_t limit, const size_t offset, const char *order, const char *orderDirection)
{
  openTIDAL o;
  char *endpoint = url_cat("/v1/users/", config.userId, "/favorites/videos", 0);
  char baseparams[150];
  
  openTIDAL_StructInit(&o);
  openTIDAL_StructAlloc(&o, 1);

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
          openTIDAL_ItemsModel video;
          cJSON *innerItem = cJSON_GetObjectItem(item, "item");

          json_items_model processed_json = json_parse_items(innerItem);
          
	  parse_items_values(&video, &processed_json);
          parse_number(limit, &video.limit);
          parse_number(offset, &video.offset);
          parse_number(totalNumberOfItems, &video.totalNumberOfItems);
	  
	  openTIDAL_StructAddItem(&o, video);
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
    return o;
  }
}

openTIDAL
openTIDAL_GetVideoContributors(const size_t videoid, const size_t limit, const size_t offset)
{
  openTIDAL o;
  char buffer[100];
  char baseparams[100];

  openTIDAL_StructInit(&o);
  openTIDAL_StructAlloc(&o, 5);

  snprintf(buffer, sizeof(buffer), "/v1/videos/%zu/contributors", videoid);
  snprintf(baseparams, sizeof(baseparams), "countryCode=%s&limit=%zu&offset=%zu", config.countryCode, 
            limit, offset);
  curl_model req = curl_get(buffer, baseparams);
  
  if(req.status != -1)
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
          openTIDAL_ContributorModel contrib;
          json_contributor_model processed_json = json_parse_contributors(item);
	  
	        parse_contributor_values(&contrib, &processed_json);
	        parse_number(limit, &contrib.limit); 
          parse_number(offset, &contrib.offset);
          parse_number(totalNumberOfItems, &contrib.totalNumberOfItems);
  
	        openTIDAL_StructAddContributor(&o, contrib);
	      }
      }
      
      o.status = 1;
    }
    else
    {
      o.status = parse_status(input_json, req, videoid, NULL);
    }

    free(req.body);
    o.json = input_json;
    return o;
  }
  else
  {
    o.status = -1;
    free(req.body);
    return o;
  }
}

openTIDAL openTIDAL_GetVideoStream(const size_t videoid)
{
  openTIDAL o;
  char endpoint[100];
  char baseparams[200];

  openTIDAL_StructInit(&o);
  
  snprintf(endpoint, 100, "/v1/videos/%zu/playbackinfopostpaywall", videoid);
  snprintf(baseparams, 200, "countryCode=%s&videoquality=%s&assetpresentation=%s&playbackmode=%s",
    config.countryCode, config.videoQuality, "FULL", "STREAM");
  curl_model req = curl_get(endpoint, baseparams);
  if (req.status != -1)
  {
    cJSON *input_json = json_parse(req.body);
    if (req.responseCode == 200)
    {
      openTIDAL_StreamModel stream;
      json_stream_model processed_json = json_parse_stream(input_json);
      parse_stream_values(&stream, &processed_json);     
      o.status = 0;
      char manifest_decoded[2048];

      if (strcmp(stream.manifestMimeType, "application/vnd.tidal.emu") == 0)
      {
        o.status = 1;
        Base64decode(manifest_decoded, stream.manifest);
        cJSON *manifest_json = json_parse(manifest_decoded);
        json_manifest_model processed_manifest = json_parse_manifest(manifest_json);

	      parse_string(processed_manifest.mimeType, &stream.mimeType);
        parse_string(processed_manifest.url, &stream.url);
	
	      o.jsonManifest = manifest_json;
      }
      else
      {
        o.status = -10;
        openTIDAL_ParseVerbose("Request Error", "Not a valid manifest. MimeType is not application/vnd.tidal.emu", 1);
      }
    }
    else
    {
      o.status = parse_status(input_json, req, videoid, NULL);
    }

    free(req.body);
    o.json = input_json;
    return o;
  }
  else
  {
    o.status = -1;
    free(req.body);
    return o;
  }
}

/* create & delete favourites */

int openTIDAL_AddFavoriteVideo(const size_t videoid)
{
  char *endpoint = url_cat("/v1/users/", config.userId, "/favorites/videos", 1);
  int status;
  char buffer[100];
  snprintf(buffer, sizeof(buffer), "videoIds=%zu&onArtifactNotFound=FAIL", videoid);
  
  curl_model req = curl_post(endpoint, buffer, "");
  /*Cleanup*/
  free(endpoint);
  free(req.body);

  if (req.status != -1)
  {
    status = parse_raw_status(&req.responseCode);
    return status;
  }
  else
  {
    return -1;
  }
}

int openTIDAL_DeleteFavoriteVideo(const size_t videoid)
{
  int status;
  char buffer[200];
  snprintf(buffer, sizeof(buffer), "/v1/users/%zu/favorites/videos/%zu?countryCode=%s", config.userId, videoid, config.countryCode);

  curl_model req = curl_delete(buffer, "", "");
  /*Cleanup*/
  free(req.body);

  if (req.status != -1)
  {
    status = parse_raw_status(&req.responseCode);
    return status;
  }
  else
  {
    return -1;
  }
}
