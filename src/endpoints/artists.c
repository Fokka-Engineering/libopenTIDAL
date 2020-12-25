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
#include "../include/parse.h"
#include "../include/handles.h"
#include "../include/openTIDAL.h"

openTIDAL_ContentContainer
openTIDAL_GetArtist(openTIDAL_SessionContainer *session, const size_t artistid)
{
    openTIDAL_ContentContainer o; 
    openTIDAL_CurlContainer curl;
    char *endpoint = NULL;
    char baseparams[20];
 
    openTIDAL_StructInit(&o);
    openTIDAL_StructAlloc(&o, 2);

    endpoint = url_cat("/v1/artists/", artistid, "", 0);
    snprintf(baseparams, 20, "countryCode=%s", session->countryCode);
    
    openTIDAL_CurlRequest(session, &curl, "GET", endpoint, baseparams, NULL, 0, 0);
    free(endpoint);
    if ( curl.status != -1 ) {
        cJSON *input_json = NULL;
        input_json = json_parse(curl.body);
        
        if ( curl.responseCode == 200 ) {
            openTIDAL_ArtistContainer artist;
            json_artist_model processed_json = json_parse_artist(input_json);
            parse_artist_values(&artist, &processed_json);
            
            o.status = 1;
            openTIDAL_StructAddArtist(&o, artist);
        }
        else {
            o.status = parse_status(input_json, &curl, artistid, NULL);
        }

        o.json = input_json;
    }
    else {
        o.status = -1;
    }

    openTIDAL_CurlRequestCleanup(&curl);
    return o;
}

openTIDAL_ContentContainer
openTIDAL_GetArtistLink(openTIDAL_SessionContainer *session, 
        const size_t artistid, const int limit, const int offset)
{
    openTIDAL_ContentContainer o;
    openTIDAL_CurlContainer curl;
    char *endpoint;
    char baseparams[50];
 
    openTIDAL_StructInit(&o);
    openTIDAL_StructAlloc(&o, 7); 

    endpoint = url_cat("/v1/artists/", artistid, "/links", 0);
    snprintf(baseparams, 50, "countryCode=%s&limit=%d&offset=%d", session->countryCode,
        limit, offset);
    
    openTIDAL_CurlRequest(session, &curl, "GET", endpoint, baseparams, NULL, 0, 0);
    free(endpoint);
    if ( curl.status != -1 ) {
        cJSON *input_json = NULL;
        input_json = json_parse(curl.body);
        
        if ( curl.responseCode == 200 ) {
            cJSON *limit = NULL;
            cJSON *offset = NULL;
            cJSON *totalNumberOfItems = NULL;
            cJSON *source = NULL;
            cJSON *items = NULL;
            cJSON *item = NULL;
            
            limit = cJSON_GetObjectItem(input_json, "limit");
            offset = cJSON_GetObjectItem(input_json, "offset");
            totalNumberOfItems = cJSON_GetObjectItem(input_json, "totalNumberOfItems");
            source = cJSON_GetObjectItemCaseSensitive(input_json, "source");
            items = cJSON_GetObjectItem(input_json, "items");
            
            if ( cJSON_IsArray(items) ) {
                cJSON_ArrayForEach( item, items ) {
                    openTIDAL_LinkContainer Value;
                    
                    json_links_model processed_json = json_parse_links(item);
                    parse_link_values(&Value, &processed_json);
                    parse_signed_number(limit, &Value.limit);
                    parse_signed_number(offset, &Value.offset);
                    parse_signed_number(totalNumberOfItems, &Value.totalNumberOfItems);
                    parse_string(source, &Value.source);

                    openTIDAL_StructAddLink(&o, Value);
                }
            }
            o.status = 1;
        }
        else {
            o.status = parse_status(input_json, &curl, artistid, NULL);
        }

        o.json = input_json;
    }
    else {
        o.status = -1;
    }

    openTIDAL_CurlRequestCleanup(&curl);
    return o;
}

