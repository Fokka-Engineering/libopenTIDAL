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

#include "parse.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* TODO: Add Credit Parser */

void
openTIDAL_ParseJsonLoginCodeValues (struct openTIDAL_LoginCodeContainer *code,
                                    struct openTIDAL_JsonLoginCodeContainer *json)
{
    openTIDAL_ParseJsonString (json->deviceCode, &code->deviceCode);
    openTIDAL_ParseJsonString (json->userCode, &code->userCode);
    openTIDAL_ParseJsonString (json->verificationUri, &code->verificationUri);
    openTIDAL_ParseJsonString (json->verificationUriComplete, &code->verificationUriComplete);
    openTIDAL_ParseJsonNumber (json->expiresIn, &code->timeFrame);
    openTIDAL_ParseJsonSignedNumber (json->interval, &code->interval);
}

void
openTIDAL_ParseJsonLoginTokenValues (struct openTIDAL_LoginTokenContainer *Value,
                                     struct openTIDAL_JsonLoginTokenContainer *json)
{
    openTIDAL_ParseJsonString (json->access_token, &Value->access_token);
    openTIDAL_ParseJsonString (json->refresh_token, &Value->refresh_token);
    openTIDAL_ParseJsonString (json->token_type, &Value->token_type);
    openTIDAL_ParseJsonNumber (json->expires_in, &Value->timeFrame);
    openTIDAL_ParseJsonNumberString (json->userId, &Value->userId);
    openTIDAL_ParseJsonString (json->email, &Value->email);
    openTIDAL_ParseJsonString (json->countryCode, &Value->countryCode);
    openTIDAL_ParseJsonString (json->fullName, &Value->fullName);
    openTIDAL_ParseJsonString (json->firstName, &Value->firstName);
    openTIDAL_ParseJsonString (json->lastName, &Value->lastName);
    openTIDAL_ParseJsonString (json->nickname, &Value->nickname);
    openTIDAL_ParseJsonString (json->username, &Value->username);
    openTIDAL_ParseJsonString (json->imageId, &Value->imageId);
    openTIDAL_ParseJsonNumberString (json->facebookUid, &Value->facebookUid);
    openTIDAL_ParseJsonString (json->appleUid, &Value->appleUid);
}

void
openTIDAL_ParseJsonUserValues (struct openTIDAL_UserContainer *Value,
                               struct openTIDAL_JsonUserContainer *json)
{
    openTIDAL_ParseJsonNumberString (json->id, &Value->id);
    openTIDAL_ParseJsonString (json->username, &Value->username);
    openTIDAL_ParseJsonString (json->firstName, &Value->firstName);
    openTIDAL_ParseJsonString (json->lastName, &Value->lastName);
    openTIDAL_ParseJsonString (json->email, &Value->email);
    openTIDAL_ParseJsonString (json->countryCode, &Value->countryCode);
    openTIDAL_ParseJsonString (json->picture, &Value->picture);
    openTIDAL_ParseJsonBool (json->newsletter, &Value->newsletter);
    openTIDAL_ParseJsonBool (json->acceptedEULA, &Value->acceptedEULA);
    openTIDAL_ParseJsonString (json->gender, &Value->gender);
    openTIDAL_ParseJsonString (json->dateOfBirth, &Value->dateOfBirth);
    openTIDAL_ParseJsonNumberString (json->facebookUid, &Value->facebookUid);
    openTIDAL_ParseJsonString (json->appleUid, &Value->appleUid);
}

void
openTIDAL_ParseJsonUserSubscriptionValues (struct openTIDAL_UserSubscriptionContainer *Value,
                                           struct openTIDAL_JsonUserSubscriptionContainer *json)
{
    openTIDAL_ParseJsonString (json->validUntil, &Value->validUntil);
    openTIDAL_ParseJsonString (json->status, &Value->accountStatus);
    openTIDAL_ParseJsonString (json->type, &Value->type);
    openTIDAL_ParseJsonSignedNumber (json->offlineGracePeriod, &Value->offlineGracePeriod);
    openTIDAL_ParseJsonString (json->highestSoundQuality, &Value->highestSoundQuality);
    openTIDAL_ParseJsonBool (json->premiumAccess, &Value->premiumAccess);
    openTIDAL_ParseJsonBool (json->canGetTrial, &Value->canGetTrial);
    openTIDAL_ParseJsonString (json->paymentType, &Value->paymentType);
}

