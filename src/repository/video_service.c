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
openTIDAL_GetVideo (openTIDAL_SessionContainer *session, const char *videoId)
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

    openTIDAL_StringHelper (&curl.endpoint, "/v1/videos/%s", videoId);
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
            struct openTIDAL_ItemsContainer video;

            struct openTIDAL_JsonItemsContainer processed_json
                = openTIDAL_ParseJsonItems ((cJSON *)o->json);
            openTIDAL_ParseJsonItemsValues (&video, &processed_json);

            o->status = 1;
            status = openTIDAL_StructAddItem (o, video);
        }
        else {
            o->status = openTIDAL_ParseStatus ((cJSON *)o->json, &curl, videoId);
        }
    }
end:
    if (status == -1) o->status = -14;
    openTIDAL_CurlRequestCleanup (&curl);
    return o;
}

openTIDAL_ContentContainer *
openTIDAL_GetVideoStream (openTIDAL_SessionContainer *session, const char *videoId)
{
    openTIDAL_ContentContainer *o = NULL;
    openTIDAL_CurlContainer curl;
    int status = 0;

    openTIDAL_CurlModelInit (&curl);

    status = openTIDAL_StructMainAlloc (&o);
    if (status == -1) return NULL;
    status = openTIDAL_StructInit (o);
    if (status == -1) goto end;
    status = openTIDAL_StructOneTimeAlloc (o, -5);
    if (status == -1) goto end;

    openTIDAL_StringHelper (&curl.endpoint, "/v1/videos/%s/playbackinfopostpaywall", videoId);
    openTIDAL_StringHelper (&curl.parameter,
                            "countryCode=%s&videoquality=%s&assetpresentation=%s&playbackmode=%s",
                            session->countryCode, session->videoQuality, "FULL", "STREAM");
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
            struct openTIDAL_JsonStreamContainer processed_json
                = openTIDAL_ParseJsonStream ((cJSON *)o->json);
            openTIDAL_ParseJsonStreamValues (o->stream, &processed_json);
            o->status = 0;

            if (strcmp (o->stream->manifestMimeType, "application/vnd.tidal.emu") == 0) {
                struct openTIDAL_JsonManifestContainer processed_manifest;
                char *manifest_decoded = NULL;

                openTIDAL_VerboseHelper ("Base64", "Allocated decoded data in heap", 2);
                manifest_decoded = malloc (openTIDAL_Base64DecodeLen (o->stream->manifest));
                if (!manifest_decoded) {
                    status = -1;
                    goto end;
                }

                openTIDAL_Base64Decode (manifest_decoded, o->stream->manifest);
                o->jsonManifest = openTIDAL_cJSONParseHelper (manifest_decoded);

                o->status = 1;
                o->stream->codec = NULL;
                o->stream->encryptionType = NULL;
                o->stream->audioMode = NULL;
                o->stream->audioQuality = NULL;

                processed_manifest = openTIDAL_ParseJsonManifest ((cJSON *)o->jsonManifest);
                openTIDAL_ParseJsonString (processed_manifest.mimeType, &o->stream->mimeType);
                openTIDAL_ParseJsonString (processed_manifest.url, &o->stream->url);

                free (manifest_decoded);
            }
            else {
                o->status = -10;
                openTIDAL_VerboseHelper (
                    "Request Error",
                    "Not a valid manifest. MimeType is not application/vnd.tidal.emu", 1);
            }
        }
        else {
            o->status = openTIDAL_ParseStatus ((cJSON *)o->json, &curl, videoId);
        }
    }
end:
    if (status == -1) o->status = -14;
    openTIDAL_CurlRequestCleanup (&curl);
    return o;
}
