#include "cJSON.h"
#include "models.h"
#include "json_models.h"

#ifndef openTIDAL__h
#define openTIDAL__h

#ifdef __cplusplus
extern "C"
{
#endif

extern char *client_id;
extern char *client_secret;
extern char *access_token;
extern char *refresh_token;
extern time_t expires_in;
extern size_t userId;
extern size_t demoEnabled;
extern char *countryCode;
extern char *audioQuality;
extern char *videoQuality;
extern char *persistentFile;

/* OAuth Endpoints */
login_code_model login_create_code();
login_token_model login_create_token(char *device_code);
login_token_model login_refresh_token(char *refresh_token);

void scan_persistent();
void create_persistent(char *username, char *audio_quality, char *video_quality);
void refresh_persistent();
void init(char *file_location);
void cleanup();

/* User Endpoints */
user_model get_user();
playlist_model get_user_playlist(size_t limit,
                              size_t offset, char *order, char *orderDirection);
album_model get_user_album(size_t limit,
                              size_t offset, char *order, char *orderDirection);
artist_model get_user_artist(size_t limit,
                              size_t offset, char *order, char *orderDirection);
items_model get_user_tracks(size_t limit,
                              size_t offset, char *order, char *orderDirection);
items_model get_user_videos(size_t limit,
                              size_t offset, char *order, char *orderDirection);

playlist_model create_user_playlist(char *title, char *description);
page_mix_model get_user_mixes();
int add_user_album(size_t albumid);
int add_user_artist(size_t artistid);
int add_user_playlist(char *playlistid);
int add_user_track(size_t trackid);
int add_user_video(size_t videoid);

int delete_user_album(size_t albumid);
int delete_user_artist(size_t artistid);
int delete_user_playlist(char *playlistid);
int delete_user_track(size_t trackid);
int delete_user_video(size_t videoid);

/* Playlist Endpoints */
playlist_model get_playlist(char *playlistid);
items_model get_playlist_items(char *playlistid, size_t limit, size_t offset);

char *get_playlist_etag(char *playlistid);
int delete_playlist(char *playlistid);
int delete_playlist_item(char *playlistid, size_t index, char *eTagHeader);
int move_playlist_item(char *playlistid, size_t index, size_t toIndex, char *eTagHeader);
int add_playlist_item(char *playlistid, size_t trackid, char *onDupes, char *eTagHeader);
int add_playlist_items(char *playlistid, char *trackids, char *onDupes, char *eTagHeader);

/* Album Endpoints */
album_model get_album(size_t albumid);
items_model get_album_items(size_t albumid, size_t limit, size_t offset);

/* Artist Endpoints */
artist_model get_artist(size_t artistid);
artist_link_model get_artist_link(size_t artistid, size_t limit, size_t offset);
mix_model get_artist_mix(size_t artistid);
items_model get_artist_toptracks(size_t artistid, size_t limit, size_t offset);
items_model get_artist_videos(size_t artistid, size_t limit, size_t offset);
album_model get_artist_albums(size_t artistid, size_t limit, size_t offset);

/* Track Endpoints  */
contributor_model get_track_contributors(size_t trackid, size_t limit, size_t offset);
mix_model get_track_mix(size_t trackid);
stream_model get_track_stream(size_t trackid);
items_model get_track(size_t trackid);

/* Video Endpoint  */
items_model get_video(size_t videoid);
contributor_model get_video_contributors(size_t videoid, size_t limit, size_t offset);
stream_model get_video_stream(size_t videoid);
/* Mix Endpoint  */
items_model get_mix_items(char *mixid);

search_model get_search(char *term, size_t limit);

#ifdef __cplusplus
}
#endif

#endif