void
openTIDAL_ParseJsonPlaylistValues (struct openTIDAL_PlaylistContainer *playlist,
                                   struct openTIDAL_JsonPlaylistContainer *json)
{
    openTIDAL_ParseJsonString (json->uuid, &playlist->uuid);
    openTIDAL_ParseJsonString (json->title, &playlist->title);
    openTIDAL_ParseJsonString (json->description, &playlist->description);
    openTIDAL_ParseJsonSignedNumber (json->popularity, &playlist->popularity);
    openTIDAL_ParseJsonSignedNumber (json->duration, &playlist->duration);
    openTIDAL_ParseJsonString (json->created, &playlist->created);
    openTIDAL_ParseJsonString (json->lastUpdated, &playlist->lastUpdated);
    openTIDAL_ParseJsonSignedNumber (json->numberOfTracks, &playlist->numberOfTracks);
    openTIDAL_ParseJsonSignedNumber (json->numberOfVideos, &playlist->numberOfVideos);
    openTIDAL_ParseJsonBool (json->publicPlaylist, &playlist->publicPlaylist);
    openTIDAL_ParseJsonString (json->image, &playlist->image);
    openTIDAL_ParseJsonString (json->squareImage, &playlist->squareImage);
    openTIDAL_ParseJsonString (json->type, &playlist->type);
}

void
openTIDAL_ParseJsonItemsValues (struct openTIDAL_ItemsContainer *Value,
                                struct openTIDAL_JsonItemsContainer *json)
{
    openTIDAL_ParseJsonNumberString (json->id, &Value->id);
    openTIDAL_ParseJsonString (json->title, &Value->title);
    openTIDAL_ParseJsonSignedNumber (json->duration, &Value->duration);
    openTIDAL_ParseJsonSignedNumber (json->popularity, &Value->popularity);
    openTIDAL_ParseJsonSignedNumber (json->trackNumber, &Value->trackNumber);
    openTIDAL_ParseJsonSignedNumber (json->volumeNumber, &Value->volumeNumber);
    openTIDAL_ParseJsonString (json->version, &Value->version);
    openTIDAL_ParseJsonString (json->cover, &Value->cover);
    openTIDAL_ParseJsonString (json->videoCover, &Value->videoCover);
    openTIDAL_ParseJsonString (json->imageId, &Value->imageId);
    openTIDAL_ParseJsonString (json->audioQuality, &Value->audioQuality);
    openTIDAL_ParseJsonString (json->quality, &Value->quality);
    openTIDAL_ParseJsonBool (json->explicitItem, &Value->explicitItem);
    openTIDAL_ParseJsonBool (json->allowStreaming, &Value->allowStreaming);
    openTIDAL_ParseJsonBool (json->streamReady, &Value->streamReady);
    openTIDAL_ParseJsonDouble (json->replayGain, &Value->replayGain);
    openTIDAL_ParseJsonDouble (json->peak, &Value->peak);
    openTIDAL_ParseJsonNumberString (json->albumId, &Value->albumId);
    openTIDAL_ParseJsonString (json->albumTitle, &Value->albumTitle);

    if (cJSON_IsObject (json->quality)) {
        Value->isVideo = 1;
    }
    else {
        Value->isVideo = 0;
    }
    if (cJSON_IsArray (json->artists)) {
        int i;
        Value->subArraySize = cJSON_GetArraySize (json->artists);
        Value->artistId = malloc (sizeof (char *) * Value->subArraySize);
        Value->artistName = malloc (sizeof (char *) * Value->subArraySize);

        for (i = 0; i < Value->subArraySize; ++i) {
            openTIDAL_ParseJsonNumberString (json->artistId[i], &Value->artistId[i]);
            openTIDAL_ParseJsonString (json->artistName[i], &Value->artistName[i]);
        }

        free (json->artistId);
        free (json->artistName);
    }
    else {
        Value->subArraySize = 0;
    }
}

void
openTIDAL_ParseJsonAlbumValues (struct openTIDAL_AlbumContainer *Value,
                                struct openTIDAL_JsonAlbumContainer *json)
{
    openTIDAL_ParseJsonNumberString (json->id, &Value->id);
    openTIDAL_ParseJsonString (json->title, &Value->title);
    openTIDAL_ParseJsonSignedNumber (json->duration, &Value->duration);
    openTIDAL_ParseJsonSignedNumber (json->popularity, &Value->popularity);
    openTIDAL_ParseJsonString (json->copyright, &Value->copyright);
    openTIDAL_ParseJsonString (json->quality, &Value->quality);
    openTIDAL_ParseJsonString (json->cover, &Value->cover);
    openTIDAL_ParseJsonString (json->videoCover, &Value->videoCover);
    openTIDAL_ParseJsonString (json->releaseDate, &Value->releaseDate);
    openTIDAL_ParseJsonString (json->version, &Value->version);
    openTIDAL_ParseJsonBool (json->explicitItem, &Value->explicitItem);
    openTIDAL_ParseJsonBool (json->allowStreaming, &Value->allowStreaming);
    openTIDAL_ParseJsonBool (json->streamReady, &Value->streamReady);
    openTIDAL_ParseJsonSignedNumber (json->numberOfTracks, &Value->numberOfTracks);
    openTIDAL_ParseJsonSignedNumber (json->numberOfVideos, &Value->numberOfVideos);
    openTIDAL_ParseJsonSignedNumber (json->numberOfVolumes, &Value->numberOfVolumes);

