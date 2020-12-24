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

openTIDAL openTIDAL_GetTrack(const size_t trackid)
{
    openTIDAL o;
    char *endpoint = url_cat("/v1/tracks/", trackid, "", 0);
    char baseparams[20];
    
    openTIDAL_StructInit(&o);
    openTIDAL_StructAlloc(&o, 1);

    snprintf(baseparams, 20, "countryCode=%s", config.countryCode);

    curl_model req = curl_get(endpoint, baseparams);
    free(endpoint);
    if (req.status != -1)
    {
        cJSON *input_json = json_parse(req.body);
        if (req.responseCode == 200)
        {
            openTIDAL_ItemsModel track;
            json_items_model processed_json = json_parse_items(input_json);
            parse_items_values(&track, &processed_json);
            o.status = 1;
            openTIDAL_StructAddItem(&o, track);
        }
        else
        {
            o.status = parse_status(input_json, req, trackid, NULL);
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
openTIDAL_GetFavoriteTracks(const int limit, const int offset, const char *order, const char *orderDirection)
{
    openTIDAL o;
    char *endpoint = url_cat("/v1/users/", config.userId, "/favorites/tracks", 0);
    char baseparams[150];
    
    openTIDAL_StructInit(&o);
    openTIDAL_StructAlloc(&o, 1);

    snprintf(baseparams, 150, "countryCode=%s&limit=%d&offset=%d&order=%s&orderDirection=%s",
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
                    openTIDAL_ItemsModel track;
                    cJSON *innerItem = cJSON_GetObjectItem(item, "item");

                    json_items_model processed_json = json_parse_items(innerItem);
                    
                    parse_items_values(&track, &processed_json);
                    parse_signed_number(limit, &track.limit);
                    parse_signed_number(offset, &track.offset);
                    parse_signed_number(totalNumberOfItems, &track.totalNumberOfItems);

                    openTIDAL_StructAddItem(&o, track);
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

openTIDAL openTIDAL_GetTrackContributors(const size_t trackid, const int limit, const int offset)
{
    openTIDAL o;
    char *endpoint = url_cat("/v1/tracks/", trackid, "/contributors", 0);
    char baseparams[50];
    
    openTIDAL_StructInit(&o);
    openTIDAL_StructAlloc(&o, 5);

    snprintf(baseparams, 50, "countryCode=%s&limit=%d&offset=%d", config.countryCode, limit, offset);
    
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
                    openTIDAL_ContributorModel contrib;
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
            o.status = parse_status(input_json, req, trackid, NULL);
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

openTIDAL openTIDAL_GetTrackMix(const size_t trackid)
{
    openTIDAL o;
    char *endpoint = url_cat("/v1/tracks/", trackid, "/mix", 0);
    char baseparams[20];
    
    openTIDAL_StructInit(&o);
    openTIDAL_StructAlloc(&o, 4);
    
    snprintf(baseparams, 20, "countryCode=%s", config.countryCode);
    
    curl_model req = curl_get(endpoint, baseparams);
    free(endpoint);

    if (req.status != -1)
    {
        cJSON *input_json = json_parse(req.body);
        if (req.responseCode == 200)
        {
            openTIDAL_MixModel mix;
            json_mix_model processed_json = json_parse_mix(input_json);
            parse_mix_values(&mix, &processed_json);
            o.status = 1;
            openTIDAL_StructAddMix(&o, mix);
        }
        else
        {
            o.status = parse_status(input_json, req, trackid, NULL);
        }

        free(req.body);
        o.json = input_json;
        return o;
    }
    else
    {
        free(req.body);
        o.status = -1;
        return o;
    }
}

openTIDAL openTIDAL_GetTrackStream(const size_t trackid)
{
    openTIDAL o;
    char *endpoint = url_cat("/v1/tracks/", trackid, "/playbackinfopostpaywall", 0);
    char buffer[200];
    
    openTIDAL_StructInit(&o);

    snprintf(buffer, 200, "countryCode=%s&audioquality=%s&assetpresentation=%s&playbackmode=%s",
                        config.countryCode, config.audioQuality, "FULL", "STREAM");
    curl_model req = curl_get(endpoint, buffer);
    free(endpoint);
    
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

            if (strcmp(stream.manifestMimeType, "application/vnd.tidal.bts") == 0)
            {
                o.status = 1;
                Base64decode(manifest_decoded, stream.manifest);
                cJSON *manifest_json = json_parse(manifest_decoded);
                json_manifest_model processed_manifest = json_parse_manifest(manifest_json);
                
                parse_string(processed_manifest.mimeType, &stream.mimeType);
                parse_string(processed_manifest.codec, &stream.codec);
                parse_string(processed_manifest.encryptionType, &stream.encryptionType);
                parse_string(processed_manifest.url, &stream.url);

                o.jsonManifest = manifest_json; 
            }
            else
            {
                o.status = -10;
                openTIDAL_ParseVerbose("Request Error", "Not a valid manifest. MimeType is not application/vnd.tidal.bts", 1);
            }

            o.stream = stream;
        }
        else
        {
            o.status = parse_status(input_json, req, trackid, NULL);
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

int openTIDAL_AddFavoriteTrack(const size_t trackid)
{
    char *endpoint = url_cat("/v1/users/", config.userId, "/favorites/tracks", 1);
    int status;
    char buffer[100];
    snprintf(buffer, 100, "trackIds=%zu&onArtifactNotFound=FAIL", trackid);

    curl_model req = curl_post(endpoint, buffer, "");
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

int openTIDAL_DeleteFavoriteTrack(const size_t trackid)
{
    int status;
    char buffer[80];
    snprintf(buffer, 80, "/v1/users/%zu/favorites/tracks/%zu?countryCode=%s", config.userId, trackid, config.countryCode);

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

