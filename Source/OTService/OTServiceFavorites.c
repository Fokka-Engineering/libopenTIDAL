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

/* openTIDAL user favorites manipulation service
 */

#include <stdlib.h>
#include <string.h>

#include "../OTHelper.h"
#include "../OTHttp.h"
#include "../openTIDAL.h"
#include "OTService.h"

static const char *OTServiceFavoriteIdTypes[6]
    = { "albumIds", "trackIds", "videoIds", "artistIds", "uuids", "mixIds" };

static const char *
OTServiceFavoriteType (const char *const suffix)
{
    const char *selection = NULL;
    if (strcmp (suffix, "albums") == 0)
        selection = OTServiceFavoriteIdTypes[0];
    else if (strcmp (suffix, "tracks") == 0)
        selection = OTServiceFavoriteIdTypes[1];
    else if (strcmp (suffix, "videos") == 0)
        selection = OTServiceFavoriteIdTypes[2];
    else if (strcmp (suffix, "artists") == 0)
        selection = OTServiceFavoriteIdTypes[3];
    else if (strcmp (suffix, "playlists") == 0)
        selection = OTServiceFavoriteIdTypes[4];
    else if (strcmp (suffix, "mixes") == 0)
        selection = OTServiceFavoriteIdTypes[5];
    return selection;
}

struct OTContentContainer *
OTServiceGetFavorites (struct OTSessionContainer *session, const char *const suffix,
                       const int limit, const int offset, const char *const order,
                       const char *const orderDirection, void *threadHandle)
{
    int isException = 0;
    struct OTHttpContainer http;
    struct OTContentContainer *content = NULL;
    enum OTHttpTypes reqType = GET;

    /* Initialise values in structure. */
    OTHttpContainerInit (&http);

    if (session->restrictedMode)
        {
            isException = 1;
            goto end;
        }

    http.type = &reqType;

    /* Favorite mixes are in v2. */
    if (strcmp (suffix, "mixes") == 0)
        OTConcatenateString (&http.endpoint, "/v2/favorites/%s", suffix);
    else if (strcmp (suffix, "playlists") == 0)
        OTConcatenateString (&http.endpoint, "/v1/users/%s/favorites/playlistsAndFavoritePlaylists",
                             session->userId);
    else
        OTConcatenateString (&http.endpoint, "/v1/users/%s/favorites/%s", session->userId, suffix);
    OTConcatenateString (&http.parameter,
                         "countryCode=%s&limit=%d&offset=%d&order=%s&orderDirection=%s&"
                         "locale=%s&deviceType=%s",
                         session->countryCode, limit, offset, order, orderDirection,
                         session->locale, session->deviceType);

    if (!http.parameter || !http.endpoint)
        {
            isException = 1;
            goto end;
        }

    content = OTServiceRequestStandard (session, &http, threadHandle);
end:
    free (http.endpoint);
    free (http.parameter);
    return content;
}

enum OTStatus
OTServiceDeleteFavorite (struct OTSessionContainer *session, const char *const suffix,
                         const char *const id, void *threadHandle)
{
    int isException = 0;
    struct OTHttpContainer http;
    enum OTHttpTypes reqType = DELETE;
    enum OTStatus status = UNKNOWN;

    /* Initialise values in structure. */
    OTHttpContainerInit (&http);

    if (session->restrictedMode)
        return status;

    http.type = &reqType;
    http.isDummy = 1;

    /* Use v2 if suffix is equal to "mixes". */
    if (!(strcmp (suffix, "mixes") == 0))
        {
            OTConcatenateString (&http.endpoint, "/v1/users/%s/favorites/%s/%s", session->userId,
                                 suffix, id);
            OTConcatenateString (&http.parameter, "countryCode=%s", session->countryCode);
        }
    else
        {
            reqType = PUT;
            OTConcatenateString (&http.endpoint, "/v2/favorites/%s/remove", suffix);
            OTConcatenateString (&http.parameter, "mixIds=%s&countryCode=%s", id,
                                 session->countryCode);
        }

    if (!http.parameter || !http.endpoint)
        {
            isException = 1;
            goto end;
        }

    status = OTServiceRequestSilent (session, &http, threadHandle);
end:
    free (http.endpoint);
    free (http.parameter);
    return status;
}

enum OTStatus
OTServiceAddFavorite (struct OTSessionContainer *session, const char *const suffix,
                      const char *itemId, const char *onArtifactNotFound, void *threadHandle)
{
    int isException = 0;
    struct OTHttpContainer http;
    const char *type;
    enum OTHttpTypes reqType = POST;
    enum OTStatus status = UNKNOWN;

    /* Initialise values in structure. */
    OTHttpContainerInit (&http);

    if (session->restrictedMode)
        return status;

    http.type = &reqType;
    http.isDummy = 1;

    /* Determine type of artefact to add. */
    type = OTServiceFavoriteType (suffix);
    if (!type)
        {
            isException = 1;
            goto end;
        }

    /* Use v2 if suffix is equal to "mixes". */
    if (!(strcmp (suffix, "mixes") == 0))
        OTConcatenateString (&http.endpoint, "/v1/users/%s/favorites/%s", session->userId, suffix);
    else
        {
            reqType = PUT;
            OTConcatenateString (&http.endpoint, "/v2/favorites/%s/add", suffix);
        }

    OTConcatenateString (&http.parameter, "countryCode=%s", session->countryCode);
    OTConcatenateString (&http.postData, "%s=%s&onArtifactNotFound=%s", type, itemId,
                         onArtifactNotFound);
    if (!http.parameter || !http.endpoint || !http.postData)
        {
            isException = 1;
            goto end;
        }

    status = OTServiceRequestSilent (session, &http, threadHandle);
end:
    free (http.endpoint);
    free (http.parameter);
    free (http.postData);
    return status;
}

enum OTStatus
OTServiceAddFavorites (struct OTSessionContainer *session, const char *const suffix,
                       const char **itemIds, const int size, const char *onArtifactNotFound,
                       void *threadHandle)
{
    int isException = 0;
    struct OTHttpContainer http;
    char *arrayString;
    const char *type;
    enum OTHttpTypes reqType = POST;
    enum OTStatus status = UNKNOWN;

    /* Initialise values in structure. */
    OTHttpContainerInit (&http);

    if (session->restrictedMode)
        return status;

    http.type = &reqType;
    http.isDummy = 1;

    OTArrayToString (&arrayString, itemIds, size);
    if (!arrayString)
        {
            isException = 1;
            goto end;
        }

    /* Determine type of artefact to add. */
    type = OTServiceFavoriteType (suffix);
    if (!type)
        {
            isException = 1;
            goto end;
        }

    /* Use v2 if suffix is equal to "mixes". */
    if (!(strcmp (suffix, "mixes") == 0))
        OTConcatenateString (&http.endpoint, "/v1/users/%s/favorites/%s", session->userId, suffix);
    else
        {
            reqType = PUT;
            OTConcatenateString (&http.endpoint, "/v2/favorites/%s/add", suffix);
        }

    OTConcatenateString (&http.parameter, "countryCode=%s", session->countryCode);
    OTConcatenateString (&http.postData, "%s=%s&onArtifactNotFound=%s", type, arrayString,
                         onArtifactNotFound);
    if (!http.parameter || !http.endpoint || !http.postData)
        {
            isException = 1;
            goto end;
        }

    status = OTServiceRequestSilent (session, &http, threadHandle);
end:
    free (http.endpoint);
    free (http.parameter);
    free (http.postData);
    return status;
}
