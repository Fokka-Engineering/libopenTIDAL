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
            struct openTIDAL_PlaylistContainer Value;

            struct openTIDAL_JsonPlaylistContainer processed_json
                = openTIDAL_ParseJsonPlaylist ((cJSON *)o->json);
            openTIDAL_ParseJsonPlaylistValues (&Value, &processed_json);

            o->status = 1;
            status = openTIDAL_StructAddPlaylist (o, Value);
        }
        else {
            o->status = openTIDAL_ParseStatus ((cJSON *)o->json, &curl, playlistId);
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
                    struct openTIDAL_ItemsContainer Value;
                    cJSON *innerItem = cJSON_GetObjectItem (item, "item");

                    struct openTIDAL_JsonItemsContainer processed_json
                        = openTIDAL_ParseJsonItems (innerItem);
                    openTIDAL_ParseJsonItemsValues (&Value, &processed_json);
                    openTIDAL_ParseJsonSignedNumber (limit, &Value.limit);
                    openTIDAL_ParseJsonSignedNumber (offset, &Value.offset);
                    openTIDAL_ParseJsonSignedNumber (totalNumberOfItems, &Value.totalNumberOfItems);

                    status = openTIDAL_StructAddItem (o, Value);
                    if (status == -1) goto end;
                }
                o->status = 1;
            }
        }
        else {
            o->status = openTIDAL_ParseStatus ((cJSON *)o->json, &curl, playlistId);
        }
    }
end:
    if (status == -1) o->status = -14;
    openTIDAL_CurlRequestCleanup (&curl);
    return o;
}

openTIDAL_ContentContainer *
openTIDAL_CreatePlaylist (openTIDAL_SessionContainer *session, char *title, char *description)
{
    openTIDAL_ContentContainer *o;
    openTIDAL_CurlContainer curl;
    char *encodedTitle = NULL;
    char *encodedDescription = NULL;
    int status = 0;

    openTIDAL_CurlModelInit (&curl);
    status = openTIDAL_StructMainAlloc (&o);
    if (status == -1) return NULL;
    status = openTIDAL_StructInit (o);
    if (status == -1) goto end;
    status = openTIDAL_StructAlloc (o, 3);
    if (status == -1) goto end;

    encodedTitle = openTIDAL_UrlEncodeHelper (title);
    encodedDescription = openTIDAL_UrlEncodeHelper (description);
    if (!encodedTitle || !encodedDescription) {
        status = -1;
        goto end;
    }

    openTIDAL_StringHelper (&curl.endpoint, "/v1/users/%s/playlists", session->userId);
    openTIDAL_StringHelper (&curl.parameter, "countryCode=%s", session->countryCode);
    openTIDAL_StringHelper (&curl.postData, "title=%s&description=%s", encodedTitle,
                            encodedDescription);
    if (!curl.endpoint || !curl.parameter || !curl.postData) {
        status = -1;
        goto end;
    }

    openTIDAL_CurlRequest (session, &curl, "POST", curl.endpoint, curl.parameter, curl.postData, 0,
                           0);
    if (curl.status != -1) {
        if (curl.responseCode == 201) {
            struct openTIDAL_PlaylistContainer playlist;
            struct openTIDAL_JsonPlaylistContainer processed_json;

            processed_json = openTIDAL_ParseJsonPlaylist ((cJSON *)o->json);
            openTIDAL_ParseJsonPlaylistValues (&playlist, &processed_json);

            o->status = 1;
            openTIDAL_StructAddPlaylist (o, playlist);
        }
    }
    else {
        o->status = openTIDAL_ParseStatus ((cJSON *)o->json, &curl, "Playlist");
    }
end:
    if (status == -1) o->status = -14;
    free (encodedTitle);
    free (encodedDescription);
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
            ptr[strlen (ptr) - 1] = '\0';
            value = strdup (ptr);
        }
    }
end:
    if (status == -1) return NULL;
    openTIDAL_CurlRequestCleanup (&curl);
    return value;
}

const int
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
        status = openTIDAL_ParseResponseCodeStatus (&curl.responseCode);
    }
    openTIDAL_CurlRequestCleanup (&curl);
    return status;
}

const int
openTIDAL_DeletePlaylistItem (openTIDAL_SessionContainer *session, const char *playlistId,
                              int index)
{
    openTIDAL_CurlContainer curl;
    char *value = NULL;
    int status = -1;

    openTIDAL_CurlModelInit (&curl);
    value = openTIDAL_GetPlaylistEntityTag (session, playlistId);
    if (!value) {
        status = -4;
        goto end;
    }

    openTIDAL_StringHelper (&curl.endpoint, "/v1/playlists/%s/items/%d", playlistId, index);
    openTIDAL_StringHelper (&curl.parameter, "countryCode=%s", session->countryCode);
    openTIDAL_StringHelper (&curl.entityTagHeader, "if-none-match: %s", value);
    if (!curl.endpoint || !curl.parameter || !curl.entityTagHeader) {
        status = -14;
        goto end;
    }

    openTIDAL_CurlRequest (session, &curl, "DELETE", curl.endpoint, curl.parameter, NULL, 0, 0);
    if (curl.status != -1) {
        status = openTIDAL_ParseResponseCodeStatus (&curl.responseCode);
    }
end:
    free (value);
    openTIDAL_CurlRequestCleanup (&curl);
    return status;
}