openTIDAL_ContentContainer
openTIDAL_GetArtistMix(openTIDAL_SessionContainer *session, const size_t artistid)
{
    openTIDAL_ContentContainer o;
    openTIDAL_CurlContainer curl;
    char *endpoint = NULL;
    char baseparams[20];

    openTIDAL_StructInit(&o);
    openTIDAL_StructAlloc(&o, 4);

    endpoint = url_cat("/v1/artists/", artistid, "/mix", 0);
    snprintf(baseparams, 20, "countryCode=%s", session->countryCode);
    
    openTIDAL_CurlRequest(session, &curl, "GET", endpoint, baseparams, NULL, 0, 0);
    free(endpoint);
    if ( curl.status != -1 ) {
        cJSON *input_json = NULL;
        input_json = json_parse(curl.body);

        if ( curl.responseCode == 200 ) {
            openTIDAL_MixContainer Value;
            
            json_mix_model processed_json = json_parse_mix(input_json);
            parse_mix_values(&Value, &processed_json);
            o.status = 1;
            openTIDAL_StructAddMix(&o, Value);
        }
        else {
            o.status = parse_status(input_json, &curl, artistid, NULL);
        }

        o.json = input_json;
    }
    else {
        o.status = -1;
    }

    openTIDAL_CurlRequestCleanup(&curl);
    return o;
}

openTIDAL_ContentContainer
openTIDAL_GetArtistTopTracks(openTIDAL_SessionContainer *session,const size_t artistid,
        const int limit, const int offset)
{
    openTIDAL_ContentContainer o;
    openTIDAL_CurlContainer curl;
    char *endpoint;
    char baseparams[50];
 
    openTIDAL_StructInit(&o);
    openTIDAL_StructAlloc(&o, 1);

    endpoint = url_cat("/v1/artists/", artistid, "/toptracks", 0);
    snprintf(baseparams, 50, "countryCode=%s&limit=%d&offset=%d", session->countryCode,
                        limit, offset);
    
    openTIDAL_CurlRequest(session, &curl, "GET", endpoint, baseparams, NULL, 0, 0);
    free(endpoint);
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

            o.status = 0;

            if ( cJSON_IsArray(items) ) {
                cJSON_ArrayForEach( item, items ) {
                    openTIDAL_ItemsContainer Value;
                    json_items_model processed_json = json_parse_items(item);
        
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
            o.status = parse_status(input_json, &curl, artistid, NULL);
        }

        o.json = input_json;
    }
    else {
        o.status = -1;
    }

    openTIDAL_CurlRequestCleanup(&curl);
    return o;
}

openTIDAL_ContentContainer
openTIDAL_GetArtistVideos(openTIDAL_SessionContainer *session, const size_t artistid,
        const int limit, const int offset)
{
    openTIDAL_ContentContainer o;
    openTIDAL_CurlContainer curl;
    char *endpoint = NULL;
    char baseparams[50];
 
    openTIDAL_StructInit(&o);
    openTIDAL_StructAlloc(&o, 1);

    endpoint = url_cat("/v1/artists/", artistid, "/videos", 0);
    snprintf(baseparams, 50, "countryCode=%s&limit=%d&offset=%d", session->countryCode,
                        limit, offset);
    
    openTIDAL_CurlRequest(session, &curl, "GET", endpoint, baseparams, NULL, 0, 0);
    free(endpoint);
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
                    
                    json_items_model processed_json = json_parse_items(item);
                    parse_items_values(&Value, &processed_json);
                    parse_signed_number(limit, &Value.limit); 
                    parse_signed_number(offset, &Value.offset);
                    parse_signed_number(totalNumberOfItems, &Value.totalNumberOfItems);

                    openTIDAL_StructAddItem(&o, Value);
                }
            }
            
            o.status = 1;
        }
        else {
            o.status = parse_status(input_json, &curl, artistid, NULL);
        }

        o.json = input_json;
    }
    else {
        o.status = -1;
    }

    openTIDAL_CurlRequestCleanup(&curl);
    return o;
}

