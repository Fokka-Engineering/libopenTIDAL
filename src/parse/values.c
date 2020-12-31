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

#include "parse.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* TODO: Add Credit Parser */

void
parse_login_code_values (openTIDAL_LoginCodeContainer *code, json_login_code_model *json)
{
    parse_string (json->deviceCode, &code->deviceCode);
    parse_string (json->userCode, &code->userCode);
    parse_string (json->verificationUri, &code->verificationUri);
    parse_string (json->verificationUriComplete, &code->verificationUriComplete);
    parse_number (json->expiresIn, &code->timeFrame);
    parse_signed_number (json->interval, &code->interval);
}

void
parse_login_token_values (openTIDAL_LoginTokenContainer *Value, json_login_token_model *json)
{
    parse_string (json->access_token, &Value->access_token);
    parse_string (json->refresh_token, &Value->refresh_token);
    parse_string (json->token_type, &Value->token_type);
    parse_number (json->expires_in, &Value->timeFrame);
    parse_number (json->userId, &Value->userId);
    parse_string (json->email, &Value->email);
    parse_string (json->countryCode, &Value->countryCode);
    parse_string (json->fullName, &Value->fullName);
    parse_string (json->firstName, &Value->firstName);
    parse_string (json->lastName, &Value->lastName);
    parse_string (json->nickname, &Value->nickname);
    parse_string (json->username, &Value->username);
    parse_string (json->imageId, &Value->imageId);
    parse_number (json->created, &Value->created);
    parse_number (json->updated, &Value->updated);
    parse_number (json->facebookUid, &Value->facebookUid);
    parse_string (json->appleUid, &Value->appleUid);
}

void
parse_user_values (openTIDAL_UserContainer *Value, json_user_model *json)
{
    parse_number (json->id, &Value->id);
    parse_string (json->username, &Value->username);
    parse_string (json->firstName, &Value->firstName);
    parse_string (json->lastName, &Value->lastName);
    parse_string (json->email, &Value->email);
    parse_string (json->countryCode, &Value->countryCode);
    parse_string (json->created, &Value->created);
    parse_string (json->picture, &Value->picture);
    parse_bool (json->newsletter, &Value->newsletter);
    parse_bool (json->acceptedEULA, &Value->acceptedEULA);
    parse_string (json->gender, &Value->gender);
    parse_string (json->dateOfBirth, &Value->dateOfBirth);
    parse_number (json->facebookUid, &Value->facebookUid);
    parse_string (json->appleUid, &Value->appleUid);
}

void
parse_user_subscription_values (openTIDAL_UserSubscriptionContainer *Value,
                                json_user_subscription_model *json)
{
    parse_string (json->validUntil, &Value->validUntil);
    parse_string (json->status, &Value->accountStatus);
    parse_string (json->type, &Value->type);
    parse_number (json->offlineGracePeriod, &Value->offlineGracePeriod);
    parse_string (json->highestSoundQuality, &Value->highestSoundQuality);
    parse_bool (json->premiumAccess, &Value->premiumAccess);
    parse_bool (json->canGetTrial, &Value->canGetTrial);
    parse_string (json->paymentType, &Value->paymentType);
}

void
parse_playlist_values (openTIDAL_PlaylistContainer *playlist, json_playlist_model *json)
{
    parse_string (json->uuid, &playlist->uuid);
    parse_string (json->title, &playlist->title);
    parse_string (json->description, &playlist->description);
    parse_signed_number (json->popularity, &playlist->popularity);
    parse_number (json->duration, &playlist->duration);
    parse_string (json->created, &playlist->created);
    parse_string (json->lastUpdated, &playlist->lastUpdated);
    parse_signed_number (json->numberOfTracks, &playlist->numberOfTracks);
    parse_signed_number (json->numberOfVideos, &playlist->numberOfVideos);
    parse_bool (json->publicPlaylist, &playlist->publicPlaylist);
    parse_string (json->image, &playlist->image);
    parse_string (json->squareImage, &playlist->squareImage);
    parse_string (json->type, &playlist->type);
}

