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
openTIDAL_GetTrack (openTIDAL_SessionContainer *session, const size_t trackid)
{
    openTIDAL_ContentContainer *o = NULL;
    openTIDAL_CurlContainer curl;

    openTIDAL_StructMainAlloc (&o);
    openTIDAL_StructInit (o);
    openTIDAL_StructAlloc (o, 1);

    openTIDAL_StringHelper (&curl.endpoint, "/v1/tracks/%zu", trackid);
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
            openTIDAL_ItemsContainer track;

            json_items_model processed_json = json_parse_items ((cJSON *)o->json);
            parse_items_values (&track, &processed_json);

            o->status = 1;
            openTIDAL_StructAddItem (o, track);
        }
        else {
            o->status = parse_status ((cJSON *)o->json, &curl, trackid, NULL);
        }
    }
    openTIDAL_CurlRequestCleanup (&curl);
    return o;
}

openTIDAL_ContentContainer *
openTIDAL_GetFavoriteTracks (openTIDAL_SessionContainer *session, const int limit, const int offset,
                             const char *order, const char *orderDirection)
{
    openTIDAL_ContentContainer *o = NULL;
    openTIDAL_CurlContainer curl;

    openTIDAL_StructMainAlloc (&o);
    openTIDAL_StructInit (o);
    openTIDAL_StructAlloc (o, 1);

    openTIDAL_StringHelper (&curl.endpoint, "/v1/users/%zu/favorites/tracks", session->userId);
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
                    openTIDAL_ItemsContainer track;
                    cJSON *innerItem = cJSON_GetObjectItem (item, "item");

                    json_items_model processed_json = json_parse_items (innerItem);

                    parse_items_values (&track, &processed_json);
                    parse_signed_number (limit, &track.limit);
                    parse_signed_number (offset, &track.offset);
                    parse_signed_number (totalNumberOfItems, &track.totalNumberOfItems);

                    openTIDAL_StructAddItem (o, track);
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

openTIDAL_ContentContainer *
openTIDAL_GetTrackContributors (openTIDAL_SessionContainer *session, const size_t trackid,
                                const int limit, const int offset)
{
    openTIDAL_ContentContainer *o = NULL;
    openTIDAL_CurlContainer curl;

    openTIDAL_StructMainAlloc (&o);
    openTIDAL_StructInit (o);
    openTIDAL_StructAlloc (o, 5);

    openTIDAL_StringHelper (&curl.endpoint, "/v1/tracks/%zu/contributors", trackid);
    openTIDAL_StringHelper (&curl.parameter, "countryCode=%s&limit=%d&offset=%d",
                            session->countryCode, limit, offset);

    openTIDAL_CurlRequest (session, &curl, "GET", curl.endpoint, curl.parameter, NULL, 0, 0);
    if (curl.status != -1) {
        o->json = openTIDAL_cJSONParseHelper (curl.body);
        if (!o->json) {
            o->status = -14;
            return o;
        }

        if (curl.responseCode == 200) {
            cJSON *limit = cJSON_GetObjectItem ((cJSON *)o->json, "limit");
            cJSON *offset = cJSON_GetObjectItem ((cJSON *)o->json, "offset");
            cJSON *totalNumberOfItems
                = cJSON_GetObjectItem ((cJSON *)o->json, "totalNumberOfItems");
            cJSON *items = cJSON_GetObjectItem ((cJSON *)o->json, "items");
            cJSON *item = NULL;

            if (cJSON_IsArray (items)) {
                cJSON_ArrayForEach (item, items)
                {
                    openTIDAL_ContributorContainer contrib;
                    json_contributor_model processed_json = json_parse_contributors (item);

                    parse_contributor_values (&contrib, &processed_json);
                    parse_signed_number (limit, &contrib.limit);
                    parse_signed_number (offset, &contrib.offset);
                    parse_signed_number (totalNumberOfItems, &contrib.totalNumberOfItems);

                    openTIDAL_StructAddContributor (o, contrib);
                }
                o->status = 1;
            }
        }
        else {
            o->status = parse_status ((cJSON *)o->json, &curl, trackid, NULL);
        }
    }
    openTIDAL_CurlRequestCleanup (&curl);
    return o;
}

openTIDAL_ContentContainer *
openTIDAL_GetTrackMix (openTIDAL_SessionContainer *session, const size_t trackid)
{
    openTIDAL_ContentContainer *o = NULL;
    openTIDAL_CurlContainer curl;

    openTIDAL_StructMainAlloc (&o);
    openTIDAL_StructInit (o);
    openTIDAL_StructAlloc (o, 4);

    openTIDAL_StringHelper (&curl.endpoint, "/v1/tracks/%zu/mix", trackid);
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
            openTIDAL_MixContainer mix;

            json_mix_model processed_json = json_parse_mix ((cJSON *)o->json);
            parse_mix_values (&mix, &processed_json);

            o->status = 1;
            openTIDAL_StructAddMix (o, mix);
        }
        else {
            o->status = parse_status ((cJSON *)o->json, &curl, trackid, NULL);
        }
    }
    openTIDAL_CurlRequestCleanup (&curl);
    return o;
}

openTIDAL_ContentContainer *
openTIDAL_GetTrackStream (openTIDAL_SessionContainer *session, const size_t trackid)
{
    openTIDAL_ContentContainer *o = NULL;
    openTIDAL_CurlContainer curl;

    openTIDAL_StructMainAlloc (&o);
    openTIDAL_StructInit (o);

    openTIDAL_StringHelper (&curl.endpoint, "/v1/tracks/%zu/playbackinfopostpaywall", trackid);
    openTIDAL_StringHelper (&curl.parameter,
                            "countryCode=%s&audioquality=%s&assetpresentation=%s&playbackmode=%s",
                            session->countryCode, session->audioQuality, "FULL", "STREAM");

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

            if (strcmp (o->stream.manifestMimeType, "application/vnd.tidal.bts") == 0) {
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
                if (!o->jsonManifest) {
                    o->status = -14;
                    return o;
                }

                processed_manifest = json_parse_manifest ((cJSON *)o->jsonManifest);
                parse_string (processed_manifest.mimeType, &o->stream.mimeType);
                parse_string (processed_manifest.codec, &o->stream.codec);
                parse_string (processed_manifest.encryptionType, &o->stream.encryptionType);
                parse_string (processed_manifest.url, &o->stream.url);
                o->status = 1;
                free (manifest_decoded);
            }
            else {
                o->status = -10;
                openTIDAL_VerboseHelper (
                    "Request Error",
                    "Not a valid manifest. MimeType is not application/vnd.tidal.bts", 1);
            }
        }
        else {
            o->status = parse_status ((cJSON *)o->json, &curl, trackid, NULL);
        }
    }
    openTIDAL_CurlRequestCleanup (&curl);
    return o;
}

/* create & delete favourites */

/*int openTIDAL_AddFavoriteTrack(const size_t trackid)
{
    char *endpoint = url_cat(session, "/v1/users/", session->userId, "/favorites/tracks", 1);
    int status;
    char buffer[100];
    snprintf(buffer, 100, "trackIds=%zu&onArtifactNotFound=FAIL", trackid);

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

int openTIDAL_DeleteFavoriteTrack(const size_t trackid)
{
    int status;
    char buffer[80];
    snprintf(buffer, 80, "/v1/users/%zu/favorites/tracks/%zu?countryCode=%s", session->userId,
trackid, session->countryCode);

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
}
*/
