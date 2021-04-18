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

/* openTIDAL persistent storage helper
 */

#include <stdio.h>
#include <stdlib.h>

#include "OTJson.h"
#include "openTIDAL.h"

/* Predeclare function prototype. */
static char *OTPersistentStream (const struct OTSessionContainer *const session);

/* Create a file handle and create the persistent config. */
int
OTPersistentCreate (const struct OTSessionContainer *const session, const char *const location)
{
    if (location)
        {
            FILE *fp;
            char *stream;

            if (session->verboseMode)
                fprintf (stderr, "* Create file-handle and create config at location.\n");

            fp = fopen (location, "w");
            if (!fp) return -1;

            if (session->verboseMode) fprintf (stderr, "* Create json stream.\n");

            stream = OTPersistentStream (session);
            if (!stream)
                {
                    if (session->verboseMode)
                        fprintf (stderr, "* Failed to create json stream. Aborting...\n");

                    fclose (fp);
                    return -1;
                }
            fprintf (fp, "%s", stream);
            free (stream);
            fclose (fp);
        }
    else
        return -1;
    return 0;
}

/* Create a stream with the values in the OTSessionContainer.
 * Returns a heap allocated string, free it after use! */
static char *
OTPersistentStream (const struct OTSessionContainer *const session)
{
    char *ptr = NULL;
    struct OTJsonContainer *authorisation = NULL;
    struct OTJsonContainer *accessToken = NULL;
    struct OTJsonContainer *refreshToken = NULL;
    struct OTJsonContainer *expiresIn = NULL;
    struct OTJsonContainer *timeFrame = NULL;
    struct OTJsonContainer *user = NULL;
    struct OTJsonContainer *id = NULL;
    struct OTJsonContainer *countryCode = NULL;
    struct OTJsonContainer *locale = NULL;
    struct OTJsonContainer *audioQuality = NULL;
    struct OTJsonContainer *videoQuality = NULL;

    /* Create json root object. */
    struct OTJsonContainer *json = OTJsonCreateObject ();
    if (!json) goto end;

    /* Create json objects. */
    authorisation = OTJsonCreateObject ();
    user = OTJsonCreateObject ();
    if (!authorisation || !user) goto end;

    /* If successful, the ownership of the pointer is transferred
     * to the OTJsonContainer */
    OTJsonAddItemToObject (json, "authorisation", authorisation);
    OTJsonAddItemToObject (json, "user", user);

    /* Create json values. */
    if (session->accessToken)
        {
            accessToken = OTJsonCreateString (session->accessToken);
            if (!accessToken) goto end;
            OTJsonAddItemToObject (authorisation, "accessToken", accessToken);
        }
    if (session->refreshToken)
        {
            refreshToken = OTJsonCreateString (session->refreshToken);
            if (!refreshToken) goto end;
            OTJsonAddItemToObject (authorisation, "refreshToken", refreshToken);
        }
    if (session->expiresIn)
        {
            expiresIn = OTJsonCreateNumber (session->expiresIn);
            if (!expiresIn) goto end;
            OTJsonAddItemToObject (authorisation, "expiresIn", expiresIn);
        }
    if (session->timeFrame)
        {
            timeFrame = OTJsonCreateNumber (session->timeFrame);
            if (!timeFrame) goto end;
            OTJsonAddItemToObject (authorisation, "timeFrame", timeFrame);
        }
    if (session->userId)
        {
            id = OTJsonCreateString (session->userId);
            if (!id) goto end;
            OTJsonAddItemToObject (user, "id", id);
        }
    if (session->countryCode)
        {
            countryCode = OTJsonCreateString (session->countryCode);
            if (!countryCode) goto end;
            OTJsonAddItemToObject (user, "countryCode", countryCode);
        }
    if (session->locale)
        {
            locale = OTJsonCreateString (session->locale);
            if (!locale) goto end;
            OTJsonAddItemToObject (user, "locale", locale);
        }
    if (session->audioQuality)
        {
            audioQuality = OTJsonCreateString (session->audioQuality);
            if (!audioQuality) goto end;
            OTJsonAddItemToObject (user, "audioQuality", audioQuality);
        }
    if (session->videoQuality)
        {
            videoQuality = OTJsonCreateString (session->videoQuality);
            if (!videoQuality) goto end;
            OTJsonAddItemToObject (user, "videoQuality", videoQuality);
        }

    /* Allocate json in heap. */
    ptr = OTJsonPrint (json);
end:
    OTJsonDelete (json);
    return ptr;
}

/* Create a file handle and allocate the contents of
 * the config file.
 * Returns a heap allocated string, free it after use! */
char *
OTPersistentLoad (const struct OTSessionContainer *const session)
{
    if (session->persistentFileLocation)
        {
            FILE *fp;
            size_t size = 0;
            char *ptr = NULL;
            fp = fopen (session->persistentFileLocation, "r");
            if (!fp) return NULL;

            fseek (fp, 0L, SEEK_END);
            size = ftell (fp);
            rewind (fp);

            /* Allocate memory for entire content. */
            ptr = malloc (size + 1);
            if (!ptr)
                {
                    fclose (fp);
                    return NULL;
                }

            if (1 != fread (ptr, size, 1, fp))
                {
                    fclose (fp);
                    free (ptr);
                    return NULL;
                }
            fclose (fp);
            return ptr;
        }
    return NULL;
}

/* Points the session->tree value to the
 * allocated OTJsonContainer. Free it after use! */
int
OTPersistentParse (struct OTSessionContainer *const session, const char *stream)
{
    int status = 0;
    const struct OTJsonContainer *authorisation = NULL;
    const struct OTJsonContainer *user = NULL;
    struct OTJsonContainer *json = OTJsonParse (stream);
    if (!json)
        {
            if (session->verboseMode) fprintf (stderr, "* Parsing configuration stream failed.\n");
            status = -1;
            goto end;
        }

    /* Parse values. */
    authorisation = OTJsonGetObjectItem (json, "authorisation");
    user = OTJsonGetObjectItem (json, "user");
    if (!authorisation || !user)
        {
            status = -1;
            goto end;
        }

    session->accessToken = OTJsonGetObjectItemStringValue (authorisation, "accessToken");
    if (!session->accessToken)
        {
            status = -1;
            goto end;
        }
    session->refreshToken = OTJsonGetObjectItemStringValue (authorisation, "refreshToken");
    if (!session->refreshToken)
        {
            status = -1;
            goto end;
        }
    session->expiresIn = OTJsonGetObjectItemNumberValue (authorisation, "expiresIn");
    session->timeFrame = OTJsonGetObjectItemNumberValue (authorisation, "timeFrame");
    session->userId = OTJsonGetObjectItemStringValue (user, "id");
    if (!session->userId)
        {
            status = -1;
            goto end;
        }
    session->countryCode = OTJsonGetObjectItemStringValue (user, "countryCode");
    if (!session->countryCode)
        {
            status = -1;
            goto end;
        }
    session->locale = OTJsonGetObjectItemStringValue (user, "locale");
    session->audioQuality = OTJsonGetObjectItemStringValue (user, "audioQuality");
    session->videoQuality = OTJsonGetObjectItemStringValue (user, "videoQuality");

    /* Delete old tree. */
    OTJsonDelete (session->tree);
    session->tree = json;
end:
    if (status == -1)
        {
            OTJsonDelete (json);
            fprintf (stderr, "* Aborting configuration stream parsing...\n");
        }
    return status;
}
