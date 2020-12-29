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
#include <time.h>
#include <unistd.h>

#include "include/handles.h"
#include "include/openTIDAL.h"
#include "include/parse.h"

static void openTIDAL_SessionInitContainer (openTIDAL_SessionContainer *session);
static const char *openTIDAL_SessionCreateFileStream (openTIDAL_SessionContainer *session);
static const int openTIDAL_SessionScanFile (openTIDAL_SessionContainer *session);
static void openTIDAL_SessionReadFileStream (openTIDAL_SessionContainer *session,
                                             cJSON *input_json);

const int
openTIDAL_SessionInit (openTIDAL_SessionContainer *session, const char *file_location)
{
    int status = 0;
    openTIDAL_SessionInitContainer (session);

    /* Scan persistent config file and exit demo-mode */
    if (file_location) {
        session->location = (char *)file_location;
        status = openTIDAL_SessionScanFile (session);
        session->demoEnabled = 0;
    }
    openTIDAL_ParseVerbose ("Session", "Initialised openTIDAL", 2);
    return status;
}

void
openTIDAL_SessionCleanup (openTIDAL_SessionContainer *session)
{

    free (session->newStream);
    cJSON_Delete ((cJSON *)session->refreshRequest);
    cJSON_Delete ((cJSON *)session->tokenRequest);
    cJSON_Delete ((cJSON *)session->stream);

    openTIDAL_CurlCleanup (session);
    openTIDAL_ParseVerbose ("Session", "Deallocated session", 2);
}

void
openTIDAL_SessionCreateFile (openTIDAL_SessionContainer *session)
{
    FILE *fp = NULL;
    fp = fopen (session->location, "w");
    if (fp != NULL) {
        const char *json = NULL;
        json = openTIDAL_SessionCreateFileStream (session);
        fprintf (fp, "%s", json);
    }
    else {
        openTIDAL_ParseVerbose (
            "Session", "Failed to create persistent config: Could not write to file location", 1);
    }
    fclose (fp);
}

/* Initialise session structure */
static void
openTIDAL_SessionInitContainer (openTIDAL_SessionContainer *session)
{
    session->location = NULL;
    session->newStream = NULL;
    session->stream = NULL;
    session->refreshRequest = NULL;
    session->tokenRequest = NULL;
    session->demoEnabled = 1;
    session->curlHandle = NULL;

    session->clientId = "8SEZWa4J1NVC5U5Y";
    session->clientSecret = "owUYDkxddz+9FpvGX24DlxECNtFEMBxipU0lBfrbq60=";
    session->scopes = "r_usr+w_usr";
    session->baseUrl = "https://api.tidal.com";
    session->authUrl = "https://auth.tidal.com";

    session->username = NULL;
    session->userId = 0;
    session->expiresIn = 0;
    session->countryCode = "US";

    session->audioQuality = "LOW";
    session->videoQuality = "LOW";

    openTIDAL_ParseVerbose ("Session", "Initialise session structure", 2);
}

static const char *
openTIDAL_SessionCreateFileStream (openTIDAL_SessionContainer *session)
{
    char *string = NULL;
    cJSON *authorization_json = NULL;
    cJSON *user_json = NULL;
    cJSON *preferences_json = NULL;
    cJSON *refresh_token_json = NULL;
    cJSON *access_token_json = NULL;
    cJSON *expires_in_json = NULL;
    cJSON *id_json = NULL;
    cJSON *username_json = NULL;
    cJSON *country_code_json = NULL;
    cJSON *audio_quality_json = NULL;
    cJSON *video_quality_json = NULL;
    cJSON *output_json = NULL;

    output_json = cJSON_CreateObject ();

    authorization_json = cJSON_CreateObject ();
    cJSON_AddItemToObject (output_json, "authorization", authorization_json);
    user_json = cJSON_CreateObject ();
    cJSON_AddItemToObject (output_json, "user", user_json);
    preferences_json = cJSON_CreateObject ();
    cJSON_AddItemToObject (output_json, "preferences", preferences_json);

    refresh_token_json = cJSON_CreateString (session->refreshToken);
    cJSON_AddItemToObject (authorization_json, "refresh_token", refresh_token_json);
    access_token_json = cJSON_CreateString (session->accessToken);
    cJSON_AddItemToObject (authorization_json, "access_token", access_token_json);
    expires_in_json = cJSON_CreateNumber (session->expiresIn);
    cJSON_AddItemToObject (authorization_json, "expires_in", expires_in_json);

    id_json = cJSON_CreateNumber (session->userId);
    cJSON_AddItemToObject (user_json, "id", id_json);
    username_json = cJSON_CreateString (session->username);
    cJSON_AddItemToObject (user_json, "username", username_json);
    country_code_json = cJSON_CreateString (session->countryCode);
    cJSON_AddItemToObject (user_json, "country_code", country_code_json);

    audio_quality_json = cJSON_CreateString (session->audioQuality);
    cJSON_AddItemToObject (preferences_json, "audio_quality", audio_quality_json);
    video_quality_json = cJSON_CreateString (session->videoQuality);
    cJSON_AddItemToObject (preferences_json, "video_quality", video_quality_json);

    string = cJSON_Print (output_json); /* Allocate Memory (needs to be deallocated)  */
    session->newStream = string;
    cJSON_Delete (output_json);

    openTIDAL_ParseVerbose ("Session", "Created new json stream", 2);
    return string;
}

