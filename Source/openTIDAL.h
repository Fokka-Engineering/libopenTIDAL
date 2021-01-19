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

#ifndef openTIDAL__h
#define openTIDAL__h

#include <time.h>

#ifdef __cplusplus
extern "C"
{
#endif

    enum OTStatus
    {
        SUCCESS,
        CURL_NOT_OK,
        BAD_REQUEST,
        UNAUTHORISED,
        EXPIRED_SESSION,
        ARTEFACT_NOT_FOUND,
        ASSET_NOT_READY_FOR_PLAYBACK,
        PRECONDITION_FAILED,
        REDIRECT,
        CLIENT_ERROR,
        SERVER_ERROR,
        MALLOC_ERROR,
        UNKNOWN_MANIFEST_MIMETYPE,
        UNKNOWN
    };

    enum OTQuality
    {
        AUDIO_LOW,
        AUDIO_HIGH,
        AUDIO_LOSSLESS,
        AUDIO_HI_RES,
        VIDEO_AUDIO_ONLY,
        VIDEO_LOW,
        VIDEO_MEDIUM,
        VIDEO_HIGH
    };

    enum OTTypes
    {
        SESSION_CONTAINER,
        CONTENT_CONTAINER,
        CONTENT_STREAM_CONTAINER
    };

    struct OTSessionContainer
    {
        char *persistentFileLocation;
        char *accessToken;
        char *refreshToken;
        char *clientId;
        char *previewClientId;
        char *clientSecret;
        char *scopes;
        char *baseUrl;
        char *authUrl;
        char *audioQuality;
        char *videoQuality;
        char *countryCode;
        char *userId;
        char *locale;
        char *deviceType;
        time_t expiresIn;
        time_t timeFrame;
        int restrictedMode;
        int verboseMode;
        struct OTJsonContainer *tree;
        struct OTJsonContainer *renewalTree;
        void *mainHttpHandle;
    };

    struct OTContentContainer
    {
        /* Returned status of the OTHttpConnector and additional
         * parsing of API specific error codes. */
        enum OTStatus status;
        struct OTJsonContainer *tree;
    };

    struct OTContentStreamContainer
    {
        enum OTStatus status;
        struct OTJsonContainer *tree;
        struct OTJsonContainer *manifest;
    };

    /* Manage an OTSession handle. */
    struct OTSessionContainer *OTSessionInit (void);
    int OTSessionClientPair (struct OTSessionContainer *const session, const char *const clientId,
                             const char *const clientSecret);
    int OTSessionLogin (struct OTSessionContainer *const session, const char *const location);
    /* disabled = 0, enabled = 1, debug = 2 */
    void OTSessionVerbose (struct OTSessionContainer *const session, const int enabled);
    void OTSessionChangeQuality (struct OTSessionContainer *const session, enum OTQuality quality);
    int OTSessionWriteChanges (const struct OTSessionContainer *session);
    enum OTStatus OTSessionRefresh (struct OTSessionContainer *session);
    void OTSessionCleanup (struct OTSessionContainer *session);

    int OTPersistentCreate (const struct OTSessionContainer *const session,
                            const char *const location);
    void OTDeallocContainer (void *container, enum OTTypes type);

    /* Create a http handle. Use one handle per thread.
     * Keep the handle(s) alive to utilise persistent connections.
     * DO NOT use one handle in multiple threads!
     * Cleanup the handles before exiting to avoid a memory leak.
     * These handles DO NOT refresh a TIDAL session. Only the
     * main handle refreshes it. */
    void *OTHttpThreadHandleCreate (void);
    void OTHttpThreadHandleCleanup (void *handle);

    /* SECTION: Service functions. */
    /* OAuth2 service.*/
    struct OTContentContainer *OTServiceGetDeviceCode (struct OTSessionContainer *session,
                                                       void *threadHandle);
    struct OTContentContainer *OTServiceGetBearerToken (struct OTSessionContainer *session,
                                                        const char *const deviceCode,
                                                        void *threadHandle);
    struct OTContentContainer *OTServiceRefreshBearerToken (struct OTSessionContainer *session,
                                                            const char *const refreshToken,
                                                            void *threadHandle);
    enum OTStatus OTServiceLogout (struct OTSessionContainer *session, void *threadHandle);

    /* v1 standard service.
     * Supported prefixes and suffixes (NULL = No Suffix):
     * albums (NULL, "items", "items/credits", "review")
     * tracks // videos (NULL, "contributors", "credits", "mix" (tracks only), "recommendations")
     * artists (NULL, "bio", "links", "mix", "toptracks", "videos")
     * playlists (NULL, "items", "recommendations/items")
     * mixes (items) */
    struct OTContentContainer *OTServiceGetStandard (struct OTSessionContainer *session,
                                                     const char *const prefix,
                                                     const char *const suffix, const char *const id,
                                                     const int limit, const int offset,
                                                     void *threadHandle);
    /* Favorite service.
     * (Suffixes: "ids", "albums", "tracks", "videos", "artists", "playlists", "mixes") */
    struct OTContentContainer *OTServiceGetFavorites (struct OTSessionContainer *session,
                                                      const char *const suffix, const int limit,
                                                      const int offset, const char *const order,
                                                      const char *const orderDirection,
                                                      void *threadHandle);
    /* DynamicPage service.
     * Id parameter only used if suffix ("album", "artist", "mix") */
    struct OTContentContainer *OTServiceGetPage (struct OTSessionContainer *session,
                                                 const char *const suffix, const char *const id,
                                                 const int limit, const int offset,
                                                 void *threadHandle);
    /* Search service.
     * (Suffixes: NULL (ALL), "albums", "tracks", "videos", "artists", "playlists", "top-hits") */
    struct OTContentContainer *OTServiceSearch (struct OTSessionContainer *session,
                                                const char *const suffix, char *query,
                                                const int limit, const int offset,
                                                void *threadHandle);
    /* Stream service.
     * Prefixes: "tracks", "videos" */
    struct OTContentStreamContainer *OTServiceGetStream (struct OTSessionContainer *session,
                                                         const char *const prefix,
                                                         const char *const id, const int isPreview,
                                                         void *threadHandle);

    /* Playlist manipulation service. */
    struct OTContentContainer *OTServiceCreatePlaylist (struct OTSessionContainer *session,
                                                        char *title, char *description,
                                                        void *threadHandle);
    /* HTTP entity-tag is used for playlist concurrency control. */
    char *OTServiceGetPlaylistEntityTag (struct OTSessionContainer *session, const char *const id,
                                         void *threadHandle);
    enum OTStatus OTServiceDeletePlaylistItem (struct OTSessionContainer *session,
                                               const char *const id, const int index,
                                               void *threadHandle);
    enum OTStatus OTServiceMovePlaylistItem (struct OTSessionContainer *session,
                                             const char *const id, const int index,
                                             const int toIndex, void *threadHandle);
    /* OnDupes: "FAIL", "SKIP", "ADD". OnArtifactNotFound: "FAIL", "SKIP". */
    enum OTStatus OTServiceAddPlaylistItem (struct OTSessionContainer *session,
                                            const char *const id, const char *itemId,
                                            const char *onArtifactNotFound, const char *onDupes,
                                            void *threadHandle);
    /* Parse an array of ids and the arraysize. */
    enum OTStatus OTServiceAddPlaylistItems (struct OTSessionContainer *session,
                                             const char *const id, const char **itemIds,
                                             const int size, const char *onArtifactNotFound,
                                             const char *onDupes, void *threadHandle);

    /* Favorite manipulation service. */
    enum OTStatus OTServiceDeleteFavorite (struct OTSessionContainer *session,
                                           const char *const suffix, const char *const id,
                                           void *threadHandle);
    enum OTStatus OTServiceAddFavorite (struct OTSessionContainer *session,
                                        const char *const suffix, const char *itemId,
                                        const char *onArtifactNotFound, void *threadHandle);
    enum OTStatus OTServiceAddFavorites (struct OTSessionContainer *session,
                                         const char *const suffix, const char **itemIds,
                                         const int size, const char *onArtifactNotFound,
                                         void *threadHandle);
    /* Feed activity service. */
    struct OTContentContainer *OTServiceGetFeedActivities (struct OTSessionContainer *session,
                                                           void *threadHandle);
    /* Check for new unseen activity. */
    struct OTContentContainer *
    OTServiceGetFeedActivityUnseenExists (struct OTSessionContainer *session, void *threadHandle);
    /* Mark unseen activity as seen. */
    enum OTStatus OTServiceFeedActivitySeen (struct OTSessionContainer *session,
                                             void *threadHandle);

    /* SECTION: OTJson parsing. */
    /* Returns the number of items in an array (or object). */
    int OTJsonGetArraySize (const struct OTJsonContainer *array);
    /* Retrieve item number "index" from array "array". Returns NULL if unsuccessful. */
    struct OTJsonContainer *OTJsonGetArrayItem (const struct OTJsonContainer *array, int index);
    struct OTJsonContainer *OTJsonGetObjectItem (const struct OTJsonContainer *const object,
                                                 const char *const string);
    int OTJsonHasObjectItem (const struct OTJsonContainer *object, const char *string);

/* Macro for iterating over an array or object */
#define OTJsonArrayForEach(element, array)                                                         \
    for (element = (array != NULL) ? (array)->child : NULL; element != NULL;                       \
         element = element->next)
    /* Check item type and return its value. */
    char *OTJsonGetStringValue (const struct OTJsonContainer *const item);
    char *OTJsonGetObjectItemStringValue (const struct OTJsonContainer *const object,
                                          const char *const string);
    double OTJsonGetNumberValue (const struct OTJsonContainer *const item);
    double OTJsonGetObjectItemNumberValue (const struct OTJsonContainer *const object,
                                           const char *const string);
    /* These functions check the type of an item. */
    int OTJsonIsInvalid (const struct OTJsonContainer *const item);
    int OTJsonIsFalse (const struct OTJsonContainer *const item);
    int OTJsonIsTrue (const struct OTJsonContainer *const item);
    int OTJsonIsBool (const struct OTJsonContainer *const item);
    int OTJsonIsNull (const struct OTJsonContainer *const item);
    int OTJsonIsNumber (const struct OTJsonContainer *const item);
    int OTJsonIsString (const struct OTJsonContainer *const item);
    int OTJsonIsArray (const struct OTJsonContainer *const item);
    int OTJsonIsObject (const struct OTJsonContainer *const item);
    int OTJsonIsRaw (const struct OTJsonContainer *const item);

    /* SECTION: OT container. */
    /* The OTJson structure. */
    struct OTJsonContainer
    {
        /* next/prev allow you to walk array/object chains. Alternatively, use
         * GetArraySize/GetArrayItem/GetObjectItem */
        struct OTJsonContainer *next;
        struct OTJsonContainer *prev;
        /* An array or object item will have a child pointer pointing to a chain of the items in the
         * array/object. */
        struct OTJsonContainer *child;

        /* The type of the item, as above. */
        int type;

        /* The item's string, if type == OTJsonString  and type == OTJsonRaw */
        char *valuestring;
        /* writing to valueint is DEPRECATED, use OTJsonSetNumberValue instead */
        int valueint;
        /* openTIDAL specific value. The parsed number in the char buffer (see parse_number)
         * is duplicated and allocated. This pointer points to the memory location of the number
         * ASCII String. The conversion to an integer is not suppressed. */
        char *valueintstring;
        /* The item's number, if type == OTJsonNumber */
        double valuedouble;

        /* The item's name string, if this item is the child of, or is in the list of subitems of an
         * object. */
        char *string;
    };

#ifdef __cplusplus
}
#endif

#endif /* openTIDAL__h */
