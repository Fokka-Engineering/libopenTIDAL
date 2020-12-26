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

openTIDAL_ContentContainer
openTIDAL_GetVideo(openTIDAL_SessionContainer *session, const size_t videoid)
{
    openTIDAL_ContentContainer o;
    openTIDAL_CurlContainer curl;
    char buffer[100];
    char baseparams[20];
 
    openTIDAL_StructInit(&o);
    openTIDAL_StructAlloc(&o, 1);

    snprintf(buffer, 100, "/v1/videos/%zu", videoid);
    snprintf(baseparams, 20, "countryCode=%s", session->countryCode);
    
    openTIDAL_CurlRequest(session, &curl, "GET", buffer, baseparams, NULL, 0, 0);
    if ( curl.status != -1 ) {
        cJSON *input_json = NULL;
        input_json = json_parse(curl.body);
        
        if ( curl.responseCode == 200 ) {
            openTIDAL_ItemsContainer video;
            
            json_items_model processed_json = json_parse_items(input_json);
            parse_items_values(&video, &processed_json);

            o.status = 1;
            openTIDAL_StructAddItem(&o, video);
        }
        else {
            o.status = parse_status(input_json, &curl, videoid, NULL);
        }

        o.json = input_json;
    }
    else
    {
        o.status = -1;
    }

    openTIDAL_CurlRequestCleanup(&curl);
    return o;
}

openTIDAL_ContentContainer
openTIDAL_GetFavoriteVideos(openTIDAL_SessionContainer *session, const int limit,
        const int offset, const char *order, const char *orderDirection)
{
    openTIDAL_ContentContainer o;
    openTIDAL_CurlContainer curl;
    char *endpoint = NULL;
    char baseparams[150];
    
    openTIDAL_StructInit(&o);
    openTIDAL_StructAlloc(&o, 1);

    endpoint = url_cat(session, "/v1/users/", session->userId, "/favorites/videos", 0);
    snprintf(baseparams, 150, "countryCode=%s&limit=%d&offset=%d&order=%s&orderDirection=%s",
                         session->countryCode, limit, offset, order, orderDirection);
    
    openTIDAL_CurlRequest(session, &curl, "GET", endpoint, baseparams, NULL, 0, 0);
    free(endpoint);
    if ( curl.status != -1 ) {
        cJSON *input_json = NULL;
        input_json = json_parse(curl.body);
        
        if ( curl.responseCode == 200 ) {
            cJSON *items = NULL;
            cJSON *item = NULL;
            cJSON *limit = NULL;
            cJSON *offset = NULL;
            cJSON *totalNumberOfItems = NULL;
            
            items = cJSON_GetObjectItem(input_json, "items");
            limit = cJSON_GetObjectItem(input_json, "limit");
            offset = cJSON_GetObjectItem(input_json, "offset");
            totalNumberOfItems = cJSON_GetObjectItem(input_json, "totalNumberOfItems");

            if ( cJSON_IsArray(items) ) {
                cJSON_ArrayForEach( item, items ) {
                    openTIDAL_ItemsContainer video;
                    cJSON *innerItem = cJSON_GetObjectItem(item, "item");

                    json_items_model processed_json = json_parse_items(innerItem);
                    
                    parse_items_values(&video, &processed_json);
                    parse_signed_number(limit, &video.limit);
                    parse_signed_number(offset, &video.offset);
                    parse_signed_number(totalNumberOfItems, &video.totalNumberOfItems);
        
                    openTIDAL_StructAddItem(&o, video);
                }
            }
         
            o.status = 1;
        }
        else
        {
            o.status = parse_status(input_json, &curl, session->userId, NULL);
        }

        o.json = input_json;
    }
    else
    {
        o.status = -1;
    }

    openTIDAL_CurlRequestCleanup(&curl);
    return o;
}

