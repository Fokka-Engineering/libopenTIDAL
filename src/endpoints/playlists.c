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
#include "../include/parse.h"
#include "../include/handles.h"
#include "../include/openTIDAL.h"

openTIDAL_ContentContainer
openTIDAL_GetPlaylist(openTIDAL_SessionContainer *session, const char *playlistid)
{
    openTIDAL_ContentContainer o;
    openTIDAL_CurlContainer curl;
    char *endpoint = NULL;
    char baseparams[20];
 
    openTIDAL_StructInit(&o);
    openTIDAL_StructAlloc(&o, 3);

    endpoint = url_cat_str("/v1/playlists/", playlistid, "");
    snprintf(baseparams, 20, "countryCode=%s", session->countryCode);
    
    openTIDAL_CurlRequest(session, &curl, "GET", endpoint, baseparams, NULL, 0, 0); 
    if ( curl.status != -1 ) {
        cJSON *input_json = json_parse(curl.body);
        if ( curl.responseCode == 200 ) {
            openTIDAL_PlaylistContainer Value;

            json_playlist_model processed_json = json_parse_playlist(input_json);
            parse_playlist_values(&Value, &processed_json); 
            
            o.status = 1;
            openTIDAL_StructAddPlaylist(&o, Value);
        }
        else {
            o.status = parse_status(input_json, &curl, 0, playlistid);
        }
        
        o.json = input_json;
    }
    else
    {
        o.status = -1;
    }

    free(endpoint);
    openTIDAL_CurlRequestCleanup(&curl);
    return o;
}

openTIDAL_ContentContainer
openTIDAL_GetPlaylistItems(openTIDAL_SessionContainer *session, 
        const char *playlistid, const int limit, const int offset)
{
    openTIDAL_ContentContainer o;
    openTIDAL_CurlContainer curl;
    char *endpoint;
    char baseparams[50];
    
    openTIDAL_StructInit(&o);
    openTIDAL_StructAlloc(&o, 1);

    endpoint = url_cat_str("/v1/playlists/", playlistid, "/items");
    snprintf(baseparams, 50, "countryCode=%s&limit=%d&offset=%d", session->countryCode,
        limit, offset);

    openTIDAL_CurlRequest(session, &curl, "GET", endpoint, baseparams, NULL, 0, 0);
    if ( curl.status != -1 ) {
        cJSON *input_json = NULL;
        input_json = json_parse(curl.body);

        if ( curl.responseCode == 200 ) {
            cJSON *limit = NULL;
            cJSON *offset = NULL;
            cJSON *totalNumberOfItems = NULL;
            cJSON *items = NULL;
            cJSON *item = NULL;
            
            limit = cJSON_GetObjectItem(input_json, "limit");
            offset = cJSON_GetObjectItem(input_json, "offset");
            totalNumberOfItems = cJSON_GetObjectItem(input_json, "totalNumberOfItems");
            items = cJSON_GetObjectItem(input_json, "items");
                        
            if ( cJSON_IsArray(items) ) {
                cJSON_ArrayForEach( item, items ) {
                    openTIDAL_ItemsContainer Value;
                    cJSON *innerItem = cJSON_GetObjectItem(item, "item");
                    
                    json_items_model processed_json = json_parse_items(innerItem);
                    parse_items_values(&Value, &processed_json);
                    parse_signed_number(limit, &Value.limit); 
                    parse_signed_number(offset, &Value.offset);
                    parse_signed_number(totalNumberOfItems, &Value.totalNumberOfItems);

                    openTIDAL_StructAddItem(&o, Value);
                }
                o.status = 1;
            }
        }
        else {
            o.status = parse_status(input_json, &curl, 0, playlistid);
        }
        
        o.json = input_json; 
    }
    else {
        o.status = -1;
    }

    free(endpoint);
    openTIDAL_CurlRequestCleanup(&curl);
    return o;
}

openTIDAL_ContentContainer
openTIDAL_GetFavoritePlaylists(openTIDAL_SessionContainer *session, 
        const int limit, const int offset, const char *order, const char *orderDirection)
{
    openTIDAL_ContentContainer o;
    openTIDAL_CurlContainer curl;
    char *endpoint = NULL;
    char baseparams[150];
    
    openTIDAL_StructInit(&o);
    openTIDAL_StructAlloc(&o, 3);

    endpoint = url_cat("/v1/users/", session->userId, "/playlistsAndFavoritePlaylists", 0);
    snprintf(baseparams, 150, "countryCode=%s&limit=%d&offset=%d&order=%s&orderDirection=%s",
                         session->countryCode, limit, offset, order, orderDirection);

    openTIDAL_CurlRequest(session, &curl, "GET", endpoint, baseparams, NULL, 0, 0);
    if ( curl.status != -1 ) {
        cJSON *input_json = NULL;
        input_json = json_parse(curl.body);
        
        if ( curl.responseCode == 200 ) {
            cJSON *items = NULL;
            cJSON *item = NULL;
            cJSON *limit = NULL;
            cJSON *offset = NULL;
            cJSON *totalNumberOfItems = NULL;
            
            items = cJSON_GetObjectItem(input_json, "items");
            limit = cJSON_GetObjectItem(input_json, "limit");
            offset = cJSON_GetObjectItem(input_json, "offset");
            totalNumberOfItems = cJSON_GetObjectItem(input_json, "totalNumberOfItems");

            if ( cJSON_IsArray(items) ) {
                cJSON_ArrayForEach( item, items ) {
                    openTIDAL_PlaylistContainer playlist;
                    cJSON *innerItem = cJSON_GetObjectItem(item, "playlist");

                    json_playlist_model processed_json = json_parse_playlist(innerItem);
                    
                    parse_playlist_values(&playlist, &processed_json);
                    parse_signed_number(limit, &playlist.limit);
                    parse_signed_number(offset, &playlist.offset);
                    parse_signed_number(totalNumberOfItems, &playlist.totalNumberOfItems);

                    openTIDAL_StructAddPlaylist(&o, playlist);
                }
            }
            
            o.status = 1;
        }
        else {
            o.status = parse_status(input_json, &curl, session->userId, NULL);
        }
        
        o.json = input_json;
    }
    else {
        o.status = -1;
    }

    free(endpoint);
    openTIDAL_CurlRequestCleanup(&curl);
    return o;
}


