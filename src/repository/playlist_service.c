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

#include "../helper/helper.h"
#include "../helper/struct_helper.h"
#include "../http_connector.h"
#include "../openTIDAL.h"
#include "../parse/parse.h"

#include <stdio.h>
#include <stdlib.h>

openTIDAL_ContentContainer *
openTIDAL_GetPlaylist (openTIDAL_SessionContainer *session, const char *playlistid)
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

    openTIDAL_StringHelper (&curl.endpoint, "/v1/playlists/%s", playlistid);
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
            o->status = parse_status ((cJSON *)o->json, &curl, 0, playlistid);
        }
    }
end:
    if (status == -1) o->status = -14;
    openTIDAL_CurlRequestCleanup (&curl);
    return o;
}

openTIDAL_ContentContainer *
openTIDAL_GetPlaylistItems (openTIDAL_SessionContainer *session, const char *playlistid,
                            const int limit, const int offset)
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

    openTIDAL_StringHelper (&curl.endpoint, "/v1/playlists/%s/items", playlistid);
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
            o->status = parse_status ((cJSON *)o->json, &curl, 0, playlistid);
        }
    }
end:
    if (status == -1) o->status = -14;
    openTIDAL_CurlRequestCleanup (&curl);
    return o;
}

openTIDAL_ContentContainer *
openTIDAL_GetFavoritePlaylists (openTIDAL_SessionContainer *session, const int limit,
                                const int offset, const char *order, const char *orderDirection)
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

    openTIDAL_StringHelper (&curl.endpoint, "/v1/users/%zu/playlistsAndFavoritePlaylists",
                            session->userId);
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
                    openTIDAL_PlaylistContainer playlist;
                    cJSON *innerItem = cJSON_GetObjectItem (item, "playlist");

                    json_playlist_model processed_json = json_parse_playlist (innerItem);

                    parse_playlist_values (&playlist, &processed_json);
                    parse_signed_number (limit, &playlist.limit);
                    parse_signed_number (offset, &playlist.offset);
                    parse_signed_number (totalNumberOfItems, &playlist.totalNumberOfItems);

                    status = openTIDAL_StructAddPlaylist (o, playlist);
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

/*openTIDAL_ETagContainer
    openTIDAL_GetPlaylistETag (openTIDAL_SessionContainer *session, const char *playlistid)
    {
        openTIDAL_ETagContainer Value;
        openTIDAL_CurlContainer curl;
        Request playlist endpoint to scrape eTag Header
        char *endpoint;
        char baseparams[20];

        endpoint = url_cat_str (session, "/v1/playlists/", playlistid, "");
        snprintf (baseparams, 20, "countryCode=%s", session->countryCode);

        openTIDAL_CurlRequest (session, &curl, "HEAD", endpoint, baseparams, NULL, 0, 0);
        if (curl.status != -1) {
            if (curl.responseCode == 200) {*/
// size_t i = 0; /* Counter for Buffer (Header) Splitter  */
// size_t e;     /* Counter for ETag-String Extraction  */
// char *p = strtok (curl.header, "\n");
// char eTag[32];
/* Buffer for splitted HTTP-Header  */
// char *array[30];
// while (p != NULL) {
// array[i++] = p;
/* Split if char is \n  */
// p = strtok (NULL, "\n");
//}
// for (e = 0; e < i; ++e) {
/* If String begins with ETag  */
/*if (strncmp (array[e], "ETag", 4) == 0) {
    size_t charCounter;
    size_t eTagCounter = 0;*/
/* Extract ETag Values */
/*for (charCounter = 7; charCounter < strlen (array[e]); ++charCounter) {
    if (array[e][charCounter] != '"') {
        strcpy (&eTag[eTagCounter], &array[e][charCounter]);
        eTagCounter = eTagCounter + 1;
    }
}
strcat (&eTag[eTagCounter + 1], "\0");
}
}*/
/* copy eTag to struct */
/*            Value.id = eTag;
            Value.status = 1;
        }
        else {
            openTIDAL_ParseVerbose ("Request Error",
                                    "Could not parse eTag-Header. Not a 200 Response", 1);
            Value.status = 0;
        }
    }
    else {
        Value.status = -1;
    }

    openTIDAL_CurlRequestCleanup (&curl);
    free (endpoint);
    return Value;
}*/

