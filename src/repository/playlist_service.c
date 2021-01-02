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

#include "../helper/helper.h"
#include "../helper/struct_helper.h"
#include "../http_connector.h"
#include "../openTIDAL.h"
#include "../parse/parse.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

openTIDAL_ContentContainer *
openTIDAL_GetPlaylist (openTIDAL_SessionContainer *session, const char *playlistId)
{
    openTIDAL_ContentContainer *o = NULL;
    openTIDAL_CurlContainer curl;
    int status = 0;

    openTIDAL_CurlModelInit (&curl);
    status = openTIDAL_StructMainAlloc (&o);
    if (status == -1) return NULL;
    status = openTIDAL_StructInit (o);
    if (status == -1) goto end;
    status = openTIDAL_StructAlloc (o, 3);
    if (status == -1) goto end;

    openTIDAL_StringHelper (&curl.endpoint, "/v1/playlists/%s", playlistId);
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
            openTIDAL_PlaylistContainer Value;

            json_playlist_model processed_json = json_parse_playlist ((cJSON *)o->json);
            parse_playlist_values (&Value, &processed_json);

            o->status = 1;
            status = openTIDAL_StructAddPlaylist (o, Value);
        }
        else {
            o->status = parse_status ((cJSON *)o->json, &curl, playlistId);
        }
    }
end:
    if (status == -1) o->status = -14;
    openTIDAL_CurlRequestCleanup (&curl);
    return o;
}

openTIDAL_ContentContainer *
openTIDAL_GetPlaylistItems (openTIDAL_SessionContainer *session, const char *playlistId, int limit,
                            int offset)
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

    openTIDAL_StringHelper (&curl.endpoint, "/v1/playlists/%s/items", playlistId);
    openTIDAL_StringHelper (&curl.parameter, "countryCode=%s&limit=%d&offset=%d",
                            session->countryCode, limit, offset);
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
            cJSON *limit = NULL;
            cJSON *offset = NULL;
            cJSON *totalNumberOfItems = NULL;
            cJSON *items = NULL;
            cJSON *item = NULL;

            limit = cJSON_GetObjectItem ((cJSON *)o->json, "limit");
            offset = cJSON_GetObjectItem ((cJSON *)o->json, "offset");
            totalNumberOfItems = cJSON_GetObjectItem ((cJSON *)o->json, "totalNumberOfItems");
            items = cJSON_GetObjectItem ((cJSON *)o->json, "items");

            if (cJSON_IsArray (items)) {
                cJSON_ArrayForEach (item, items)
                {
                    openTIDAL_ItemsContainer Value;
                    cJSON *innerItem = cJSON_GetObjectItem (item, "item");

                    json_items_model processed_json = json_parse_items (innerItem);
                    parse_items_values (&Value, &processed_json);
                    parse_signed_number (limit, &Value.limit);
                    parse_signed_number (offset, &Value.offset);
                    parse_signed_number (totalNumberOfItems, &Value.totalNumberOfItems);

                    status = openTIDAL_StructAddItem (o, Value);
                    if (status == -1) goto end;
                }
                o->status = 1;
            }
        }
        else {
            o->status = parse_status ((cJSON *)o->json, &curl, playlistId);
        }
    }
end:
    if (status == -1) o->status = -14;
    openTIDAL_CurlRequestCleanup (&curl);
    return o;
}

char *
openTIDAL_GetPlaylistEntityTag (openTIDAL_SessionContainer *session, const char *playlistId)
{
    openTIDAL_CurlContainer curl;
    int status = 0;
    char *ptr = NULL;
    char *value = NULL;

    openTIDAL_CurlModelInit (&curl);

    openTIDAL_StringHelper (&curl.endpoint, "/v1/playlists/%s", playlistId);
    openTIDAL_StringHelper (&curl.parameter, "countryCode=%s", session->countryCode);
    if (!curl.endpoint || !curl.parameter) {
        status = -1;
        goto end;
    }

    openTIDAL_CurlRequest (session, &curl, "HEAD", curl.endpoint, curl.parameter, NULL, 0, 0);
    if (curl.status != -1) {
        if (curl.responseCode == 200) {
            openTIDAL_ParseHeaderKeyHelper (curl.header, "etag", &ptr);
            value = strdup (ptr);
        }
    }
end:
    if (status == -1) return NULL;
    openTIDAL_CurlRequestCleanup (&curl);
    return value;
}

int
openTIDAL_DeletePlaylist (openTIDAL_SessionContainer *session, const char *playlistId)
{
    openTIDAL_CurlContainer curl;
    int status = -1;
    openTIDAL_CurlModelInit (&curl);

    openTIDAL_StringHelper (&curl.endpoint, "/v1/playlists/%s", playlistId);
    openTIDAL_StringHelper (&curl.parameter, "countryCode=%s", session->countryCode);
    if (!curl.endpoint || !curl.parameter) {
        status = -14;
        return status;
    }

    openTIDAL_CurlRequest (session, &curl, "DELETE", curl.endpoint, curl.parameter, NULL, 0, 0);
    if (curl.status != -1) {
        status = parse_raw_status (&curl.responseCode);
    }
    openTIDAL_CurlRequestCleanup (&curl);
    return status;
}

