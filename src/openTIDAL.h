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

#include <time.h>

#define STRUCT_INIT_CAPACITY 10
#ifndef openTIDAL__h
#define openTIDAL__h

#ifdef __cplusplus
extern "C" {
#endif

typedef struct openTIDAL_SessionContainer {
    /* File location of persistent session config.
     * If the pointer is NULL restricted authorization
     * is enabled. */
    char *location;

    /* Stream of a newly created json session config.
     * Allocated in heap if a new persistent config is
     * created or updated.  */
    char *newStream;
    /* Void pointer to allocated session config. Typecasted
     * cJSON Structure. If a persistent session config is
     * specified the file is loaded into heap and parsed with
     * cJSON. The cJSON Structure gets deallocated with
     * openTIDAL_SessionCleanup */
    void *stream;
    /* Part of the SessionAuthAutoRenewal process. Typecasted
     * openTIDAL_ContentContainer Structure. Allocated after refresh
     * request is performed.
     * Old cJSON Structure from previous request will be deallocated
     * prior. */
    void *refreshRequest;
    /* Part of the AuthCreateBearerToken process. Typecasted openTIDAL_ContentContainer
     * Structure. Allocated after request is performed. Only used
     * after persistent config creation. */
    void *tokenRequest;
    void *subscriptionRequest;

    /* Boolean to specifiy state of authorization.
     *
     * If no file location is specified cURL performs
     * a clientId-based authorization (x-tidal-token: {client_id}).
     *
     * User-based and stream requests are disabled and result in a 401.
     * A 30s preview of a track (64kbps 22.05kHz MP3) or
     * a video (640x360 1116kbps MP4) can be requested.
     * This mode is primarly useful for metadata requests. */
    int demoEnabled;

    /* Constant pointers initialised with openTIDAL_SessionInit */
    const char *clientId;
    const char *clientSecret;
    const char *scopes;
    const char *baseUrl;
    const char *authUrl;

    /* Pointers to allocated cJSON Values. Either from the persistent
     * session config, the tokenRequest or the refreshRequest. NULL if
     * demoEnabled is TRUE. */
    char *username;
    char *userId;
    time_t expiresIn;
    char *countryCode;

    char *accessToken;
    char *refreshToken;

    char *audioQuality;
    char *videoQuality;

    /* Typecasted CURL Structure. Persistent cURL request handle. */
    void *curlHandle;
} openTIDAL_SessionContainer;

/* Initialised with openTIDAL_StructInit({pointer to container}).
 * See structCtrl.c for more detailed documentation. */
typedef struct openTIDAL_ContentContainer {
    struct openTIDAL_LoginCodeContainer *code;
    struct openTIDAL_LoginTokenContainer *token;

    struct openTIDAL_UserContainer *user;
    struct openTIDAL_UserSubscriptionContainer *subscription;

    struct openTIDAL_StreamContainer *stream;
    struct openTIDAL_ModuleContainer *modules;
    /* Pointer to dynamic arrays. Allocated one by one only if
     * needed. The arrays can also function as content pools in
     * case of module based endpoints (home endpoint). In this case
     * a direct position to the content is given.
     * Each array has it's own identifier to retrieve the correct capacity
     * and total items in array values.
     * The arrays are populated with helper functions (openTIDAL_StructAdd*)
     * and grow automatically heap by doubling it's current size. This prevents
     * the expensive use of realloc.
     * The initial capacity is 10.
     * To deallocate all arrays inside the ContentContainer call
     * openTIDAL_StructDelete({pointer to container}). */
    struct openTIDAL_AlbumContainer *albums;             /* Index (Capacity & Total): 0 */
    struct openTIDAL_ItemsContainer *items;              /* Index (Capacity & Total): 1 */
    struct openTIDAL_ArtistContainer *artists;           /* Index (Capacity & Total): 2 */
    struct openTIDAL_PlaylistContainer *playlists;       /* Index (Capacity & Total): 3 */
    struct openTIDAL_MixContainer *mixes;                /* Index (Capacity & Total): 4 */
    struct openTIDAL_ContributorContainer *contributors; /* Index (Capacity & Total): 5 */
    struct openTIDAL_CreditsContainer *credits;          /* Index (Capacity & Total): 6 */
    struct openTIDAL_LinkContainer *links;               /* Index (Capacity & Total): 7 */

    /* Custom status of performed request. Error handling of the tidalapi and
     * primitive http response codes are parsed to provide detailed information
     * about the state of the request. */
    int status;

    /* This static array is being used to store the capacity of each array.
     * The capacity array keeps track of the allocated heap size of
     * each dynamic array.
     * The dynamic array identifiers are used to access the correct values */
    int capacity[8];
    /* This static array is being used to store the current number of items
     * added to each dynamic array.
     * The dynamic array identifiers are used to access the correct values */
    int total[8];

    /* Pointer to allocated cJSON Structure. After a request is performed and
     * is successful it's allocated buffer is parsed with cJSON and deallocated.
     * This pointer is used for deallocation purposes only. */
    void *json;
    /* Pointer to allocated cJSON Structure. After a get stream request is performed
     * and is successful it's stream manifest gets decoded with base64 and pased with
     * cJSON.
     * This pointer is used for deallocation purposes only. */
    void *jsonManifest;
} openTIDAL_ContentContainer;

/* Enable verbose mode stdout//stderr (enables cURL verbose and openTIDAL verbose) */
void openTIDAL_Verbose (int loglevel);
int openTIDAL_GetLogLevel ();
/* Deallocate content in struct */
void openTIDAL_StructDelete (openTIDAL_ContentContainer *o);

/* Session */
const int openTIDAL_SessionInit (openTIDAL_SessionContainer *session, const char *file_location);
const int openTIDAL_SessionCreateFile (openTIDAL_SessionContainer *session);
void openTIDAL_SessionRefresh (openTIDAL_SessionContainer *session);
void openTIDAL_SessionCleanup (openTIDAL_SessionContainer *session);

/* OAuth Service */
openTIDAL_ContentContainer *openTIDAL_AuthCreateUserCode (openTIDAL_SessionContainer *session);
openTIDAL_ContentContainer *openTIDAL_AuthCreateBearerToken (openTIDAL_SessionContainer *session,
                                                             const char *device_code);
openTIDAL_ContentContainer *openTIDAL_AuthRefreshBearerToken (openTIDAL_SessionContainer *session,
                                                              const char *refresh_token);
const int openTIDAL_AuthLogout (openTIDAL_SessionContainer *session);

/* User Service */
openTIDAL_ContentContainer *openTIDAL_GetUser (openTIDAL_SessionContainer *session);
openTIDAL_ContentContainer *openTIDAL_GetUserSubscription (openTIDAL_SessionContainer *session);

/* Playlist Service */

openTIDAL_ContentContainer *openTIDAL_GetPlaylist (openTIDAL_SessionContainer *session,
                                                   const char *playlistid);
openTIDAL_ContentContainer *openTIDAL_GetPlaylistItems (openTIDAL_SessionContainer *session,
                                                        const char *playlistid, const int limit,
                                                        const int offset);
openTIDAL_ContentContainer *openTIDAL_CreatePlaylist (openTIDAL_SessionContainer *session,
                                                      char *title, char *description);
char *openTIDAL_GetPlaylistEntityTag (openTIDAL_SessionContainer *session, const char *playlistid);

const int openTIDAL_DeletePlaylist (openTIDAL_SessionContainer *session, const char *playlistId);
const int openTIDAL_DeletePlaylistItem (openTIDAL_SessionContainer *session, const char *playlistId,
                                        const int index);
const int openTIDAL_MovePlaylistItem (openTIDAL_SessionContainer *session, const char *playlistId,
                                      const int index, const int toIndex);
const int openTIDAL_AddPlaylistItem (openTIDAL_SessionContainer *session, const char *playlistId,
                                     const char *itemId, const char *onArtifactNotFound,
                                     const char *onDupes);
const int openTIDAL_AddPlaylistItems (openTIDAL_SessionContainer *session, const char *playlistId,
                                      const char **itemIds, const int size,
                                      const char *onArtifactNotFound, const char *onDupes);

/* Album Service */

openTIDAL_ContentContainer *openTIDAL_GetAlbum (openTIDAL_SessionContainer *session,
                                                const char *albumid);
openTIDAL_ContentContainer *openTIDAL_GetAlbumItems (openTIDAL_SessionContainer *session,
                                                     const char *albumid, const int limit,
                                                     const int offset);

/* Artist Service */

openTIDAL_ContentContainer *openTIDAL_GetArtist (openTIDAL_SessionContainer *session,
                                                 const char *artistid);
openTIDAL_ContentContainer *openTIDAL_GetArtistLink (openTIDAL_SessionContainer *session,
                                                     const char *artistid, const int limit,
                                                     const int offset);
openTIDAL_ContentContainer *openTIDAL_GetArtistMix (openTIDAL_SessionContainer *session,
                                                    const char *artistid);
openTIDAL_ContentContainer *openTIDAL_GetArtistTopTracks (openTIDAL_SessionContainer *session,
                                                          const char *artistid, const int limit,
                                                          const int offset);
openTIDAL_ContentContainer *openTIDAL_GetArtistVideos (openTIDAL_SessionContainer *session,
                                                       const char *artistid, const int limit,
                                                       const int offset);
openTIDAL_ContentContainer *openTIDAL_GetArtistAlbums (openTIDAL_SessionContainer *session,
                                                       const char *artistid, const int limit,
                                                       const int offset);

/* Track Service */
openTIDAL_ContentContainer *openTIDAL_GetTrack (openTIDAL_SessionContainer *session,
                                                const char *trackid);
openTIDAL_ContentContainer *openTIDAL_GetTrackMix (openTIDAL_SessionContainer *session,
                                                   const char *trackid);
openTIDAL_ContentContainer *openTIDAL_GetTrackStream (openTIDAL_SessionContainer *session,
                                                      const char *trackid);

/* Video Service */

openTIDAL_ContentContainer *openTIDAL_GetVideo (openTIDAL_SessionContainer *session,
                                                const char *videoid);
openTIDAL_ContentContainer *openTIDAL_GetVideoStream (openTIDAL_SessionContainer *session,
                                                      const char *videoid);

/* Mix Service */
openTIDAL_ContentContainer *openTIDAL_GetMixItems (openTIDAL_SessionContainer *session,
                                                   const char *mixid, const int limit,
                                                   const int offset);

/* Favorite Service */
openTIDAL_ContentContainer *openTIDAL_GetFavoriteAlbums (openTIDAL_SessionContainer *session,
                                                         const int limit, const int offset,
                                                         const char *order,
                                                         const char *orderDirection);
openTIDAL_ContentContainer *openTIDAL_GetFavoriteTracks (openTIDAL_SessionContainer *session,
                                                         const int limit, const int offset,
                                                         const char *order,
                                                         const char *orderDirection);
openTIDAL_ContentContainer *openTIDAL_GetFavoriteVideos (openTIDAL_SessionContainer *session,
                                                         const int limit, const int offset,
                                                         const char *order,
                                                         const char *orderDirection);
openTIDAL_ContentContainer *openTIDAL_GetFavoriteArtists (openTIDAL_SessionContainer *session,
                                                          const int limit, const int offset,
                                                          const char *order,
                                                          const char *orderDirection);
openTIDAL_ContentContainer *openTIDAL_GetFavoritePlaylists (openTIDAL_SessionContainer *session,
                                                            const int limit, const int offset,
                                                            const char *order,
                                                            const char *orderDirection);
const int openTIDAL_AddFavoriteAlbum (openTIDAL_SessionContainer *session, const char *albumId,
                                      const char *onArtifactNotFound);
const int openTIDAL_AddFavoriteAlbums (openTIDAL_SessionContainer *session, const char **albumIds,
                                       const int size, const char *onArtifactNotFound);
const int openTIDAL_DeleteFavoriteAlbum (openTIDAL_SessionContainer *session, const char *albumId);
const int openTIDAL_AddFavoriteTrack (openTIDAL_SessionContainer *session, const char *trackId,
                                      const char *onArtifactNotFound);
const int openTIDAL_AddFavoriteTracks (openTIDAL_SessionContainer *session, const char **trackIds,
                                       const int size, const char *onArtifactNotFound);
const int openTIDAL_DeleteFavoriteTrack (openTIDAL_SessionContainer *session, const char *trackId);
const int openTIDAL_AddFavoriteVideo (openTIDAL_SessionContainer *session, const char *videoId,
                                      const char *onArtifactNotFound);
const int openTIDAL_AddFavoriteVideos (openTIDAL_SessionContainer *session, const char **videoIds,
                                       const int size, const char *onArtifactNotFound);
const int openTIDAL_DeleteFavoriteVideo (openTIDAL_SessionContainer *session, const char *videoId);
const int openTIDAL_AddFavoriteArtist (openTIDAL_SessionContainer *session, const char *artistId,
                                       const char *onArtifactNotFound);
const int openTIDAL_AddFavoriteArtists (openTIDAL_SessionContainer *session, const char **artistIds,
                                        const int size, const char *onArtifactNotFound);
const int openTIDAL_DeleteFavoriteArtist (openTIDAL_SessionContainer *session,
                                          const char *artistId);
const int openTIDAL_AddFavoritePlaylist (openTIDAL_SessionContainer *session,
                                         const char *playlistid, const char *onArtifactNotFound);
const int openTIDAL_AddFavoritePlaylists (openTIDAL_SessionContainer *session,
                                          const char **playlistIds, const int size,
                                          const char *onArtifactNotFound);
const int openTIDAL_DeleteFavoritePlaylist (openTIDAL_SessionContainer *session,
                                            const char *playlistid);

/* Search Service */
openTIDAL_ContentContainer *openTIDAL_SearchAll (openTIDAL_SessionContainer *session, char *term,
                                                 const int limit);

/* Dynamic Page Service */
openTIDAL_ContentContainer *openTIDAL_GetPageHome (openTIDAL_SessionContainer *session);
openTIDAL_ContentContainer *openTIDAL_GetPageMixes (openTIDAL_SessionContainer *session);

struct openTIDAL_LoginCodeContainer {
    /* The Device Code is a 128bit uuid used to verify the session between
     * openTIDAL and the authorization server. OpenTIDAL uses the uuid
     * to request the access token from the authorization server.
     * OAuth2 Grant Type: urn:ietf:params:oauth:grant-type:device_code */
    char *deviceCode;
    /* The User Code is a 5 character string linked to the Device Code.
     * It's shown to the user and used to identify the session in a
     * web browser. */
    char *userCode;

    /* The URI the user should go to with the User Code in order to
     * sign in. */
    char *verificationUri;
    char *verificationUriComplete;

    /* The number of seconds before the Device Code and User Code expire. */
    size_t timeFrame;
    /* Calculated Unix timestamp */
    time_t expires_in;
    /* The number of seconds the client should wait between polling
     * requests. */
    int interval;
};

struct openTIDAL_LoginTokenContainer {
    /* Access Token issued by Authorization Server with a limited
     * lifespan. See timeFrame and expires_in. */
    char *access_token;
    /* After the Access Token expired the Refresh Token is used
     * to renew the Access Token. It has no expiry date.  */
    char *refresh_token;
    /* Always "Bearer" */
    char *token_type;

    /* The number of seconds before the Access Token expires. */
    size_t timeFrame;
    /* Calculated Unix timestamp */
    time_t expires_in;
    /* Internal error handling of token request */
    char *error;
    char *email;
    char *countryCode;
    char *fullName;
    char *firstName;
    char *lastName;
    char *nickname;
    char *username;
    char *imageId;
    char *userId;
    /* Only present if third-party oauth authentication is being
     * used or the TIDAL account is linked to a facebook account. */
    char *facebookUid;
    char *appleUid;
};

struct openTIDAL_UserContainer {
    char *id;
    char *username;
    char *firstName;
    char *lastName;
    char *email;
    char *countryCode;
    char *created;
    char *picture;
    int newsletter;
    int acceptedEULA;
    char *gender;
    char *dateOfBirth;
    /* Only present if third-party oauth authentication is being
     * used or the TIDAL account is linked to a facebook account. */
    char *facebookUid;
    char *appleUid;
};

struct openTIDAL_UserSubscriptionContainer {
    /* Subscription timeframe until next bill  */
    char *validUntil;
    char *accountStatus;
    /* TIDAL subscription type */
    char *type;
    /* The Offline Grace Period defines the time a
     * client with offline capabilities can be offline without
     * reauthenticating. This prevents the user from playing
     * offline content with an inactive subscription. OpenTIDAL
     * does not use TIDALs offline functionalities. */
    int offlineGracePeriod;
    char *highestSoundQuality;
    int premiumAccess;
    int canGetTrial;
    /* Family (Parent) or direct. */
    char *paymentType;
};

struct openTIDAL_PlaylistContainer {
    /* Applied limit by the api. */
    int limit;
    /* Applied offset by the api. */
    int offset;
    int totalNumberOfItems;
    int numberOfTracks;
    int numberOfVideos;
    char *uuid;
    char *title;
    char *description;
    int duration;
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
    /* Arraysize of artist arrays (artistId & artistName). */
    int subArraySize;
    char *id;
    char *title;
    int explicitItem;
    int duration;
    int allowStreaming;
    int streamReady;
    int popularity;
    int trackNumber;
    int volumeNumber;
    char **artistId;
    char **artistName;
    char *albumId;
    char *albumTitle;

    /* Only available if item is a track.
     * (isVideo: FALSE) */
    double replayGain;
    double peak;
    char *audioQuality;
    char *version;
    /* Album cover 128bit uuid.
     * 80x80, 160x160, 320x320, 640x640, 1280x1280 */
    char *cover;
    /* An animated album cover is a new TIDAL feature.
     * 80x80, 160x160, 320x320, 640x640, 1280x1280  */
    char *videoCover;

    /* Only available if item is a music video.
     * (isVideo: TRUE) */
    char *imageId;
    char *quality;

    int isVideo;
};

struct openTIDAL_AlbumContainer {
    int limit;
    int offset;
    int totalNumberOfItems;
    /* Arraysize of artist arrays (artistId & artistName). */
    int subArraySize;
    int explicitItem;
    int duration;
    int allowStreaming;
    int streamReady;
    char *copyright;
    int numberOfTracks;
    int numberOfVideos;
    int numberOfVolumes;
    int popularity;
    char **artistId;
    char **artistName;
    char *quality;
    char *version;
    char *id;
    char *title;
    char *cover;
    /* An animated album cover is a new TIDAL feature. */
    char *videoCover;
    char *releaseDate;
};

struct openTIDAL_ArtistContainer {
    int limit;
    int offset;
    int totalNumberOfItems;
    char *id;
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
    int arraySize;
    char **name;
    char **id;
    char **role;
};

struct openTIDAL_CreditsContainer {
    int limit;
    int offset;
    int arraySize;
    char **type;
    struct openTIDAL_ContributorContainer *contributors;
};

struct openTIDAL_StreamContainer {
    char *url;
    char *trackId;
    char *videoId;
    char *assetPresentation;
    char *audioQuality;
    char *audioMode;
    char *videoQuality;
    char *manifestMimeType;
    char *manifest;
    char *mimeType;
    char *codec;
    char *encryptionType;
};

struct openTIDAL_ModuleContainer {
    int arraySize;
    int arrayCapacity;

    char **moduleType;
    char **moduleTitle;
    char **modulePreTitle;
    int *arrayType;
    int *offset;
    int *total;

    int mixedListSize;
    int *mixedListTypes;
    int *mixedListOffset;
    int *mixedListTotal;
};

struct openTIDAL_IdContainer {
    int albumSize;
    char **albumIds;
    int trackSize;
    char **trackIds;
    int videoSize;
    char **videoIds;
    int playlistSize;
    char **playlistIds;
};

#ifdef __cplusplus
}
#endif

#endif // openTIDAL__h