/*openTIDAL_ContentContainer
openTIDAL_GetVideoContributors(const size_t videoid, const int limit, const int offset)
{
    openTIDAL_ContentContainer o;
    char buffer[100];
    char baseparams[100];

    openTIDAL_StructInit(&o);
    openTIDAL_StructAlloc(&o, 5);

    snprintf(buffer, sizeof(buffer), "/v1/videos/%zu/contributors", videoid);
    snprintf(baseparams, sizeof(baseparams), "countryCode=%s&limit=%d&offset=%d", session->countryCode, 
                        limit, offset);
    curl_model curl = curl_get(buffer, baseparams);
    
    if(curl.status != -1)
    {
        cJSON *input_json = json_parse(curl.body);
        if (curl.responseCode == 200)
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
                    openTIDAL_ContributorContainer contrib;
                    json_contributor_model processed_json = json_parse_contributors(item);
        
                    parse_contributor_values(&contrib, &processed_json);
                    parse_signed_number(limit, &contrib.limit); 
                    parse_signed_number(offset, &contrib.offset);
                    parse_signed_number(totalNumberOfItems, &contrib.totalNumberOfItems);
 
                    openTIDAL_StructAddContributor(&o, contrib);
                }
            }
            
            o.status = 1;
        }
        else
        {
            o.status = parse_status(input_json, curl, videoid, NULL);
        }

        free(curl.body);
        o.json = input_json;
        return o;
    }
    else
    {
        o.status = -1;
        free(curl.body);
        return o;
    }
}*/

openTIDAL_ContentContainer
openTIDAL_GetVideoStream(openTIDAL_SessionContainer *session, const size_t videoid)
{
    openTIDAL_ContentContainer o;
    openTIDAL_CurlContainer curl;
    char endpoint[100];
    char baseparams[200];

    openTIDAL_StructInit(&o);
    
    snprintf(endpoint, 100, "/v1/videos/%zu/playbackinfopostpaywall", videoid);
    snprintf(baseparams, 200, "countryCode=%s&videoquality=%s&assetpresentation=%s&playbackmode=%s",
        session->countryCode, session->videoQuality, "FULL", "STREAM");
    
    openTIDAL_CurlRequest(session, &curl, "GET", endpoint, baseparams, NULL, 0, 0);
    if (curl.status != -1)
    {
        cJSON *input_json = json_parse(curl.body);
        if (curl.responseCode == 200)
        {
            openTIDAL_StreamContainer stream;
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

                stream.codec = NULL;
                stream.encryptionType = NULL;
                stream.audioMode = NULL;
                stream.audioQuality = NULL;

                parse_string(processed_manifest.mimeType, &stream.mimeType);
                parse_string(processed_manifest.url, &stream.url);
                
                o.jsonManifest = manifest_json;
            }
            else
            {
                o.status = -10;
                openTIDAL_ParseVerbose("Request Error", "Not a valid manifest. MimeType is not application/vnd.tidal.emu", 1);
            }
            o.stream = stream;
        }
        else
        {
            o.status = parse_status(input_json, &curl, videoid, NULL);
        }

        o.json = input_json;
    }
    else
    {
        o.status = -1;
    }

    openTIDAL_CurlRequestCleanup(&curl);
    return o;
}

/* create & delete favourites */

/*int openTIDAL_AddFavoriteVideo(const size_t videoid)
{
    char *endpoint = url_cat(session, "/v1/users/", session->userId, "/favorites/videos", 1);
    int status;
    char buffer[100];
    snprintf(buffer, sizeof(buffer), "videoIds=%zu&onArtifactNotFound=FAIL", videoid);
    
    curl_model curl = curl_post(endpoint, buffer, "");
    free(endpoint);
    free(curl.body);

    if (curl.status != -1)
    {
        status = parse_raw_status(&curl.responseCode);
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
    snprintf(buffer, sizeof(buffer), "/v1/users/%zu/favorites/videos/%zu?countryCode=%s", session->userId, videoid, session->countryCode);

    curl_model curl = curl_delete(buffer, "", "");
    free(curl.body);

    if (curl.status != -1)
    {
        status = parse_raw_status(&curl.responseCode);
        return status;
    }
    else
    {
        return -1;
    }
}*/
