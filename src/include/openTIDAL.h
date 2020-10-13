#include "cJSON.h"
#include "models.h"

extern char *client_id;
extern char *client_secret;
//extern char *refresh_token;
extern char *access_token;

extern char *countryCode;
extern char *soundQuality;
extern char *userId;

/* cURL Handles*/
curl_model curl_get(char *endpoint, char *data); /* BaseAPI cURL Handle */
curl_model curl_post(char *endpoint, char *data, char *optHeader); /* BaseAPI cURL Handle */
curl_model curl_delete(char *endpoint, char *data, char *optHeader); /* BaseAPI cURL Handle  */
curl_model curl_head(char *endpoint, char *data);
curl_model curl_post_auth(char *endpoint, char *data); /* AuthAPI URL & Different Handle with HTTP AUTH */
void curl_exit(); /* Close BaseAPI Handle */
void curl_exit_auth(); /* Close AuthAPI Handle */

/* String Concatenation */
char *url_cat(char *strOne, size_t id, char *strTwo, int appendCountryCode); /* Dynamic Endpoint Concatenation */
char *url_cat_str(char *strOne, char *id, char *strTwo); /* Dynamic Endpoint Concatenation Str-based  */
char *param_cat(char *limit, char *query, char *extra);
char *url_encode(char *str);

/* Parse Functions */
cJSON *json_parse(const char * input);
size_t parse_unauthorized(cJSON *input_json, size_t id);
size_t parse_notfound(cJSON *input_json, size_t id, char *uuid);
size_t parse_preconditionfailed(cJSON *input_json, size_t id, char *uuid);

items_model parse_items(cJSON *input_json, int version, int video);
items_model parse_tracks(cJSON *input_json);
playlist_model parse_playlist(cJSON *input_json, int version);
album_model parse_album(cJSON *input_json, int version);
artist_model parse_artist(cJSON *input_json, int version);
search_model parse_search(cJSON *input_json);

/* OAuth Endpoints  */
login_code_model login_create_code();
login_token_model login_create_token(char *device_code);
login_token_model login_refresh_token(char *refresh_token);


/* User Endpoints */
user_model get_user(size_t userid);
playlist_model get_user_playlist(size_t userid);
items_model get_user_album(size_t userid);
artist_model get_user_artist(size_t userid);
items_model get_user_tracks(size_t userid);
items_model get_user_videos(size_t userid);

playlist_model create_user_playlist(size_t userid, char *title, char *description);

int add_user_album(size_t userid, size_t albumid);
int add_user_artist(size_t userid, size_t artistid);
int add_user_playlist(size_t userid, char *playlistid);
int add_user_track(size_t userid, size_t trackid);
int add_user_video(size_t userid, size_t videoid);

int delete_user_album(size_t userid, size_t albumid);
int delete_user_artist(size_t userid, size_t artistid);
int delete_user_playlist(size_t userid, char *playlistid);
int delete_user_track(size_t userid, size_t trackid);
int delete_user_video(size_t userid, size_t videoid);

/* Playlist Endpoints */
playlist_model get_playlist(char *playlistid);
items_model get_playlist_items(char *playlistid);

char *get_playlist_etag(char *playlistid);
int delete_playlist(char *playlistid);
int delete_playlist_item(char *playlistid, size_t index, char *eTag);
int move_playlist_item(char *playlistid, size_t index, size_t toIndex, char *eTagHeader);
int add_playlist_item(char *playlistid, size_t trackid, char *onDupes, char *eTagHeader);
int add_playlist_items(char *playlistid, char *trackids, char *onDupes, char *eTagHeader);

/* Album Endpoints */
album_model get_album(size_t albumid);
items_model get_album_items(size_t albumid);

/* Artist Endpoints */
artist_model get_artist(size_t artistid);
artist_link_model get_artist_link(size_t artistid);
mix_model get_artist_mix(size_t artistid);
items_model get_artist_toptracks(size_t artistid);
items_model get_artist_videos(size_t artistid);
album_model get_artist_albums(size_t artistid);

/* Track Endpoints  */
tracks_contributor_model get_track_contributors(size_t trackid);
mix_model get_track_mix(size_t trackid);
stream_model get_track_streamUrl(size_t trackid);
items_model get_track(size_t trackid);

search_model get_search(char *term, char *limit);
