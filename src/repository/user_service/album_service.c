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

#include "../../helper/helper.h"
#include "../../helper/struct_helper.h"
#include "../../http_connector.h"
#include "../../openTIDAL.h"
#include "../../parse/parse.h"
#include <stdio.h>
#include <stdlib.h>

openTIDAL_ContentContainer *
openTIDAL_GetFavoriteAlbums (openTIDAL_SessionContainer *session, const int limit, const int offset,
                             const char *order, const char *orderDirection)
{
    openTIDAL_ContentContainer *o = NULL;
    openTIDAL_CurlContainer curl;
    int status = 0;

    openTIDAL_CurlModelInit (&curl);
    status = openTIDAL_StructMainAlloc (&o);
    if (status == -1) return NULL;
    status = openTIDAL_StructInit (o);
    if (status == -1) goto end;
    status = openTIDAL_StructAlloc (o, 0);
    if (status == -1) goto end;

    openTIDAL_StringHelper (&curl.endpoint, "/v1/users/%s/favorites/albums", session->userId);
    openTIDAL_StringHelper (&curl.parameter,
                            "countryCode=%s&limit=%d&offset=%d&order=%s&orderDirection=%s",
                            session->countryCode, limit, offset, order, orderDirection);
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
                    struct openTIDAL_AlbumContainer album;
                    cJSON *innerItem = NULL;

                    innerItem = cJSON_GetObjectItem (item, "item");
                    struct openTIDAL_JsonAlbumContainer processed_json
                        = openTIDAL_ParseJsonAlbum (innerItem);
                    openTIDAL_ParseJsonAlbumValues (&album, &processed_json);
                    openTIDAL_ParseJsonNumber (limit, (size_t *)&album.limit);
                    openTIDAL_ParseJsonNumber (offset, (size_t *)&album.offset);
                    openTIDAL_ParseJsonNumber (totalNumberOfItems,
                                               (size_t *)&album.totalNumberOfItems);

                    status = openTIDAL_StructAddAlbum (o, album);
                    if (status == -1) goto end;
                }
                o->status = 1;
            }
        }
        else {
            o->status = openTIDAL_ParseStatus ((cJSON *)o->json, &curl, session->userId);
        }
    }
end:
    if (status == -1) o->status = -14;
    openTIDAL_CurlRequestCleanup (&curl);
    return o;
}

const int
openTIDAL_AddFavoriteAlbum (openTIDAL_SessionContainer *session, const char *albumId,
                            const char *onArtifactNotFound)
{
    openTIDAL_CurlContainer curl;
    int status = -1;

    openTIDAL_CurlModelInit (&curl);
    openTIDAL_StringHelper (&curl.endpoint, "/v1/users/%s/favorites/albums", session->userId);

    openTIDAL_StringHelper (&curl.parameter, "countryCode=%s", session->countryCode);
    openTIDAL_StringHelper (&curl.postData, "albumIds=%s&onArtifactNotFound=%s", albumId,
                            onArtifactNotFound);
    if (!curl.endpoint || !curl.parameter || !curl.postData) {
        status = -14;
        return status;
    }

    openTIDAL_CurlRequest (session, &curl, "POST", curl.endpoint, curl.parameter, curl.postData, 0,
                           1);
    if (curl.status != -1) {
        status = openTIDAL_ParseResponseCodeStatus (&curl.responseCode);
    }
    openTIDAL_CurlRequestCleanup (&curl);
    return status;
}

const int
openTIDAL_AddFavoriteAlbums (openTIDAL_SessionContainer *session, const char **albumIds,
                             const int size, const char *onArtifactNotFound)
{
    openTIDAL_CurlContainer curl;
    int status = -1;
    char *ptr;

    openTIDAL_CurlModelInit (&curl);

    openTIDAL_ArrayToStringHelper (&ptr, albumIds, size);
    if (!ptr) {
        status = -14;
        return status;
    }
    openTIDAL_StringHelper (&curl.endpoint, "/v1/users/%s/favorites/albums", session->userId);
    openTIDAL_StringHelper (&curl.parameter, "countryCode=%s", session->countryCode);
    openTIDAL_StringHelper (&curl.postData, "albumIds=%s&onArtifactNotFound=%s", ptr,
                            onArtifactNotFound);
    if (!curl.endpoint || !curl.parameter || !curl.postData) {
        status = -14;
        return status;
    }

    openTIDAL_CurlRequest (session, &curl, "POST", curl.endpoint, curl.parameter, curl.postData, 0,
                           1);
    if (curl.status != -1) {
        status = openTIDAL_ParseResponseCodeStatus (&curl.responseCode);
    }
    free (ptr);
    openTIDAL_CurlRequestCleanup (&curl);
    return status;
}

const int
openTIDAL_DeleteFavoriteAlbum (openTIDAL_SessionContainer *session, const char *albumId)
{
    openTIDAL_CurlContainer curl;
    int status = -1;

    openTIDAL_CurlModelInit (&curl);
    openTIDAL_StringHelper (&curl.endpoint, "/v1/users/%s/favorites/albums/%s", session->userId,
                            albumId);
    openTIDAL_StringHelper (&curl.parameter, "countryCode=%s", session->countryCode);
    if (!curl.endpoint || !curl.parameter) {
        status = -14;
        return status;
    }

    openTIDAL_CurlRequest (session, &curl, "DELETE", curl.endpoint, curl.parameter, NULL, 0, 1);
    if (curl.status != -1) {
        status = openTIDAL_ParseResponseCodeStatus (&curl.responseCode);
    }
    openTIDAL_CurlRequestCleanup (&curl);
    return status;
}