openTIDAL_ContentContainer
openTIDAL_GetArtistAlbums(openTIDAL_SessionContainer *session, const size_t artistid,
        const int limit, const int offset)
{
    openTIDAL_ContentContainer o;
    openTIDAL_CurlContainer curl;
    char *endpoint;
    char baseparams[50];
 
    openTIDAL_StructInit(&o);
    openTIDAL_StructAlloc(&o, 0);

    endpoint = url_cat("/v1/artists/", artistid, "/albums", 0);
    snprintf(baseparams, 50, "countryCode=%s&limit=%d&offset=%d", session->countryCode,
                        limit, offset);
    
    openTIDAL_CurlRequest(session, &curl, "GET", endpoint, baseparams, NULL, 0, 0);
    free(endpoint);
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
                    openTIDAL_AlbumContainer Value;
                    
                    json_album_model processed_json = json_parse_album(item);
                    parse_album_values(&Value, &processed_json);
                    parse_signed_number(limit, &Value.limit); 
                    parse_signed_number(offset, &Value.offset);
                    parse_signed_number(totalNumberOfItems, &Value.totalNumberOfItems);
                    
                    openTIDAL_StructAddAlbum(&o, Value);
                }
            }
            
            o.status = 1;
        }
        else {
            o.status = parse_status(input_json, &curl, artistid, NULL);
        }

        o.json = input_json;
    }
    else {
        o.status = -1;
    }

    openTIDAL_CurlRequestCleanup(&curl);
    return o;
}

openTIDAL_ContentContainer
openTIDAL_GetFavoriteArtists(openTIDAL_SessionContainer *session, const int limit,
        const int offset, const char *order, const char *orderDirection)
{
    openTIDAL_ContentContainer o;
    openTIDAL_CurlContainer curl;
    char *endpoint = NULL;
    char baseparams[150];
    
    openTIDAL_StructInit(&o);
    openTIDAL_StructAlloc(&o, 2);

    endpoint = url_cat("/v1/users/", session->userId, "/favorites/artists", 0);
    snprintf(baseparams, 150, "countryCode=%s&limit=%d&offset=%d&order=%s&orderDirection=%s",
                         session->countryCode, limit, offset, order, orderDirection);

    openTIDAL_CurlRequest(session, &curl, "GET", endpoint, baseparams, NULL, 0, 0);
    free(endpoint);
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
                    openTIDAL_ArtistContainer artist;
                    cJSON *innerItem = NULL;
                    
                    innerItem = cJSON_GetObjectItem(item, "item");
                    
                    json_artist_model processed_json = json_parse_artist(innerItem);
                    parse_artist_values(&artist, &processed_json);
                    parse_signed_number(limit, &artist.limit);
                    parse_signed_number(offset, &artist.offset);
                    parse_signed_number(totalNumberOfItems, &artist.totalNumberOfItems);

                    openTIDAL_StructAddArtist(&o, artist);
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

    openTIDAL_CurlRequestCleanup(&curl);
    return o;
}

/* create & delete favourites */

int openTIDAL_AddFavoriteArtist(openTIDAL_SessionContainer *session, const size_t artistid)
{
    openTIDAL_CurlContainer curl;
    int status;
    char buffer[60];
    char *endpoint = NULL;

    endpoint = url_cat("/v1/users/", session->userId, "/favorites/artists", 1);
    snprintf(buffer, 60, "artistIds=%zu&onArtifactNotFound=FAIL", artistid);

    openTIDAL_CurlRequest(session, &curl, "POST", endpoint, NULL, buffer, 0, 1);
    free(endpoint);
    if ( curl.status != -1 ) {
        status = parse_raw_status(&curl.responseCode);
    }
    else {
        status = -1;
    }

    openTIDAL_CurlRequestCleanup(&curl);
    return status;
}

int openTIDAL_DeleteFavoriteArtist(openTIDAL_SessionContainer *session, const size_t artistid)
{
    openTIDAL_CurlContainer curl;
    int status;
    char buffer[80];

    snprintf(buffer, 80, "/v1/users/%zu/favorites/artists/%zu?countryCode=%s",
            session->userId, artistid, session->countryCode);

    openTIDAL_CurlRequest(session, &curl, "DELETE", buffer, NULL, NULL, 0, 1);
    if ( curl.status != -1 ) {
        status = parse_raw_status(&curl.responseCode);
    }
    else {
        status = -1;
    }

    openTIDAL_CurlRequestCleanup(&curl);
    return status;
}

