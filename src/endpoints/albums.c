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

#include "../include/handles.h"
#include "../include/openTIDAL.h"
#include "../include/parse.h"
#include <stdio.h>
#include <stdlib.h>

openTIDAL_ContentContainer
openTIDAL_GetAlbum (openTIDAL_SessionContainer *session, const size_t albumid)
{
    openTIDAL_ContentContainer o;
    openTIDAL_CurlContainer curl;

    openTIDAL_StructInit (&o);
    openTIDAL_StructAlloc (&o, 0);

    openTIDAL_StringHelper (&curl.endpoint, "/v1/albums/%zu", albumid);
    openTIDAL_StringHelper (&curl.parameter, "countryCode=%s", session->countryCode);
    if (!curl.endpoint || !curl.parameter) {
        o.status = -14;
        return o;
    }

    openTIDAL_CurlRequest (session, &curl, "GET", curl.endpoint, curl.parameter, NULL, 0, 0);
    if (curl.status != -1) {
        o.json = openTIDAL_cJSONParseHelper (curl.body);
        if (!o.json) {
            o.status = -14;
            return o;
        }

        if (curl.responseCode == 200) {
            openTIDAL_AlbumContainer album;
            json_album_model processed_json = json_parse_album ((cJSON *)o.json);
            parse_album_values (&album, &processed_json);

            o.status = 1;
            openTIDAL_StructAddAlbum (&o, album);
        }
        else {
            o.status = parse_status ((cJSON *)o.json, &curl, albumid, NULL);
        }
    }
    openTIDAL_CurlRequestCleanup (&curl);
    return o;
}

openTIDAL_ContentContainer
openTIDAL_GetAlbumItems (openTIDAL_SessionContainer *session, const size_t albumid, const int limit,
                         const int offset)
{
    openTIDAL_ContentContainer o;
    openTIDAL_CurlContainer curl;

    openTIDAL_StructInit (&o);
    openTIDAL_StructAlloc (&o, 1);

    openTIDAL_StringHelper (&curl.endpoint, "/v1/albums/%zu/items", albumid);
    openTIDAL_StringHelper (&curl.parameter, "countryCode=%s&limit=%d&offset=%d",
                            session->countryCode, limit, offset);
    if (!curl.endpoint || !curl.parameter) {
        o.status = -14;
        return o;
    }

    openTIDAL_CurlRequest (session, &curl, "GET", curl.endpoint, curl.parameter, NULL, 0, 0);
    if (curl.status != -1) {
        o.json = openTIDAL_cJSONParseHelper (curl.body);
        if (!o.json) {
            o.status = -14;
            return o;
        }

        if (curl.responseCode == 200) {
            cJSON *items = NULL;
            cJSON *item = NULL;
            cJSON *limit = NULL;
            cJSON *offset = NULL;
            cJSON *totalNumberOfItems = NULL;

            limit = cJSON_GetObjectItem ((cJSON *)o.json, "limit");
            items = cJSON_GetObjectItem ((cJSON *)o.json, "items");
            offset = cJSON_GetObjectItem ((cJSON *)o.json, "offset");
            totalNumberOfItems = cJSON_GetObjectItem ((cJSON *)o.json, "totalNumberOfItems");

            if (cJSON_IsArray (items)) {
                cJSON_ArrayForEach (item, items)
                {
                    openTIDAL_ItemsContainer Value;
                    cJSON *innerItem = NULL;

                    innerItem = cJSON_GetObjectItem (item, "item");
                    json_items_model processed_json = json_parse_items (innerItem);
                    parse_items_values (&Value, &processed_json);
                    parse_signed_number (limit, &Value.limit);
                    parse_signed_number (offset, &Value.offset);
                    parse_signed_number (totalNumberOfItems, &Value.totalNumberOfItems);

                    openTIDAL_StructAddItem (&o, Value);
                }
                o.status = 1;
            }
        }
        else {
            o.status = parse_status ((cJSON *)o.json, &curl, albumid, NULL);
        }
    }
    openTIDAL_CurlRequestCleanup (&curl);
    return o;
}

