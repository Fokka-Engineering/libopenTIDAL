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

#include "../include/base64.h"
#include "../include/handles.h"
#include "../include/helper.h"
#include "../include/openTIDAL.h"
#include "../include/parse.h"
#include "../include/struct.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

openTIDAL_ContentContainer *
openTIDAL_GetVideo (openTIDAL_SessionContainer *session, const size_t videoid)
{
    openTIDAL_ContentContainer *o = NULL;
    openTIDAL_CurlContainer curl;

    openTIDAL_StructMainAlloc (&o);
    openTIDAL_StructInit (o);
    openTIDAL_StructAlloc (o, 1);

    openTIDAL_StringHelper (&curl.endpoint, "/v1/videos/%zu", videoid);
    openTIDAL_StringHelper (&curl.parameter, "countryCode=%s", session->countryCode);
    if (!curl.endpoint || !curl.parameter) {
        o->status = -14;
        return o;
    }

    openTIDAL_CurlRequest (session, &curl, "GET", curl.endpoint, curl.parameter, NULL, 0, 0);
    if (curl.status != -1) {
        o->json = openTIDAL_cJSONParseHelper (curl.body);
        if (!o->json) {
            o->status = -14;
            return o;
        }

        if (curl.responseCode == 200) {
            openTIDAL_ItemsContainer video;

            json_items_model processed_json = json_parse_items ((cJSON *)o->json);
            parse_items_values (&video, &processed_json);

            o->status = 1;
            openTIDAL_StructAddItem (o, video);
        }
        else {
            o->status = parse_status ((cJSON *)o->json, &curl, videoid, NULL);
        }
    }
    openTIDAL_CurlRequestCleanup (&curl);
    return o;
}

openTIDAL_ContentContainer *
openTIDAL_GetFavoriteVideos (openTIDAL_SessionContainer *session, const int limit, const int offset,
                             const char *order, const char *orderDirection)
{
    openTIDAL_ContentContainer *o = NULL;
    openTIDAL_CurlContainer curl;

    openTIDAL_StructMainAlloc (&o);
    openTIDAL_StructInit (o);
    openTIDAL_StructAlloc (o, 1);

    openTIDAL_StringHelper (&curl.endpoint, "/v1/users/%zu/favorites/videos", session->userId);
    openTIDAL_StringHelper (&curl.parameter,
                            "countryCode=%s&limit=%d&offset=%d&order=%s&orderDirection=%s",
                            session->countryCode, limit, offset, order, orderDirection);
    if (!curl.endpoint || !curl.parameter) {
        o->status = -14;
        return o;
    }

    openTIDAL_CurlRequest (session, &curl, "GET", curl.endpoint, curl.parameter, NULL, 0, 0);
    if (curl.status != -1) {
        o->json = openTIDAL_cJSONParseHelper (curl.body);
        if (!o->json) {
            o->status = -14;
            return o;
        }

        if (curl.responseCode == 200) {
            cJSON *items = NULL;
            cJSON *item = NULL;
            cJSON *limit = NULL;
            cJSON *offset = NULL;
            cJSON *totalNumberOfItems = NULL;

            items = cJSON_GetObjectItem ((cJSON *)o->json, "items");
            limit = cJSON_GetObjectItem ((cJSON *)o->json, "limit");
            offset = cJSON_GetObjectItem ((cJSON *)o->json, "offset");
            totalNumberOfItems = cJSON_GetObjectItem ((cJSON *)o->json, "totalNumberOfItems");

            if (cJSON_IsArray (items)) {
                cJSON_ArrayForEach (item, items)
                {
                    openTIDAL_ItemsContainer video;
                    cJSON *innerItem = cJSON_GetObjectItem (item, "item");

                    json_items_model processed_json = json_parse_items (innerItem);

                    parse_items_values (&video, &processed_json);
                    parse_signed_number (limit, &video.limit);
                    parse_signed_number (offset, &video.offset);
                    parse_signed_number (totalNumberOfItems, &video.totalNumberOfItems);

                    openTIDAL_StructAddItem (o, video);
                }
                o->status = 1;
            }
        }
        else {
            o->status = parse_status ((cJSON *)o->json, &curl, session->userId, NULL);
        }
    }
    openTIDAL_CurlRequestCleanup (&curl);
    return o;
}