openTIDAL_ETagContainer
openTIDAL_GetPlaylistETag(openTIDAL_SessionContainer *session,
        const char *playlistid)
{
    openTIDAL_ETagContainer Value;
    openTIDAL_CurlContainer curl;
    /* Request playlist endpoint to scrape eTag Header  */
    char *endpoint;
    char baseparams[20];

    endpoint = url_cat_str("/v1/playlists/", playlistid, "");
    snprintf(baseparams, 20, "countryCode=%s", session->countryCode);
    
    openTIDAL_CurlRequest(session, &curl, "HEAD", endpoint, baseparams, NULL, 0, 0);
    if ( curl.status != -1 ) {
        if ( curl.responseCode == 200 ) {
            size_t i = 0; /* Counter for Buffer (Header) Splitter  */
            size_t e; /* Counter for ETag-String Extraction  */
            char *p = strtok (curl.header, "\n");
            char eTag[32];
            /* Buffer for splitted HTTP-Header  */ 
            char *array[30];
            while (p != NULL)
            {
                array[i++] = p;
                /* Split if char is \n  */
                p = strtok (NULL, "\n");
            }
            for (e = 0; e < i; ++e)
            {
                /* If String begins with ETag  */
                if (strncmp(array[e], "ETag", 4) == 0)
                {
                    size_t charCounter;
                    size_t eTagCounter = 0;
                    /* Extract ETag Values */
                    for (charCounter = 7; charCounter < strlen(array[e]); ++charCounter)
                    {
                        if (array[e][charCounter] != '"')
                        {
                            strcpy(&eTag[eTagCounter], &array[e][charCounter]);
                            eTagCounter = eTagCounter + 1;
                        }
                    }
                    strcat(&eTag[eTagCounter + 1], "\0");
                }
            }
            /* copy eTag to struct */
            Value.id = eTag;
            Value.status = 1;
        }
        else
        {
            openTIDAL_ParseVerbose("Request Error", "Could not parse eTag-Header. Not a 200 Response", 1); 
            Value.status = 0;
        }
    }
    else
    {
        Value.status = -1;
    }

    openTIDAL_CurlRequestCleanup(&curl);
    free(endpoint);
    return Value;
}

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
    snprintf(buffer, 100, "/v1/playlists/%s/items/%d?countryCode=%s", playlistid, index, config.countryCode);
    curl_model req = curl_delete(buffer, "", etag_buffer);
    
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
    snprintf(buffer, 100, "/v1/playlists/%s/items/%d?countryCode=%s", playlistid, index, config.countryCode);
    
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
    snprintf(index_buffer, 100, "trackIds=%d&onArtifactNotFound=%s&onDupes=%s", trackid, "FAIL", onDupes);
    snprintf(buffer, 100, "/v1/playlists/%s/items?countryCode=%s", playlistid, config.countryCode);
     
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

int openTIDAL_AddPlaylistItems(const char *playlistid, const char *trackids, const char *onDupes)
{
    char buffer[100];
    char etag_buffer[50];
    char index_buffer[1000];

    openTIDAL_ETagModel etag = openTIDAL_GetPlaylistETag(playlistid);
    
    snprintf(etag_buffer, 50, "if-none-match: %s", etag.id);
    snprintf(index_buffer, 1000, "trackIds=%s&onArtifactNotFound=%s&onDupes=%s", trackids, "FAIL", onDupes);
    snprintf(buffer, 100, "/v1/playlists/%s/items?countryCode=%s", playlistid, config.countryCode);
    
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

/*openTIDAL_ContentContainer openTIDAL_CreatePlaylist(const char *title, const char *description)
{
    openTIDAL_ContentContainer o;
    char *endpoint = url_cat("/v1/users/", config.userId, "/playlists", 1);
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
        cJSON *input_json = json_parse(req.body);
        if (req.responseCode == 201 || req.responseCode == 200)
        {
            openTIDAL_PlaylistModel playlist;

            json_playlist_model processed_json = json_parse_playlist(input_json);
            parse_playlist_values(&playlist, &processed_json);
            o.status = 1;
            openTIDAL_StructAddPlaylist(&o, playlist);
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


int openTIDAL_AddFavoritePlaylist(const char *playlistid)
{
    char *endpoint = url_cat("/v1/users/", config.userId, "/favorites/playlists", 1);
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
    snprintf(buffer, 80, "/v1/users/%zu/favorites/playlists/%s?countryCode=%s", config.userId, playlistid, config.countryCode);

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
