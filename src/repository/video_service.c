/*
    Copyright (c) 2020-2021 Hugo Melder and openTIDAL contributors

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

#include "../external/base64.h"
#include "../helper/helper.h"
#include "../helper/struct_helper.h"
#include "../http_connector.h"
#include "../openTIDAL.h"
#include "../parse/parse.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

openTIDAL_ContentContainer *
openTIDAL_GetVideo (openTIDAL_SessionContainer *session, size_t videoid)
{
    openTIDAL_ContentContainer *o = NULL;
    openTIDAL_CurlContainer curl;
    int status = 0;

    openTIDAL_CurlModelInit (&curl);

    status = openTIDAL_StructMainAlloc (&o);
    if (status == -1) return NULL;
    status = openTIDAL_StructInit (o);
    if (status == -1) goto end;
    status = openTIDAL_StructAlloc (o, 1);
    if (status == -1) goto end;

    openTIDAL_StringHelper (&curl.endpoint, "/v1/videos/%zu", videoid);
    openTIDAL_StringHelper (&curl.parameter, "countryCode=%s", session->countryCode);
    if (!curl.endpoint || !curl.parameter) {
        status = -1;
        goto end;
    }

    openTIDAL_CurlRequest (session, &curl, "GET", curl.endpoint, curl.parameter, NULL, 0, 0);
    if (curl.status != -1) {
        o->json = openTIDAL_cJSONParseHelper (curl.body);
        if (!o->json) {
            status = -1;
            goto end;
        }

        if (curl.responseCode == 200) {
            openTIDAL_ItemsContainer video;

            json_items_model processed_json = json_parse_items ((cJSON *)o->json);
            parse_items_values (&video, &processed_json);

            o->status = 1;
            status = openTIDAL_StructAddItem (o, video);
        }
        else {
            o->status = parse_status ((cJSON *)o->json, &curl, videoid, NULL);
        }
    }
end:
    if (status == -1) o->status = -14;
    openTIDAL_CurlRequestCleanup (&curl);
    return o;
}

/*openTIDAL_ContentContainer *
openTIDAL_GetVideoContributors( size_t videoid,  int limit,  int offset)
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
openTIDAL_GetVideoStream (openTIDAL_SessionContainer *session, size_t videoid)
{
    openTIDAL_ContentContainer *o = NULL;
    openTIDAL_CurlContainer curl;
    int status = 0;

    openTIDAL_CurlModelInit (&curl);

    status = openTIDAL_StructMainAlloc (&o);
    if (status == -1) return NULL;
    status = openTIDAL_StructInit (o);
    if (status == -1) goto end;
    status = openTIDAL_StructOneTimeAlloc (o, -5);
    if (status == -1) goto end;

    openTIDAL_StringHelper (&curl.endpoint, "/v1/videos/%zu/playbackinfopostpaywall", videoid);
    openTIDAL_StringHelper (&curl.parameter,
                            "countryCode=%s&videoquality=%s&assetpresentation=%s&playbackmode=%s",
                            session->countryCode, session->videoQuality, "FULL", "STREAM");
    if (!curl.endpoint || !curl.parameter) {
        status = -1;
        goto end;
    }

    openTIDAL_CurlRequest (session, &curl, "GET", curl.endpoint, curl.parameter, NULL, 0, 0);
    if (curl.status != -1) {
        o->json = openTIDAL_cJSONParseHelper (curl.body);
        if (!o->json) {
            status = -1;
            goto end;
        }

        if (curl.responseCode == 200) {
            json_stream_model processed_json = json_parse_stream ((cJSON *)o->json);
            parse_stream_values (o->stream, &processed_json);
            o->status = 0;

            if (strcmp (o->stream->manifestMimeType, "application/vnd.tidal.emu") == 0) {
                json_manifest_model processed_manifest;
                char *manifest_decoded = NULL;

                openTIDAL_VerboseHelper ("Base64", "Allocated decoded data in heap", 2);
                manifest_decoded = malloc (openTIDAL_Base64DecodeLen (o->stream->manifest));
                if (!manifest_decoded) {
                    status = -1;
                    goto end;
                }

                openTIDAL_Base64Decode (manifest_decoded, o->stream->manifest);
                o->jsonManifest = openTIDAL_cJSONParseHelper (manifest_decoded);

                o->status = 1;
                o->stream->codec = NULL;
                o->stream->encryptionType = NULL;
                o->stream->audioMode = NULL;
                o->stream->audioQuality = NULL;

                processed_manifest = json_parse_manifest ((cJSON *)o->jsonManifest);
                parse_string (processed_manifest.mimeType, &o->stream->mimeType);
                parse_string (processed_manifest.url, &o->stream->url);

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
end:
    if (status == -1) o->status = -14;
    openTIDAL_CurlRequestCleanup (&curl);
    return o;
}