const int
openTIDAL_MovePlaylistItem (openTIDAL_SessionContainer *session, const char *playlistId,
                            const int index, const int toIndex)
{
    openTIDAL_CurlContainer curl;
    char *value = NULL;
    int status = -1;

    openTIDAL_CurlModelInit (&curl);
    value = openTIDAL_GetPlaylistEntityTag (session, playlistId);
    if (!value) {
        status = -4;
        goto end;
    }

    openTIDAL_StringHelper (&curl.endpoint, "/v1/playlists/%s/items/%d", playlistId, index);
    openTIDAL_StringHelper (&curl.parameter, "countryCode=%s", session->countryCode);
    openTIDAL_StringHelper (&curl.postData, "toIndex=%d", toIndex);
    openTIDAL_StringHelper (&curl.entityTagHeader, "if-none-match: %s", value);
    if (!curl.endpoint || !curl.parameter || !curl.entityTagHeader || !curl.postData) {
        status = -14;
        goto end;
    }

    openTIDAL_CurlRequest (session, &curl, "POST", curl.endpoint, curl.parameter, curl.postData, 0,
                           1);
    if (curl.status != -1) {
        status = openTIDAL_ParseResponseCodeStatus (&curl.responseCode);
    }
end:
    free (value);
    openTIDAL_CurlRequestCleanup (&curl);
    return status;
}

const int
openTIDAL_AddPlaylistItem (openTIDAL_SessionContainer *session, const char *playlistId,
                           const char *itemId, const char *onArtifactNotFound, const char *onDupes)
{
    openTIDAL_CurlContainer curl;
    char *value = NULL;
    int status = -1;

    openTIDAL_CurlModelInit (&curl);
    value = openTIDAL_GetPlaylistEntityTag (session, playlistId);
    if (!value) {
        status = -4;
        goto end;
    }
    openTIDAL_StringHelper (&curl.endpoint, "/v1/playlists/%s/items/", playlistId);
    openTIDAL_StringHelper (&curl.parameter, "countryCode=%s", session->countryCode);
    openTIDAL_StringHelper (&curl.postData, "itemIds=%s&onArtifactNotFound=%s&onDupes=%s", itemId,
                            onArtifactNotFound, onDupes);
    openTIDAL_StringHelper (&curl.entityTagHeader, "If-None-Match: %s", value);
    if (!curl.endpoint || !curl.parameter || !curl.postData || !curl.entityTagHeader) {
        status = -14;
        goto end;
    }

    openTIDAL_CurlRequest (session, &curl, "POST", curl.endpoint, curl.parameter, curl.postData, 0,
                           1);
    if (curl.status != -1) {
        status = openTIDAL_ParseResponseCodeStatus (&curl.responseCode);
    }
end:
    free (value);
    openTIDAL_CurlRequestCleanup (&curl);
    return status;
}

const int
openTIDAL_AddPlaylistItems (openTIDAL_SessionContainer *session, const char *playlistId,
                            const char **itemIds, const int size, const char *onArtifactNotFound,
                            const char *onDupes)
{
    openTIDAL_CurlContainer curl;
    char *value = NULL;
    char *string = NULL;
    int status = -1;

    openTIDAL_CurlModelInit (&curl);
    value = openTIDAL_GetPlaylistEntityTag (session, playlistId);
    if (!value) {
        status = -4;
        goto end;
    }

    openTIDAL_ArrayToStringHelper (&string, itemIds, size);
    if (!string) {
        status = -14;
        goto end;
    }
    openTIDAL_StringHelper (&curl.endpoint, "/v1/playlists/%s/items/", playlistId);
    openTIDAL_StringHelper (&curl.parameter, "countryCode=%s", session->countryCode);
    openTIDAL_StringHelper (&curl.postData, "itemIds=%s&onArtifactNotFound=%s&onDupes=%s", string,
                            onArtifactNotFound, onDupes);
    openTIDAL_StringHelper (&curl.entityTagHeader, "If-None-Match: %s", value);
    if (!curl.endpoint || !curl.parameter || !curl.postData || !curl.entityTagHeader) {
        status = -14;
        goto end;
    }

    openTIDAL_CurlRequest (session, &curl, "POST", curl.endpoint, curl.parameter, curl.postData, 0,
                           1);
    if (curl.status != -1) {
        status = openTIDAL_ParseResponseCodeStatus (&curl.responseCode);
    }
end:
    free (string);
    free (value);
    openTIDAL_CurlRequestCleanup (&curl);
    return status;
}
