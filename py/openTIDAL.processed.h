/* Models */
typedef long time_t;
typedef struct curl_models
{
  int status;
  size_t responseCode;
  char *body;
  char *header;
} curl_model;

typedef struct login_code_models
{
  int status;
  char deviceCode[37];
  char userCode[6];
  time_t timestamp;
} login_code_model;

typedef struct login_token_models
{
  int status;
  time_t timestamp;
  size_t expires_in;
  char access_token[1000];
  char refresh_token[1000];
  char countryCode[3];
  char username[50];
  size_t userId;
  size_t appleUid;
  size_t facebookUid;
  char error[50];
} login_token_model;

typedef struct user_models
{
  int status;
  size_t id;
  size_t hasFirstName;
  size_t hasLastName;
  size_t hasPicture;
  size_t hasGender;
  size_t hasDateOfBirth;
  char username[50];
  char firstName[25];
  char lastName[25];
  char email[50];
  char countryCode[3];
  char created[29];
  char picture[37];
  char gender[2];
  char dateOfBirth[11];
} user_model;

typedef struct playlist_models
{
  int status;
  size_t limit;
  size_t offset;
  size_t arraySize;
  size_t totalNumberOfItems;
  size_t numberOfTracks[100];
  size_t numberOfVideos[100];
  char uuid[100][37];
  char title[100][80];
  size_t hasDescription[100];
  char description[100][1024];
  size_t duration[100];
  char lastUpdated[100][29];
  char created[100][29];
  char image[100][37];
  char squareImage[100][37];
  size_t popularity[100];
} playlist_model;

typedef struct items_models
{
  int status;
  size_t limit;
  size_t offset;
  size_t totalNumberOfItems;
  size_t arraySize;
  size_t isVideo[100];
  size_t subArraySize[100];
  size_t id[100];
  char title[100][80];
  int explicit[100];
  size_t duration[100];
  int allowStreaming[100];
  int streamReady[100];
  size_t popularity[100];
  size_t trackNumber[100];
  size_t volumeNumber[100];
  size_t hasVersion[100];
  char version[100][50];
  size_t artistId[100][6];
  char artistName[100][6][50];
  size_t albumId[100];
  char albumTitle[100][80];
  char cover[100][37];
  char quality[100][20];
  size_t hasReleaseDate[100];
  char releaseDate[100][11];
  char type[100][12];
} items_model;

typedef struct albums_models
{
  int status;
  size_t limit;
  size_t offset;
  size_t totalNumberOfItems;
  size_t arraySize;
  size_t subArraySize[100];
  size_t explicit[100];
  size_t duration[100];
  size_t allowStreaming[100];
  size_t streamReady[100];
  char copyright[100][50];
  size_t numberOfTracks[100];
  size_t numberOfVideos[100];
  size_t numberOfVolumes[100];
  size_t popularity[100];
  size_t artistId[100][6];
  char artistName[100][6][50];
  char quality[100][20];
  size_t id[100];
  char title[100][80];
  char cover[100][37];
  char videoCover[100][37];
  char releaseDate[100][11];
} album_model;

typedef struct items_credits_models
{
  int status;
  size_t arraySize;
  int subArraySize;
  int offset;
  char type[100][25];
  char name[100][30];
  size_t id[100];

} items_credits_model;

typedef struct album_credits_models
{
  int status;
  size_t arraySize;
  char type[50][30];
} album_credits_model;

typedef struct artists_models
{
  int status;
  size_t limit;
  size_t offset;
  size_t totalNumberOfItems;
  size_t arraySize;
  size_t id[100];
  char name[100][50];
  size_t hasPicture[100];
  char picture[100][37];
  size_t popularity[100];
} artist_model;

typedef struct artist_links_models
{
  int status;
  size_t limit;
  size_t offset;
  size_t arraySize;
  size_t totalNumberOfItems;
  char url[20][80];
  char siteName[20][50];
  char source[25];
} artist_link_model;

typedef struct mix_models
{
  int status;
  char id[32];
} mix_model;

typedef struct pages_mix_models
{
  int status;
  size_t arraySize;
  size_t limit;
  size_t offset;
  size_t totalNumberOfItems;
  char id[20][32];
  char title[20][50];
  char subTitle[20][100];
  char smallImageUrl[50][400];
  char mediumImageUrl[20][400];
  char largeImageUrl[20][400];
} page_mix_model;

typedef struct contributors_models
{
  int status;
  size_t limit;
  size_t offset;
  size_t totalNumberOfItems;
  size_t arraySize;
  char name[50][50];
  char role[50][25];
} contributor_model;

typedef struct search_models
{
  int status;
  char topHitType[10];
  int isTopAlbum;
  int isTopArtist;
  int isTopTrack;
  int isTopPlaylist;
  int isTopVideo;
  album_model albums;
  artist_model artists;
  items_model tracks;
  playlist_model playlists;
  items_model videos;
  album_model topAlbum;
  artist_model topArtist;
  items_model topTrack;
  playlist_model topPlaylist;
  items_model topVideo;
} search_model;

typedef struct stream_models
{
  int status;
  char url[512];
  size_t trackId;
  size_t videoId;
  char assetPresentation[10];
  char audioQuality[20];
  char audioMode[20];
  char videoQuality[20];
  char mimeType[30];
  char codec[10];
} stream_model;
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

