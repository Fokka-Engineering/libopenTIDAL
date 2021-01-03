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

#include "../external/cJSON.h"
#include "../http_connector.h"
#include "../openTIDAL.h"
#include "container.h"

#ifndef PARSE__h
#define PARSE__h

void openTIDAL_ParseJsonDouble (cJSON *object, double *number);
void openTIDAL_ParseJsonNumber (cJSON *object, size_t *number);
void openTIDAL_ParseJsonSignedNumber (cJSON *object, int *number);
void openTIDAL_ParseJsonNumberString (cJSON *object, char **string);
void openTIDAL_ParseJsonBool (cJSON *object, int *number);
void openTIDAL_ParseJsonString (cJSON *object, char **string);

int openTIDAL_ParseStatus (cJSON *input_json, openTIDAL_CurlContainer *Value, const char *id);
int openTIDAL_ParseResponseCodeStatus (long *code);
int openTIDAL_ParseUnauthorized (cJSON *input_json, const char *id);
int openTIDAL_ParseNotFound (cJSON *input_json, const char *id);
int openTIDAL_ParsePreconditionFailed (cJSON *input_json, const char *id);
int openTIDAL_ParseBadRequest (cJSON *input_json, const char *id);

/* parse Objects */
struct openTIDAL_JsonLoginCodeContainer openTIDAL_ParseJsonLoginCode (cJSON *directObject);
struct openTIDAL_JsonLoginTokenContainer openTIDAL_ParseJsonLoginToken (cJSON *directObject);
struct openTIDAL_JsonUserContainer openTIDAL_ParseJsonUser (cJSON *directObject);
struct openTIDAL_JsonUserSubscriptionContainer
openTIDAL_ParseJsonUserSubscription (cJSON *directObject);
struct openTIDAL_JsonPlaylistContainer openTIDAL_ParseJsonPlaylist (cJSON *directObject);
struct openTIDAL_JsonItemsContainer openTIDAL_ParseJsonItems (cJSON *directObject);
struct openTIDAL_JsonAlbumContainer openTIDAL_ParseJsonAlbum (cJSON *directObject);
struct openTIDAL_JsonArtistContainer openTIDAL_ParseJsonArtist (cJSON *directObject);
struct openTIDAL_JsonLinkContainer openTIDAL_ParseJsonLinks (cJSON *directObject);
struct openTIDAL_JsonCreditContainer json_parse_credits (cJSON *directObject);
struct openTIDAL_JsonContributorContainer openTIDAL_ParseJsonContributor (cJSON *directObject);
struct openTIDAL_JsonMixContainer openTIDAL_ParseJsonMix (cJSON *directObject);
struct openTIDAL_JsonStreamContainer openTIDAL_ParseJsonStream (cJSON *directObject);
struct openTIDAL_JsonManifestContainer openTIDAL_ParseJsonManifest (cJSON *directObject);

/* parse Values */
void openTIDAL_ParseJsonLoginCodeValues (struct openTIDAL_LoginCodeContainer *code,
                                         struct openTIDAL_JsonLoginCodeContainer *json);
void openTIDAL_ParseJsonLoginTokenValues (struct openTIDAL_LoginTokenContainer *token,
                                          struct openTIDAL_JsonLoginTokenContainer *json);
void openTIDAL_ParseJsonUserValues (struct openTIDAL_UserContainer *user,
                                    struct openTIDAL_JsonUserContainer *json);
void
openTIDAL_ParseJsonUserSubscriptionValues (struct openTIDAL_UserSubscriptionContainer *sub,
                                           struct openTIDAL_JsonUserSubscriptionContainer *json);
void openTIDAL_ParseJsonPlaylistValues (struct openTIDAL_PlaylistContainer *playlist,
                                        struct openTIDAL_JsonPlaylistContainer *json);
void openTIDAL_ParseJsonItemsValues (struct openTIDAL_ItemsContainer *items,
                                     struct openTIDAL_JsonItemsContainer *json);
void openTIDAL_ParseJsonAlbumValues (struct openTIDAL_AlbumContainer *album,
                                     struct openTIDAL_JsonAlbumContainer *json);
void openTIDAL_ParseJsonArtistValues (struct openTIDAL_ArtistContainer *artist,
                                      struct openTIDAL_JsonArtistContainer *json);
void openTIDAL_ParseJsonLinkValues (struct openTIDAL_LinkContainer *link,
                                    struct openTIDAL_JsonLinkContainer *json);
void openTIDAL_ParseJsonContributorValues (struct openTIDAL_ContributorContainer *contrib,
                                           struct openTIDAL_JsonContributorContainer *json);
void openTIDAL_ParseJsonMixValues (struct openTIDAL_MixContainer *mix,
                                   struct openTIDAL_JsonMixContainer *json);
void openTIDAL_ParseJsonStreamValues (struct openTIDAL_StreamContainer *stream,
                                      struct openTIDAL_JsonStreamContainer *json);

/* extra */
void openTIDAL_ParseSearch (openTIDAL_ContentContainer *o, cJSON *input_json);

#endif
