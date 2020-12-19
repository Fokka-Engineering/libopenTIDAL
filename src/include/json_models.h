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

#include "cJSON.h"
#pragma once

/* structures used to parse a json response */

typedef struct json_login_code_model
{
  /* Raw JSON Object */
  cJSON *directObject;
  /* Login Code Values */
  cJSON *deviceCode;
  cJSON *userCode;
  cJSON *verificationUri;
  cJSON *verificationUriComplete;
  cJSON *expiresIn;
  cJSON *interval;
} json_login_code_model;

typedef struct json_login_token_model
{
  /* Raw JSON Object */
  cJSON *directObject;
  /* Login Token Values */
  cJSON *access_token;
  cJSON *refresh_token;
  cJSON *token_type;
  cJSON *expires_in;
  cJSON *userId;
  cJSON *email;
  cJSON *countryCode;
  cJSON *fullName;
  cJSON *firstName;
  cJSON *lastName;
  cJSON *nickname;
  cJSON *username;
  cJSON *imageId;
  cJSON *created;
  cJSON *updated;
  cJSON *facebookUid;
  cJSON *appleUid;
} json_login_token_model;

typedef struct json_user_model
{
  /* Raw JSON Object */
  cJSON *directObject;
  /* User Values */
  cJSON *id;
  cJSON *username;
  cJSON *firstName;
  cJSON *lastName;
  cJSON *email;
  cJSON *countryCode;
  cJSON *created;
  cJSON *picture;
  cJSON *newsletter;
  cJSON *acceptedEULA;
  cJSON *gender;
  cJSON *dateOfBirth;
  cJSON *facebookUid;
  cJSON *appleUid;
} json_user_model;

typedef struct json_user_subscription_model
{
  /* Raw JSON Object */
  cJSON *directObject;
  /* Subscription Values */
  cJSON *validUntil;
  cJSON *status;
  cJSON *type;
  cJSON *offlineGracePeriod;
  cJSON *highestSoundQuality;
  cJSON *premiumAccess;
  cJSON *canGetTrial;
  cJSON *paymentType;
} json_user_subscription_model;

typedef struct json_playlist_model
{
  /* Raw JSON Object */
  cJSON *directObject;
  /* Pagination Values */
  cJSON *totalNumberOfItems;
  cJSON *limit;
  cJSON *offset;
  /* Playlist Values */
  cJSON *uuid;
  cJSON *title;
  cJSON *description;
  cJSON *popularity;
  cJSON *duration;
  cJSON *created;
  cJSON *lastUpdated;
  cJSON *numberOfTracks;
  cJSON *numberOfVideos;
  cJSON *publicPlaylist;
  cJSON *image;
  cJSON *squareImage;
  cJSON *type;
} json_playlist_model;

typedef struct json_items_model
{
  /* Raw JSON Object */
  cJSON *directObject;
  /* Pagination Values */
  cJSON *totalNumberOfItems;
  cJSON *limit;
  cJSON *offset;
  /* Items Values */
  cJSON *id;
  cJSON *title;
  cJSON *duration;
  cJSON *popularity;
  cJSON *trackNumber;
  cJSON *volumeNumber;
  cJSON *version;
  cJSON *cover;
  cJSON *videoCover;
  cJSON *imageId;
  cJSON *quality;
  cJSON *audioQuality;
  cJSON *explicitItem;
  cJSON *allowStreaming;
  cJSON *streamReady;
  cJSON *replayGain;
  cJSON *peak;
  cJSON *artists;
  cJSON **artistId;
  cJSON **artistName;
  cJSON *albumId;
  cJSON *albumTitle;
  cJSON *type;
} json_items_model;

typedef struct json_album_model
{
  /* Raw JSON Object */
  cJSON *directObject;
  /* Pagination Values */
  cJSON *totalNumberOfItems;
  cJSON *limit;
  cJSON *offset;
  /* Album Values */
  cJSON *id;
  cJSON *title;
  cJSON *duration;
  cJSON *popularity;
  cJSON *copyright;
  cJSON *quality;
  cJSON *cover;
  cJSON *videoCover;
  cJSON *releaseDate;
  cJSON *version;
  cJSON *explicitItem;
  cJSON *allowStreaming;
  cJSON *streamReady;
  cJSON *numberOfTracks;
  cJSON *numberOfVideos;
  cJSON *numberOfVolumes;
  cJSON *artists;
  cJSON **artistId;
  cJSON **artistName;
} json_album_model;

typedef struct json_artist_model
{
  /* Raw JSON Object */
  cJSON *directObject;
  /* Pagination Values */
  cJSON *totalNumberOfItems;
  cJSON *limit;
  cJSON *offset;
  /* Artist Values */
  cJSON *id;
  cJSON *name;
  cJSON *picture;
  cJSON *popularity;
} json_artist_model;

typedef struct json_link_model
{
  /* Raw JSON Object */
  cJSON *directObject;
  /* Pagination Values */
  cJSON *totalNumberOfItems;
  cJSON *limit;
  cJSON *offset;
  /* Link Values */
  cJSON *url;
  cJSON *siteName;
  cJSON *source;
} json_links_model;

typedef struct json_credit_model
{
  /* Raw JSON Object */
  cJSON *directObject;
  /* Pagination Values */
  cJSON *totalNumberOfItems;
  cJSON *limit;
  cJSON *offset;
  /* Credit Values */
  cJSON *type;
  cJSON *name;
  cJSON *id;
} json_credit_model;

typedef struct json_contributor_model
{
  /* Raw JSON Object */
  cJSON *directObject;
  /* Pagination Values */
  cJSON *totalNumberOfItems;
  cJSON *limit;
  cJSON *offset;
  /* Contributor Values */
  cJSON *name;
  cJSON *role;
} json_contributor_model;

typedef struct json_stream_model
{
  /* Raw JSON Object */
  cJSON *directObject;
  /* Stream Values */
  cJSON *trackId;
  cJSON *videoId;
  cJSON *assetPresentation;
  cJSON *audioQuality;
  cJSON *audioMode;
  cJSON *videoQuality;
  cJSON *manifestMimeType;
  cJSON *manifest;
} json_stream_model;

typedef struct json_manifest_model
{
  /* Raw JSON Object */
  cJSON *directObject;
  /* Manifest Values */
  cJSON *mimeType;
  cJSON *codec;
  cJSON *encryptionType;
  cJSON *urls;
  cJSON *url;
} json_manifest_model;

typedef struct json_mix_model
{
  /* Raw JSON Object */
  cJSON *directObject;
  /* Pagination Values */
  cJSON *totalNumberOfItems;
  cJSON *limit;
  cJSON *offset;
  /* Mix Values */
  cJSON *id;
  cJSON *title;
  cJSON *subTitle;
  cJSON *smallImageWidth;
  cJSON *smallImageHeight;
  cJSON *smallImageUrl;
  cJSON *mediumImageWidth;
  cJSON *mediumImageHeight;
  cJSON *mediumImageUrl;
  cJSON *largeImageWidth;
  cJSON *largeImageHeight;
  cJSON *largeImageUrl;
  cJSON *mixType;
} json_mix_model;