/*openTIDAL_ContentContainer *
openTIDAL_GetVideoContributors(const size_t videoid, const int limit, const int offset)
{
    openTIDAL_ContentContainer * o;
    char buffer[100];
    char baseparams[100];

    openTIDAL_StructInit(&o);
    openTIDAL_StructAlloc(&o, 5);

    snprintf(buffer, sizeof(buffer), "/v1/videos/%zu/contributors", videoid);
    snprintf(baseparams, sizeof(baseparams), "countryCode=%s&limit=%d&offset=%d",
session->countryCode, limit, offset); curl_model curl = curl_get(buffer, baseparams);

    if(curl.status != -1)
    {
        cJSON *(cJSON *)o->json = json_parse(curl.body);
        if (curl.responseCode == 200)
        {
            cJSON *limit = cJSON_GetObjectItem((cJSON *)o->json, "limit");
            cJSON *offset = cJSON_GetObjectItem((cJSON *)o->json, "offset");
            cJSON *totalNumberOfItems = cJSON_GetObjectItem((cJSON *)o->json, "totalNumberOfItems");
            cJSON *items = cJSON_GetObjectItem((cJSON *)o->json, "items");
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

            o->status = 1;
        }
        else
        {
            o->status = parse_status((cJSON *)o->json, curl, videoid, NULL);
        }

        free(curl.body);
        o->json = (cJSON *)o->json;
        return o;
    }
    else
    {
        o->status = -1;
        free(curl.body);
        return o;
    }
}*/

openTIDAL_ContentContainer *
openTIDAL_GetVideoStream (openTIDAL_SessionContainer *session, const size_t videoid)
{
    openTIDAL_ContentContainer *o = NULL;
    openTIDAL_CurlContainer curl;

    openTIDAL_StructMainAlloc (&o);
    openTIDAL_StructInit (o);

    openTIDAL_StringHelper (&curl.endpoint, "/v1/videos/%zu/playbackinfopostpaywall", videoid);
    openTIDAL_StringHelper (&curl.parameter,
                            "countryCode=%s&videoquality=%s&assetpresentation=%s&playbackmode=%s",
                            session->countryCode, session->videoQuality, "FULL", "STREAM");
    if (!curl.endpoint || !curl.parameter) {
        o->status = -14;
        return o;
    }

    openTIDAL_CurlRequest (session, &curl, "GET", curl.endpoint, curl.parameter, NULL, 0, 0);
    if (curl.status != -1) {
        o->json = openTIDAL_cJSONParseHelper (curl.body);
        if (!o->json) {
            o->status = -14;
            return o;
        }

        if (curl.responseCode == 200) {
            json_stream_model processed_json = json_parse_stream ((cJSON *)o->json);
            parse_stream_values (&o->stream, &processed_json);
            o->status = 0;

            if (strcmp (o->stream.manifestMimeType, "application/vnd.tidal.emu") == 0) {
                json_manifest_model processed_manifest;
                char *manifest_decoded = NULL;

                openTIDAL_VerboseHelper ("Base64", "Allocated decoded data in heap", 2);
                manifest_decoded = malloc (openTIDAL_Base64DecodeLen (o->stream.manifest));
                if (!manifest_decoded) {
                    o->status = -14;
                    return o;
                }

                openTIDAL_Base64Decode (manifest_decoded, o->stream.manifest);
                o->jsonManifest = openTIDAL_cJSONParseHelper (manifest_decoded);

                o->status = 1;
                o->stream.codec = NULL;
                o->stream.encryptionType = NULL;
                o->stream.audioMode = NULL;
                o->stream.audioQuality = NULL;

                processed_manifest = json_parse_manifest ((cJSON *)o->jsonManifest);
                parse_string (processed_manifest.mimeType, &o->stream.mimeType);
                parse_string (processed_manifest.url, &o->stream.url);

                free (manifest_decoded);
            }
            else {
                o->status = -10;
                openTIDAL_VerboseHelper (
                    "Request Error",
                    "Not a valid manifest. MimeType is not application/vnd.tidal.emu", 1);
            }
        }
        else {
            o->status = parse_status ((cJSON *)o->json, &curl, videoid, NULL);
        }
    }
    openTIDAL_CurlRequestCleanup (&curl);
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
    snprintf(buffer, sizeof(buffer), "/v1/users/%zu/favorites/videos/%zu?countryCode=%s",
session->userId, videoid, session->countryCode);

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
