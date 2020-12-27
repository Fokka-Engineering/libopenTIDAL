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
#include <time.h>

#include "../include/openTIDAL.h"

struct openTIDAL_LoginCodeContainer {
    char *deviceCode; /* deviceCode is always 36 Chars long */
    char *userCode; /* userCode is always 5 Chars long */
    char *verificationUri;
    char *verificationUriComplete;
    size_t timeFrame;
    time_t expires_in;
    int interval;
};

struct openTIDAL_LoginTokenContainer { 
    time_t expires_in;
    char *access_token;
    char *refresh_token;
    char *token_type;
    size_t timeFrame;
    char *email;
    char *countryCode;
    char *fullName;
    char *firstName;
    char *lastName;
    char *nickname;
    char *username;
    char *imageId;
    size_t created;
    size_t updated;
    size_t userId;
    size_t facebookUid;
    char *appleUid;
    char *error;
};

struct openTIDAL_UserContainer {
    size_t id;
    char *username;
    char *firstName;
    char *lastName;
    char *email;
    char *countryCode;
    char *created;
    char *picture; /* Always 36 Chars Long */
    int newsletter;
    int acceptedEULA;
    char *gender;
    char *dateOfBirth;
    size_t facebookUid;
    char *appleUid;
};

struct openTIDAL_UserSubscriptionContainer {
    char *validUntil;
    char *accountStatus;
    char *type;
    size_t offlineGracePeriod;
    char *highestSoundQuality;
    int premiumAccess;
    int canGetTrial;
    char *paymentType;
};

struct openTIDAL_PlaylistContainer {
    int limit;
    int offset;
    int totalNumberOfItems;
    int numberOfTracks;
    int numberOfVideos;
    char *uuid; /* Always 36 Chars Long */
    char *title;
    char *description;
    size_t duration;
    char *lastUpdated;
    char *created;
    int publicPlaylist;
    char *image;
    char *squareImage;
    char *type;
    int popularity;
};

struct openTIDAL_ItemsContainer { 
    int limit;
    int offset;
    int totalNumberOfItems;
    int subArraySize; /* 2nd ArraySize of Artist Array */
    size_t id;
    char *title;
    int explicitItem;
    size_t duration;
    int allowStreaming;
    int streamReady;
    double replayGain;
    double peak;
    int popularity;
    int trackNumber;
    int volumeNumber;
    char *version;
    size_t *artistId;
    char **artistName;
    size_t albumId;
    char *albumTitle;
    char *cover;
    char *videoCover;
    char *imageId;
    char *audioQuality;
    char *quality;
    int isVideo;
};

struct openTIDAL_AlbumContainer {
    int limit;
    int offset;
    int totalNumberOfItems;
    int subArraySize; /* 2nd ArraySize of Artist Array */
    int explicitItem;
    size_t duration;
    int allowStreaming;
    int streamReady;
    char *copyright;
    int numberOfTracks;
    int numberOfVideos;
    int numberOfVolumes;
    int popularity;
    size_t *artistId; /* 2D-Array for (featured) Artists    */
    char **artistName;
    char *quality;
    char *version;
    size_t id;
    char *title;
    char *cover;
    char *videoCover;
    char *releaseDate;
};

struct openTIDAL_CreditsContainer {
    int limit;
    int offset;
    char *type;
    char *name;
    size_t id;

};

struct openTIDAL_ArtistContainer {
    int limit;
    int offset;
    int totalNumberOfItems;
    size_t id;
    char *name;
    char *picture;
    int popularity;
};

struct openTIDAL_LinkContainer {
    int limit;
    int offset;
    int totalNumberOfItems;
    char *url;
    char *siteName;
    char *source;
};

struct openTIDAL_ETagContainer {
    int status;
    char *id;
};

struct openTIDAL_MixContainer {
    int limit;
    int offset;
    int totalNumberOfItems;
    char *id;
    char *title;
    char *subTitle;
    int smallImageWidth;
    int smallImageHeight;
    char *smallImageUrl;
    int mediumImageWidth;
    int mediumImageHeight;
    char *mediumImageUrl;
    int largeImageWidth;
    int largeImageHeight;
    char *largeImageUrl;
    char *mixType;
};

struct openTIDAL_ContributorContainer {
    int limit;
    int offset;
    int totalNumberOfItems;
    char *name;
    char *role;
};

struct openTIDAL_StreamContainer {
    char *url;
    size_t trackId;
    size_t videoId;
    char *assetPresentation;
    char *audioQuality;
    char *audioMode;
    char *videoQuality;
    char *manifestMimeType;
    char *manifest; //Pointer to temporary allocated Manifest
    char *mimeType;
    char *codec;
    char *encryptionType;
};