    if (cJSON_IsArray (json->artists)) {
        int i;
        Value->subArraySize = cJSON_GetArraySize (json->artists);
        Value->artistId = malloc (sizeof (char *) * Value->subArraySize);
        Value->artistName = malloc (sizeof (char *) * Value->subArraySize);

        for (i = 0; i < Value->subArraySize; ++i) {
            openTIDAL_ParseJsonNumberString (json->artistId[i], &Value->artistId[i]);
            openTIDAL_ParseJsonString (json->artistName[i], &Value->artistName[i]);
        }

        free (json->artistId);
        free (json->artistName);
    }
    else {
        Value->subArraySize = 0;
    }
}

void
openTIDAL_ParseJsonArtistValues (struct openTIDAL_ArtistContainer *Value,
                                 struct openTIDAL_JsonArtistContainer *json)
{
    openTIDAL_ParseJsonNumberString (json->id, &Value->id);
    openTIDAL_ParseJsonString (json->name, &Value->name);
    openTIDAL_ParseJsonString (json->picture, &Value->picture);
    openTIDAL_ParseJsonSignedNumber (json->popularity, &Value->popularity);
}

void
openTIDAL_ParseJsonLinkValues (struct openTIDAL_LinkContainer *Value,
                               struct openTIDAL_JsonLinkContainer *json)
{
    openTIDAL_ParseJsonString (json->url, &Value->url);
    openTIDAL_ParseJsonString (json->siteName, &Value->siteName);
}

void
openTIDAL_ParseJsonContributorValues (struct openTIDAL_ContributorContainer *Value,
                                      struct openTIDAL_JsonContributorContainer *json)
{
    Value->name = malloc (sizeof (char *) + Value->arraySize);
    Value->id = malloc (sizeof (char *) + Value->arraySize);
    Value->role = malloc (sizeof (char *) + Value->arraySize);
}

void
openTIDAL_ParseJsonMixValues (struct openTIDAL_MixContainer *Value,
                              struct openTIDAL_JsonMixContainer *json)
{
    openTIDAL_ParseJsonString (json->id, &Value->id);
    openTIDAL_ParseJsonString (json->title, &Value->title);
    openTIDAL_ParseJsonString (json->subTitle, &Value->subTitle);
    openTIDAL_ParseJsonSignedNumber (json->smallImageWidth, &Value->smallImageWidth);
    openTIDAL_ParseJsonSignedNumber (json->smallImageHeight, &Value->smallImageHeight);
    openTIDAL_ParseJsonString (json->smallImageUrl, &Value->smallImageUrl);
    openTIDAL_ParseJsonSignedNumber (json->mediumImageWidth, &Value->mediumImageWidth);
    openTIDAL_ParseJsonSignedNumber (json->mediumImageHeight, &Value->mediumImageHeight);
    openTIDAL_ParseJsonString (json->mediumImageUrl, &Value->mediumImageUrl);
    openTIDAL_ParseJsonSignedNumber (json->largeImageWidth, &Value->largeImageWidth);
    openTIDAL_ParseJsonSignedNumber (json->largeImageHeight, &Value->largeImageHeight);
    openTIDAL_ParseJsonString (json->largeImageUrl, &Value->largeImageUrl);
    openTIDAL_ParseJsonString (json->mixType, &Value->mixType);
}

void
openTIDAL_ParseJsonStreamValues (struct openTIDAL_StreamContainer *Value,
                                 struct openTIDAL_JsonStreamContainer *json)
{
    openTIDAL_ParseJsonNumberString (json->trackId, &Value->trackId);
    openTIDAL_ParseJsonNumberString (json->videoId, &Value->videoId);
    openTIDAL_ParseJsonString (json->assetPresentation, &Value->assetPresentation);
    openTIDAL_ParseJsonString (json->audioQuality, &Value->audioQuality);
    openTIDAL_ParseJsonString (json->audioMode, &Value->audioMode);
    openTIDAL_ParseJsonString (json->videoQuality, &Value->videoQuality);
    openTIDAL_ParseJsonString (json->manifestMimeType, &Value->manifestMimeType);
    openTIDAL_ParseJsonString (json->manifest, &Value->manifest);
}