openTIDAL_ContentContainer
openTIDAL_GetFavoriteAlbums (openTIDAL_SessionContainer *session, const int limit, const int offset,
                             const char *order, const char *orderDirection)
{
    openTIDAL_ContentContainer o;
    openTIDAL_CurlContainer curl;

    openTIDAL_StructInit (&o);
    openTIDAL_StructAlloc (&o, 0);

    openTIDAL_StringHelper (&curl.endpoint, "/v1/users/%zu/favorites/albums", session->userId);
    openTIDAL_StringHelper (&curl.parameter,
                            "countryCode=%s&limit=%d&offset=%d&order=%s&orderDirection=%s",
                            session->countryCode, limit, offset, order, orderDirection);
    if (!curl.endpoint || !curl.parameter) {
        o.status = -14;
        return o;
    }

    openTIDAL_CurlRequest (session, &curl, "GET", curl.endpoint, curl.parameter, NULL, 0, 0);
    if (curl.status != -1) {
        o.json = openTIDAL_cJSONParseHelper (curl.body);
        if (!o.json) {
            o.status = -14;
            return o;
        }

        if (curl.responseCode == 200) {
            cJSON *items = NULL;
            cJSON *item = NULL;
            cJSON *limit = NULL;
            cJSON *offset = NULL;
            cJSON *totalNumberOfItems = NULL;

            items = cJSON_GetObjectItem ((cJSON *)o.json, "items");
            limit = cJSON_GetObjectItem ((cJSON *)o.json, "limit");
            offset = cJSON_GetObjectItem ((cJSON *)o.json, "offset");
            totalNumberOfItems = cJSON_GetObjectItem ((cJSON *)o.json, "totalNumberOfItems");

            if (cJSON_IsArray (items)) {
                cJSON_ArrayForEach (item, items)
                {
                    openTIDAL_AlbumContainer album;
                    cJSON *innerItem = NULL;

                    innerItem = cJSON_GetObjectItem (item, "item");
                    json_album_model processed_json = json_parse_album (innerItem);
                    parse_album_values (&album, &processed_json);
                    parse_number (limit, (size_t *)&album.limit);
                    parse_number (offset, (size_t *)&album.offset);
                    parse_number (totalNumberOfItems, (size_t *)&album.totalNumberOfItems);

                    openTIDAL_StructAddAlbum (&o, album);
                }
                o.status = 1;
            }
        }
        else {
            o.status = parse_status ((cJSON *)o.json, &curl, session->userId, NULL);
        }
    }
    openTIDAL_CurlRequestCleanup (&curl);
    return o;
}

/* create & delete favourites */

int
openTIDAL_AddFavoriteAlbum (openTIDAL_SessionContainer *session, const size_t albumid)
{
    openTIDAL_CurlContainer curl;
    int status = -1;

    openTIDAL_StringHelper (&curl.endpoint, "/v1/users/%zu/favorites/albums", session->userId);
    openTIDAL_StringHelper (&curl.parameter, "countryCode=%s", session->countryCode);
    openTIDAL_StringHelper (&curl.postData, "albumIds=%zu&onArtifactNotFound=FAIL", albumid);
    if (!curl.endpoint || !curl.parameter || !curl.postData) {
        status = -14;
        return status;
    }

    openTIDAL_CurlRequest (session, &curl, "POST", curl.endpoint, curl.parameter, curl.postData, 0,
                           1);
    if (curl.status != -1) {
        status = parse_raw_status (&curl.responseCode);
    }
    openTIDAL_CurlRequestCleanup (&curl);
    return status;
}

int
openTIDAL_DeleteFavoriteAlbum (openTIDAL_SessionContainer *session, const size_t albumid)
{
    openTIDAL_CurlContainer curl;
    int status = -1;

    openTIDAL_StringHelper (&curl.endpoint, "/v1/users/%zu/favorites/albums/%zu", session->userId,
                            albumid);
    openTIDAL_StringHelper (&curl.parameter, "countryCode=%s", session->countryCode);
    if (!curl.endpoint || !curl.parameter) {
        status = -14;
        return status;
    }

    openTIDAL_CurlRequest (session, &curl, "DELETE", curl.endpoint, curl.parameter, NULL, 0, 1);
    if (curl.status != -1) {
        status = parse_raw_status (&curl.responseCode);
    }
    openTIDAL_CurlRequestCleanup (&curl);
    return status;
}