int
openTIDAL_DeletePlaylistItem (openTIDAL_SessionContainer *session, const char *playlistId,
                              int index)
{
    openTIDAL_CurlContainer curl;
    char *value;
    int status = -1;

    openTIDAL_CurlModelInit (&curl);
    value = openTIDAL_GetPlaylistEntityTag (session, playlistId);
    if (!value) {
        status = -4;
        return status;
    }

    openTIDAL_StringHelper (&curl.endpoint, "/v1/playlists/%s/items/%d", playlistId, index);
    openTIDAL_StringHelper (&curl.parameter, "countryCode=%s", session->countryCode);
    openTIDAL_StringHelper (&curl.entityTagHeader, "if-none-match: %s", value);
    if (!curl.endpoint || !curl.parameter || !curl.entityTagHeader) {
        status = -14;
        return status;
    }

    openTIDAL_CurlRequest (session, &curl, "DELETE", curl.endpoint, curl.parameter, NULL, 0, 0);
    if (curl.status != -1) {
        status = parse_raw_status (&curl.responseCode);
    }
    free (value);
    openTIDAL_CurlRequestCleanup (&curl);
    return status;
}

int
openTIDAL_MovePlaylistItem (openTIDAL_SessionContainer *session, const char *playlistId, int index,
                            int toIndex)
{
    openTIDAL_CurlContainer curl;
    char *value;
    int status = -1;

    openTIDAL_CurlModelInit (&curl);
    value = openTIDAL_GetPlaylistEntityTag (session, playlistId);
    if (!value) {
        status = -4;
        return status;
    }

    openTIDAL_StringHelper (&curl.endpoint, "/v1/playlists/%s/items/%d", playlistId, index);
    openTIDAL_StringHelper (&curl.parameter, "countryCode=%s", session->countryCode);
    openTIDAL_StringHelper (&curl.postData, "toIndex=%d", toIndex);
    openTIDAL_StringHelper (&curl.entityTagHeader, "if-none-match: %s", value);
    if (!curl.endpoint || !curl.parameter || !curl.entityTagHeader || !curl.postData) {
        status = -14;
        return status;
    }

    openTIDAL_CurlRequest (session, &curl, "POST", curl.endpoint, curl.parameter, curl.postData, 0,
                           0);
    if (curl.status != -1) {
        status = parse_raw_status (&curl.responseCode);
    }
    free (value);
    openTIDAL_CurlRequestCleanup (&curl);
    return status;
}

int
openTIDAL_AddPlaylistItem (openTIDAL_SessionContainer *session, const char *playlistId,
                           size_t itemid, const char *onArtifactNotFound, const char *onDupes)
{
    openTIDAL_CurlContainer curl;
    char *value;
    int status = -1;

    openTIDAL_CurlModelInit (&curl);
    value = openTIDAL_GetPlaylistEntityTag (session, playlistId);
    if (!value) {
        status = -4;
        return status;
    }

    openTIDAL_StringHelper (&curl.endpoint, "/v1/playlists/%s/items/", playlistId);
    openTIDAL_StringHelper (&curl.parameter, "countryCode=%s", session->countryCode);
    openTIDAL_StringHelper (&curl.postData, "itemIds=%zu&onArtifactNotFound=%s&onDupes=%s", itemid,
                            onArtifactNotFound, onDupes);
    openTIDAL_StringHelper (&curl.entityTagHeader, "if-none-match: %s", value);
    if (!curl.endpoint || !curl.parameter || !curl.entityTagHeader || !curl.postData) {
        status = -14;
        return status;
    }

    openTIDAL_CurlRequest (session, &curl, "POST", curl.endpoint, curl.parameter, curl.postData, 0,
                           0);
    if (curl.status != -1) {
        status = parse_raw_status (&curl.responseCode);
    }
    free (value);
    openTIDAL_CurlRequestCleanup (&curl);
    return status;
}

/*openTIDAL_ContentContainer * openTIDAL_CreatePlaylist( const char *title,  const char
*description)
{
    openTIDAL_ContentContainer * o;
    char *endpoint = url_cat(session, "/v1/users/", config.userId, "/playlists", 1);
    char *data = malloc(strlen(title)+strlen(description)+7+14+1);
    strcpy(data, "title=");
    strcat(data, title);
    strcat(data, "&description=");
    strcat(data, description);

    openTIDAL_StructInit(&o);
    openTIDAL_StructAlloc(&o, 3);

    curl_model req = curl_post(endpoint, data, "");
    free(endpoint);
    free(data);

    if (req.status != -1)
    {
        cJSON *(cJSON *)o->json = json_parse(req.body);
        if (req.responseCode == 201 || req.responseCode == 200)
        {
            openTIDAL_PlaylistModel playlist;

            json_playlist_model processed_json = json_parse_playlist((cJSON *)o->json);
            parse_playlist_values(&playlist, &processed_json);
            o->status = 1;
            openTIDAL_StructAddPlaylist(&o, playlist);
        }
        else
        {
            o->status = parse_status((cJSON *)o->json, req, config.userId, NULL);
        }

        free(req.body);
        o->json = (cJSON *)o->json;
        return o;
    }
    else
    {
        o->status = -1;
        free(req.body);
        return o;
    }
} */