/*int openTIDAL_DeletePlaylist(openTIDAL_SessionContainer *session, const char *playlistid)
{
    openTIDAL_CurlContainer curl;
    char buffer[100];

    snprintf(buffer, 100, "/v1/playlists/%s?countryCode=%s", playlistid, session->countryCode);

    curl_model req = curl_delete(buffer, "", "");
    if (req.status != -1)
    {
        int status = parse_raw_status(&req.responseCode);
        return status;
    }
    else
    {
        return -1;
    }
}

int openTIDAL_DeletePlaylistItem(const char *playlistid, const int index)
{
    char buffer[100];
    char etag_buffer[50];

    openTIDAL_ETagModel etag = openTIDAL_GetPlaylistETag(playlistid);

    snprintf(etag_buffer, 50, "if-none-match: %s", etag.id);
    snprintf(buffer, 100, "/v1/playlists/%s/items/%d?countryCode=%s", playlistid, index,
config.countryCode); curl_model req = curl_delete(buffer, "", etag_buffer);

    free(req.body);

    if (req.status != -1)
    {
        int status = parse_raw_status(&req.responseCode);
        return status;
    }
    else
    {
        return -1;
    }
}

int openTIDAL_MovePlaylistItem(const char *playlistid, const int index, const int toIndex)
{
    char buffer[100];
    char etag_buffer[50];
    char index_buffer[20];

    openTIDAL_ETagModel etag = openTIDAL_GetPlaylistETag(playlistid);

    snprintf(etag_buffer, 50, "if-none-match: %s", etag.id);
    snprintf(index_buffer, 20, "toIndex=%d", toIndex);
    snprintf(buffer, 100, "/v1/playlists/%s/items/%d?countryCode=%s", playlistid, index,
config.countryCode);

    curl_model req = curl_post(buffer, index_buffer, etag_buffer);

    free(req.body);

    if (req.status != -1)
    {
        int status = parse_raw_status(&req.responseCode);
        return status;
    }
    else
    {
        return -1;
    }
}

int openTIDAL_AddPlaylistItem(const char *playlistid, const int trackid, const char *onDupes)
{
    char buffer[100];
    char etag_buffer[50];
    char index_buffer[100];

    openTIDAL_ETagModel etag = openTIDAL_GetPlaylistETag(playlistid);

    snprintf(etag_buffer, 50, "if-none-match: %s", etag.id);
    snprintf(index_buffer, 100, "trackIds=%d&onArtifactNotFound=%s&onDupes=%s", trackid, "FAIL",
onDupes); snprintf(buffer, 100, "/v1/playlists/%s/items?countryCode=%s", playlistid,
config.countryCode);

    curl_model req = curl_post(buffer, index_buffer, etag_buffer);
    free(req.body);

    if (req.status != -1)
    {
        int status = parse_raw_status(&req.responseCode);
        return status;
    }
    else
    {
        return -1;
    }

}

int openTIDAL_AddPlaylistItems(const char *playlistid, const char *trackids, const char
*onDupes)
{
    char buffer[100];
    char etag_buffer[50];
    char index_buffer[1000];

    openTIDAL_ETagModel etag = openTIDAL_GetPlaylistETag(playlistid);

    snprintf(etag_buffer, 50, "if-none-match: %s", etag.id);
    snprintf(index_buffer, 1000, "trackIds=%s&onArtifactNotFound=%s&onDupes=%s", trackids,
"FAIL", onDupes); snprintf(buffer, 100, "/v1/playlists/%s/items?countryCode=%s", playlistid,
config.countryCode);

    curl_model req = curl_post(buffer, index_buffer, etag_buffer);

    free(req.body);

    if (req.status != -1)
    {
        int status = parse_raw_status(&req.responseCode);
        return status;
    }
    else
    {
        return -1;
    }

}*/

/* create & delete favourites */

/*openTIDAL_ContentContainer * openTIDAL_CreatePlaylist(const char *title, const char
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
}


int openTIDAL_AddFavoritePlaylist(const char *playlistid)
{
    char *endpoint = url_cat(session, "/v1/users/", config.userId, "/favorites/playlists", 1);
    int status;
    char *data = malloc(strlen(playlistid)+10+25+1);
    strcpy(data, "uuids=");
    strcat(data, playlistid);
    strcat(data, "&onArtifactNotFound=FAIL");

    curl_model req = curl_post(endpoint, data, "");
    free(endpoint);
    free(data);
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

int openTIDAL_DeleteFavoritePlaylist(const char *playlistid)
{
    int status;
    char buffer[80];
    snprintf(buffer, 80, "/v1/users/%zu/favorites/playlists/%s?countryCode=%s", config.userId,
playlistid, config.countryCode);

    curl_model req = curl_delete(buffer, "", "");
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
} */