void
parse_items_values (openTIDAL_ItemsContainer *Value, json_items_model *json)
{
    parse_number (json->id, &Value->id);
    parse_string (json->title, &Value->title);
    parse_number (json->duration, &Value->duration);
    parse_signed_number (json->popularity, &Value->popularity);
    parse_signed_number (json->trackNumber, &Value->trackNumber);
    parse_signed_number (json->volumeNumber, &Value->volumeNumber);
    parse_string (json->version, &Value->version);
    parse_string (json->cover, &Value->cover);
    parse_string (json->videoCover, &Value->videoCover);
    parse_string (json->imageId, &Value->imageId);
    parse_string (json->audioQuality, &Value->audioQuality);
    parse_string (json->quality, &Value->quality);
    parse_bool (json->explicitItem, &Value->explicitItem);
    parse_bool (json->allowStreaming, &Value->allowStreaming);
    parse_bool (json->streamReady, &Value->streamReady);
    parse_double (json->replayGain, &Value->replayGain);
    parse_double (json->peak, &Value->peak);
    parse_number (json->albumId, &Value->albumId);
    parse_string (json->albumTitle, &Value->albumTitle);

    if (cJSON_IsNull (json->album)) {
        Value->isVideo = 1;
    }
    else {
        Value->isVideo = 0;
    }
    if (cJSON_IsArray (json->artists)) {
        int i;
        Value->subArraySize = cJSON_GetArraySize (json->artists);
        Value->artistId = malloc (sizeof (size_t) * Value->subArraySize);
        Value->artistName = malloc (sizeof (char *) * Value->subArraySize);

        for (i = 0; i < Value->subArraySize; ++i) {
            parse_number (json->artistId[i], &Value->artistId[i]);
            parse_string (json->artistName[i], &Value->artistName[i]);
        }

        free (json->artistId);
        free (json->artistName);
    }
    else {
        Value->subArraySize = 0;
    }
}

void
parse_album_values (openTIDAL_AlbumContainer *Value, json_album_model *json)
{
    parse_number (json->id, &Value->id);
    parse_string (json->title, &Value->title);
    parse_number (json->duration, &Value->duration);
    parse_signed_number (json->popularity, &Value->popularity);
    parse_string (json->copyright, &Value->copyright);
    parse_string (json->quality, &Value->quality);
    parse_string (json->cover, &Value->cover);
    parse_string (json->videoCover, &Value->videoCover);
    parse_string (json->releaseDate, &Value->releaseDate);
    parse_string (json->version, &Value->version);
    parse_bool (json->explicitItem, &Value->explicitItem);
    parse_bool (json->allowStreaming, &Value->allowStreaming);
    parse_bool (json->streamReady, &Value->streamReady);
    parse_signed_number (json->numberOfTracks, &Value->numberOfTracks);
    parse_signed_number (json->numberOfVideos, &Value->numberOfVideos);
    parse_signed_number (json->numberOfVolumes, &Value->numberOfVolumes);

    if (cJSON_IsArray (json->artists)) {
        int i;
        Value->subArraySize = cJSON_GetArraySize (json->artists);
        Value->artistId = malloc (sizeof (size_t) * Value->subArraySize);
        Value->artistName = malloc (sizeof (char *) * Value->subArraySize);

        for (i = 0; i < Value->subArraySize; ++i) {
            parse_number (json->artistId[i], &Value->artistId[i]);
            parse_string (json->artistName[i], &Value->artistName[i]);
        }

        free (json->artistId);
        free (json->artistName);
    }
    else {
        Value->subArraySize = 0;
    }
}

void
parse_artist_values (openTIDAL_ArtistContainer *Value, json_artist_model *json)
{
    parse_number (json->id, &Value->id);
    parse_string (json->name, &Value->name);
    parse_string (json->picture, &Value->picture);
    parse_signed_number (json->popularity, &Value->popularity);
}

void
parse_link_values (openTIDAL_LinkContainer *Value, json_links_model *json)
{
    parse_string (json->url, &Value->url);
    parse_string (json->siteName, &Value->siteName);
}

void
parse_contributor_values (openTIDAL_ContributorContainer *Value, json_contributor_model *json)
{
    parse_string (json->name, &Value->name);
    parse_string (json->role, &Value->role);
}

void
parse_mix_values (openTIDAL_MixContainer *Value, json_mix_model *json)
{
    parse_string (json->id, &Value->id);
    parse_string (json->title, &Value->title);
    parse_string (json->subTitle, &Value->subTitle);
    parse_signed_number (json->smallImageWidth, &Value->smallImageWidth);
    parse_signed_number (json->smallImageHeight, &Value->smallImageHeight);
    parse_string (json->smallImageUrl, &Value->smallImageUrl);
    parse_signed_number (json->mediumImageWidth, &Value->mediumImageWidth);
    parse_signed_number (json->mediumImageHeight, &Value->mediumImageHeight);
    parse_string (json->mediumImageUrl, &Value->mediumImageUrl);
    parse_signed_number (json->largeImageWidth, &Value->largeImageWidth);
    parse_signed_number (json->largeImageHeight, &Value->largeImageHeight);
    parse_string (json->largeImageUrl, &Value->largeImageUrl);
    parse_string (json->mixType, &Value->mixType);
}

void
parse_stream_values (openTIDAL_StreamContainer *Value, json_stream_model *json)
{
    parse_number (json->trackId, &Value->trackId);
    parse_number (json->videoId, &Value->videoId);
    parse_string (json->assetPresentation, &Value->assetPresentation);
    parse_string (json->audioQuality, &Value->audioQuality);
    parse_string (json->audioMode, &Value->audioMode);
    parse_string (json->videoQuality, &Value->videoQuality);
    parse_string (json->manifestMimeType, &Value->manifestMimeType);
    parse_string (json->manifest, &Value->manifest);
}
