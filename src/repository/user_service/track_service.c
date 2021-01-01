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

#include "../../helper/helper.h"
#include "../../helper/struct_helper.h"
#include "../../http_connector.h"
#include "../../openTIDAL.h"
#include "../../parse/parse.h"

openTIDAL_ContentContainer *
openTIDAL_GetFavoriteTracks (openTIDAL_SessionContainer *session, int limit, int offset,
                             char *order, char *orderDirection)
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

    openTIDAL_StringHelper (&curl.endpoint, "/v1/users/%zu/favorites/tracks", session->userId);
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
                    openTIDAL_ItemsContainer track;
                    cJSON *innerItem = cJSON_GetObjectItem (item, "item");

                    json_items_model processed_json = json_parse_items (innerItem);

                    parse_items_values (&track, &processed_json);
                    parse_signed_number (limit, &track.limit);
                    parse_signed_number (offset, &track.offset);
                    parse_signed_number (totalNumberOfItems, &track.totalNumberOfItems);

                    status = openTIDAL_StructAddItem (o, track);
                    if (status == -1) goto end;
                }
                o->status = 1;
            }
        }
        else {
            o->status = parse_status ((cJSON *)o->json, &curl, session->userId, NULL);
        }
    }
end:
    if (status == -1) o->status = -14;
    openTIDAL_CurlRequestCleanup (&curl);
    return o;
}

int
openTIDAL_AddFavoriteTrack (openTIDAL_SessionContainer *session, size_t trackid)
{
    openTIDAL_CurlContainer curl;
    int status = -1;

    openTIDAL_CurlModelInit (&curl);
    openTIDAL_StringHelper (&curl.endpoint, "/v1/users/%zu/favorites/tracks", session->userId);
    openTIDAL_StringHelper (&curl.parameter, "countryCode=%s", session->countryCode);
    openTIDAL_StringHelper (&curl.postData, "trackIds=%zu&onArtifactNotFound=FAIL", trackid);
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
openTIDAL_DeleteFavoriteTrack (openTIDAL_SessionContainer *session, size_t trackid)
{
    openTIDAL_CurlContainer curl;
    int status = -1;

    openTIDAL_CurlModelInit (&curl);
    openTIDAL_StringHelper (&curl.endpoint, "/v1/users/%zu/favorites/tracks/%zu", session->userId,
                            trackid);
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