static void
openTIDAL_SessionReadFileStream (openTIDAL_SessionContainer *session, cJSON *input_json)
{
    cJSON *authorization = NULL;
    cJSON *refreshToken = NULL;
    cJSON *accessToken = NULL;
    cJSON *expiresIn = NULL;

    cJSON *user = NULL;
    cJSON *id = NULL;
    cJSON *countryCode = NULL;

    cJSON *preferences = NULL;
    cJSON *audioQuality = NULL;
    cJSON *videoQuality = NULL;

    authorization = cJSON_GetObjectItem (input_json, "authorization");
    refreshToken = cJSON_GetObjectItemCaseSensitive (authorization, "refresh_token");
    accessToken = cJSON_GetObjectItemCaseSensitive (authorization, "access_token");
    expiresIn = cJSON_GetObjectItem (authorization, "expires_in");

    user = cJSON_GetObjectItem (input_json, "user");
    id = cJSON_GetObjectItem (user, "id");
    countryCode = cJSON_GetObjectItemCaseSensitive (user, "country_code");

    preferences = cJSON_GetObjectItem (input_json, "preferences");
    audioQuality = cJSON_GetObjectItemCaseSensitive (preferences, "audio_quality");
    videoQuality = cJSON_GetObjectItemCaseSensitive (preferences, "video_quality");

    parse_string (refreshToken, &session->refreshToken);
    parse_string (accessToken, &session->accessToken);
    parse_number (expiresIn, (size_t *)&session->expiresIn);
    parse_number (id, &session->userId);
    parse_string (countryCode, &session->countryCode);
    parse_string (audioQuality, &session->audioQuality);
    parse_string (videoQuality, &session->videoQuality);
    openTIDAL_ParseVerbose ("Session", "Read & allocate persistent stream from file", 2);
}

static const int
openTIDAL_SessionScanFile (openTIDAL_SessionContainer *session)
{
    FILE *persistentJSON = NULL;
    long streamSize = 0;
    char *stream = NULL;
    int error = 0;

    /* open persistentFile  */
    persistentJSON = fopen (session->location, "rb");
    if (!persistentJSON) {
        openTIDAL_ParseVerbose ("Session", "File not found. Please authenticate", 1);
        error = 1;
        goto end;
    }

    fseek (persistentJSON, 0L, SEEK_END);
    streamSize = ftell (persistentJSON);
    rewind (persistentJSON);

    /* allocate memory for entire content */
    stream = calloc (1, streamSize + 1);
    if (!stream) {
        fclose (persistentJSON);
        openTIDAL_ParseVerbose ("Session", "Memory allocation of persistent config file failed", 1);
        error = 1;
        goto end;
    }

    /* copy the file into the buffer */
    if (1 != fread (stream, streamSize, 1, persistentJSON)) {
        fclose (persistentJSON);
        free (stream);
        fprintf (stderr, "entire read fails");
        openTIDAL_ParseVerbose (
            "Session", "Copying persistent config into buffer failed. Entire read fails", 1);
        error = 1;
        goto end;
    }
    fclose (persistentJSON);

    /* parse JSON stream    */
    cJSON *input_json = NULL;
    input_json = openTIDAL_cJSONParseHelper (stream);
    if (!input_json) {
        error = 1;
        goto end;
    }

    openTIDAL_SessionReadFileStream (session, input_json);
    session->stream = input_json;

end:
    if (error == 1) {
        openTIDAL_ParseVerbose ("Session", "Scanning persistent config failed", 1);
        return 0;
    }
    return 1;
}

void
openTIDAL_SessionRefresh (openTIDAL_SessionContainer *session)
{
    /* start OAuth refresh routine  */
    time_t currentTime = time (NULL);
    size_t skip = 0;
    double diff_t = 0;
    if (session->demoEnabled) {
        goto end;
    }
    /* Check if ExpiryDate is in the future  */
    if (currentTime > session->expiresIn) {
        diff_t = difftime (currentTime, session->expiresIn);
    }
    else {
        diff_t = difftime (session->expiresIn, currentTime);
    }

    char buffer[50];
    snprintf (buffer, sizeof (buffer), "Difference timestamp and currentTime: %zu", (size_t)diff_t);
    openTIDAL_ParseVerbose ("Session", buffer, 2);

    /* If ExpiryDate is in the future with a difference of more than 5min  */
    if (currentTime < session->expiresIn && (size_t)diff_t >= 300) {
        openTIDAL_ParseVerbose ("Session", "AccessToken renewal not necessary", 2);
    }
    /* Start renewal process    */
    else {
        openTIDAL_ParseVerbose ("Session", "Start AccessToken renewal process", 2);
        openTIDAL_ContentContainer res
            = openTIDAL_AuthRefreshBearerToken (session, session->refreshToken);
        if (res.status == 1) {
            FILE *fp = NULL;
            session->accessToken = res.token.access_token;
            session->expiresIn = time (NULL) + 604800; /* Calculate new ExpiryDate */

            fp = fopen (session->location, "w");
            if (fp != NULL) {
                const char *json = NULL;
                json = openTIDAL_SessionCreateFileStream (session);
                fprintf (fp, "%s", json);

                openTIDAL_ParseVerbose ("Session", "AccessToken renewal successful", 2);
            }
            else {
                openTIDAL_ParseVerbose (
                    "Session",
                    "AccessToken renewal partially failed. Failed to update persistent config", 1);
            }
            fclose (fp);
            cJSON_Delete ((cJSON *)session->refreshRequest);
            session->refreshRequest = res.json;
        }
        else {
            openTIDAL_ParseVerbose ("Session", "AccessToken refresh failed. Switching to demo-mode",
                                    1);
            session->demoEnabled = 1;
        }
    }
end:
    if (skip) {
        // fprintf(stderr, "[OAuth] Auto Refresh skipped.\n");
    }
}
