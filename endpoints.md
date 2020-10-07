#### Authentication
TIDAL uses OAuth2 for authorization. After authorization (Email or 3rd-Party) the response code is then exchanged for the accessToken (authorization: Bearier xxx in Header). The accessToken is only valid for one day and has to be updated.

TIDAL has also implemented a DeviceFlow for devices with limited input (like TVs or NetworkStreamer) and applications that are not web-based (reCaptcha v3 in the webflow). Here the device requests a 4 digit Authcode and a DeviceCode. The user then links the AuthCode to his account and the DeviceCode can then be exchanged for an accessToken.
#### Deprecation
TIDAL used to allow direct Username and Password logins with a SessionId exchange. The endpoint still exists but all new client_ids (Tokens) disabled direct session based logins. Still there are a few working tokens for legacy app support (old iOS and FireTV apps). Just recently TIDAL disabled the client_id for the legacy android app.
#### BaseURLs
Tidal uses three different API BaseURLs:

* listen.tidal.com/v1 (Used in the WebPlayer)

* api.tidalhifi.com/v1 (Same as the third)

* api.tidal.com/v1 (Used in the Tidal Android APP)

There's no difference between all of them.

#### AuthURLs

* login.tidal.com (Webflow)

* auth.tidal.com/v1 (Codeflow & DeviceFlow)

#### Background

I've decompiled various TIDAL Android App Versions and debundled the Browser JS App to gain some knowledge about the endpoints, general API functions and the authflow. The Browser JS App is boring because all streams are DRM encrypted and in a MPEG DASH stream.

#### Endpoints

###### Artists

[```GET artists/{id}```][+]()

[```GET artists/{id}/bio```][+]()

[```GET artists/{id}/links```][+]()

[```GET artists/{id}/mix```][+]()

[```GET artists/{id}/toptracks```][+]()

[```GET artists/{id}/videos```][+]()

[```GET artists/{id}/albums```][+]()
###### Albums

[```GET albums/{id}```][+]()

[```GET albums/{id}/credits```][+]()

[```GET albums/{id}/items```][+]()

[```GET albums/{id}/items/credits```][+]()

[```GET albums/{id}/review```][+]()
###### Playlists

[```POST playlists/{uuid}/items```][+]()

[```DELETE playlists/{uuid}```][+]()

[```DELETE playlists/{uuid}/items/{indices}```][+]()

[```GET playlists/{uuid}```][+]()

[```GET playlists/{uuid}/items```][+]()

[```GET playlists/{uuid}/recommendations/items```][+]()

[```POST playlists/{uuid}/items/{indices}```][+]()

[```POST playlists/{uuid}/items/{index}/replace```][+]()

[```POST playlists/{uuid}```][+]()
###### Tracks

[```GET tracks/{id}/contributors```][+]()

[```GET tracks/{id}/mix```][+]()

[```GET tracks/{id}/streamUrl```][+]()

[```GET tracks/{id}/playbackinfopostpaywall```][+]()

[```GET tracks/{id}```][+]()
###### Videos

[```GET videos/{id}/contributors```][+]()

[```GET videos/{id}/streamUrl```][+]()

[```GET videos/{id}/playbackinfopostpaywall```][+]()

[```GET videos/{id}```][+]()
###### Mixes

[```GET mixes/{mixId}/items```][+]()
###### User
[```POST users/{userId}/facebook```][+]()

[```POST users/{userId}/favorites/albums```][+]()

[```POST users/{userId}/favorites/artists```][+]()

[```POST users/{userId}/favorites/playlists```][+]()

[```POST users/{userId}/favorites/tracks```][+]()

[```POST users/{userId}/favorites/videos```][+]()

[```POST users/{userId}/clients/{clientId}/offline/albums```][+]()

[```POST users/{userId}/clients/{clientId}/offline/playlists```][+]()

[```POST users/{userId}/playlists```][+]()

[```GET users/facebook/acceptedEULA```][+]()

[```GET users/{userId}/clients```][+]()

[```GET users/{userId}/favorites/ids```][+]()

[```GET users/{userId}/favorites/albums```][+]()

[```GET users/{userId}/favorites/artists```][+]()

[```GET users/{userId}/playlistsAndFavoritePlaylists```][+]()

[```GET users/{userId}/favorites/playlists```][+]()

[```GET users/{userId}/favorites/tracks```][+]()

[```GET users/{userId}/favorites/videos```][+]()

[```GET users/{userId}/handovertoken```][+]()

[```GET users/{userId}/clients/{clientId}/offline/albums```][+]()

[```GET users/{userId}/clients/{clientId}/offline/playlists```][+]()

[```DELETE users/{userId}/clients/{clientId}/offline/all```][+]()

[```DELETE users/{userId}/facebook```][+]()

[```DELETE users/{userId}/favorites/albums/{albumId}```][+]()

[```DELETE users/{userId}/favorites/artists/{artistId}```][+]()

[```DELETE users/{userId}/favorites/playlists/{uuid}```][+]()

[```DELETE users/{userId}/favorites/tracks/{trackId}```][+]()

[```DELETE users/{userId}/favorites/videos/{videoId}```][+]()

[```DELETE playlist/{uuid}```][+]()

[```DELETE users/{userId}/clients/{clientId}/offline/albums/{albumId}```][+]()

[```DELETE users/{userId}/clients/{clientId}/offline/playlists/{playlistUuid}```][+]()

[```POST users/{userId}/subscription/updateWithAmazon```][+]()

[```POST users/{userId}/image```][+]()
###### Pages
[```GET pages/home```][+]()

[```GET pages/explore```][+]()

[```GET pages/videos```][+]()

[```GET pages/my_collection_my_mixes```][+]()
###### Search
[```GET search/albums```][+]()

[```GET search/artists```][+]()

[```GET search/playlists```][+]()

[```GET search```][+]()

[```GET search/tracks```][+]()

[```GET search/videos```][+]()
###### Widevine

[```POST drm/licenses/widevine```][+]()

[```POST streamingprivileges/tokens```][+]()

###### Authentication
[```POST oauth2/token```][+]()

[```POST oauth2/device_authorization```][+]()

[```GET users/{userId}/loginToken```][+]()

[```GET username/password```][+]()

###### Miscellaneous
[```GET country```][+]()
