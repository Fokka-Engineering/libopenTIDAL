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

openTIDAL_ContentContainer *
openTIDAL_GetArtist (openTIDAL_SessionContainer *session, const char *artistId)
{
    openTIDAL_ContentContainer *o = NULL;
    openTIDAL_CurlContainer curl;
    int status = 0;

    openTIDAL_CurlModelInit (&curl);
    status = openTIDAL_StructMainAlloc (&o);
    if (status == -1) return NULL;
    status = openTIDAL_StructInit (o);
    if (status == -1) goto end;
    status = openTIDAL_StructAlloc (o, 2);
    if (status == -1) goto end;

    openTIDAL_StringHelper (&curl.endpoint, "/v1/artists/%s", artistId);
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
            struct openTIDAL_ArtistContainer artist;
            struct openTIDAL_JsonArtistContainer processed_json
                = openTIDAL_ParseJsonArtist ((cJSON *)o->json);
            openTIDAL_ParseJsonArtistValues (&artist, &processed_json);

            o->status = 1;
            status = openTIDAL_StructAddArtist (o, artist);
        }
        else {
            o->status = openTIDAL_ParseStatus ((cJSON *)o->json, &curl, artistId);
        }
    }
end:
    if (status == -1) o->status = -14;
    openTIDAL_CurlRequestCleanup (&curl);
    return o;
}

openTIDAL_ContentContainer *
openTIDAL_GetArtistLink (openTIDAL_SessionContainer *session, const char *artistId, int limit,
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
    status = openTIDAL_StructAlloc (o, 7);
    if (status == -1) goto end;

    openTIDAL_StringHelper (&curl.endpoint, "/v1/artists/%s/links", artistId);
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
            cJSON *source = NULL;
            cJSON *items = NULL;
            cJSON *item = NULL;

            limit = cJSON_GetObjectItem ((cJSON *)o->json, "limit");
            offset = cJSON_GetObjectItem ((cJSON *)o->json, "offset");
            totalNumberOfItems = cJSON_GetObjectItem ((cJSON *)o->json, "totalNumberOfItems");
            source = cJSON_GetObjectItemCaseSensitive ((cJSON *)o->json, "source");
            items = cJSON_GetObjectItem ((cJSON *)o->json, "items");

            if (cJSON_IsArray (items)) {
                cJSON_ArrayForEach (item, items)
                {
                    struct openTIDAL_LinkContainer Value;

                    struct openTIDAL_JsonLinkContainer processed_json
                        = openTIDAL_ParseJsonLinks (item);
                    openTIDAL_ParseJsonLinkValues (&Value, &processed_json);
                    openTIDAL_ParseJsonSignedNumber (limit, &Value.limit);
                    openTIDAL_ParseJsonSignedNumber (offset, &Value.offset);
                    openTIDAL_ParseJsonSignedNumber (totalNumberOfItems, &Value.totalNumberOfItems);
                    openTIDAL_ParseJsonString (source, &Value.source);

                    status = openTIDAL_StructAddLink (o, Value);
                    if (status == -1) goto end;
                }
                o->status = 1;
            }
        }
        else {
            o->status = openTIDAL_ParseStatus ((cJSON *)o->json, &curl, artistId);
        }
    }
end:
    if (status == -1) o->status = -14;
    openTIDAL_CurlRequestCleanup (&curl);
    return o;
}

openTIDAL_ContentContainer *
openTIDAL_GetArtistMix (openTIDAL_SessionContainer *session, const char *artistId)
{
    openTIDAL_ContentContainer *o = NULL;
    openTIDAL_CurlContainer curl;
    int status = 0;

    openTIDAL_CurlModelInit (&curl);
    status = openTIDAL_StructMainAlloc (&o);
    if (status == -1) return NULL;
    status = openTIDAL_StructInit (o);
    if (status == -1) goto end;
    status = openTIDAL_StructAlloc (o, 4);
    if (status == -1) goto end;

    openTIDAL_StringHelper (&curl.endpoint, "/v1/artists/%s/mix", artistId);
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
            struct openTIDAL_MixContainer Value;

            struct openTIDAL_JsonMixContainer processed_json
                = openTIDAL_ParseJsonMix ((cJSON *)o->json);
            openTIDAL_ParseJsonMixValues (&Value, &processed_json);
            o->status = 1;
            status = openTIDAL_StructAddMix (o, Value);
        }
        else {
            o->status = openTIDAL_ParseStatus ((cJSON *)o->json, &curl, artistId);
        }
    }
end:
    if (status == -1) o->status = -14;
    openTIDAL_CurlRequestCleanup (&curl);
    return o;
}

openTIDAL_ContentContainer *
openTIDAL_GetArtistTopTracks (openTIDAL_SessionContainer *session, const char *artistId, int limit,
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

    openTIDAL_StringHelper (&curl.endpoint, "/v1/artists/%s/toptracks", artistId);
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

            o->status = 0;

            if (cJSON_IsArray (items)) {
                cJSON_ArrayForEach (item, items)
                {
                    struct openTIDAL_ItemsContainer Value;
                    struct openTIDAL_JsonItemsContainer processed_json
                        = openTIDAL_ParseJsonItems (item);

                    status = openTIDAL_ParseJsonItemsValues (&Value, &processed_json);
                    if (status == -1) goto end;
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
            o->status = openTIDAL_ParseStatus ((cJSON *)o->json, &curl, artistId);
        }
    }
end:
    if (status == -1) o->status = -14;
    openTIDAL_CurlRequestCleanup (&curl);
    return o;
}

openTIDAL_ContentContainer *
openTIDAL_GetArtistVideos (openTIDAL_SessionContainer *session, const char *artistId, int limit,
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

    openTIDAL_StringHelper (&curl.endpoint, "/v1/artists/%s/videos", artistId);
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

                    struct openTIDAL_JsonItemsContainer processed_json
                        = openTIDAL_ParseJsonItems (item);
                    status = openTIDAL_ParseJsonItemsValues (&Value, &processed_json);
                    if (status == -1) goto end;
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
            o->status = openTIDAL_ParseStatus ((cJSON *)o->json, &curl, artistId);
        }
    }
end:
    if (status == -1) o->status = -14;
    openTIDAL_CurlRequestCleanup (&curl);
    return o;
}

openTIDAL_ContentContainer *
openTIDAL_GetArtistAlbums (openTIDAL_SessionContainer *session, const char *artistId, int limit,
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
    status = openTIDAL_StructAlloc (o, 0);
    if (status == -1) goto end;

    openTIDAL_StringHelper (&curl.endpoint, "/v1/artists/%s/albums", artistId);
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
                    struct openTIDAL_AlbumContainer Value;

                    struct openTIDAL_JsonAlbumContainer processed_json
                        = openTIDAL_ParseJsonAlbum (item);
                    status = openTIDAL_ParseJsonAlbumValues (&Value, &processed_json);
                    if (status == -1) goto end;
                    openTIDAL_ParseJsonSignedNumber (limit, &Value.limit);
                    openTIDAL_ParseJsonSignedNumber (offset, &Value.offset);
                    openTIDAL_ParseJsonSignedNumber (totalNumberOfItems, &Value.totalNumberOfItems);

                    status = openTIDAL_StructAddAlbum (o, Value);
                    if (status == -1) goto end;
                }
                o->status = 1;
            }
        }
        else {
            o->status = openTIDAL_ParseStatus ((cJSON *)o->json, &curl, artistId);
        }

        o->json = (cJSON *)o->json;
    }
end:
    if (status == -1) o->status = -14;
    openTIDAL_CurlRequestCleanup (&curl);
    return o;
}

