#### Models

```(type) variable```\
```(type) variable[]``` <- Array\
```(type) variable[][]``` <- 2D-Array

###### Login Code
```(int) status```

```(char[]) deviceCode```\
```(char[]) userCode```\
```(char[]) verificationUri```\
```(char[]) verificationUriComplete```\
```(time_t) expires_in```\
```(size_t) interval```

###### Login Token

```(int) status```

```(time_t) expires_in```\
```(char[]) email```\
```(char[]) countryCode```\
```(char[]) fullName```\
```(char[]) firstName```\
```(char[]) lastName```\
```(char[]) nickname```\
```(char[]) username```\
```(char[]) imageId```\
```(size_t) created```\
```(size_t) updated```\
```(size_t) userId```\
```(size_t) facebookUid```\
```(char[]) appleUid```

###### User

```(int) status```

```(size_t) id```\
```(char[]) username```\
```(char[]) firstName```\
```(char[]) lastName```\
```(char[]) email```\
```(char[]) countryCode```\
```(char[]) created```\
```(char[]) picture```\
```(size_t) newsletter```\
```(size_t) acceptedEULA```\
```(char[]) gender```\
```(char[]) dateOfBirth```\
```(size_t) facebookUid```\
```(char[]) appleUid```


###### User Subscription
```(int) status```

```(char[]) validUntil```\
```(char[]) accountStatus```\
```(char[]) type```\
```(size_t) offlineGracePeriod```\
```(char[]) highestSoundQuality```\
```(size_t) premiumAccess```\
```(size_t) canGetTrial```\
```(char[]) paymentType```

###### Artists
```(int) status```

```(size_t) limit```\
```(size_t) offset```\
```(size_t) totalNumberOfItems```\
```(size_t) arraySize```

```(size_t) id[]```\
```(char[]) name[]```\
```(char[]) picture[]```\
```(size_t) popularity[]```

###### Albums
```(int) status```

```(size_t) limit```\
```(size_t) offset```\
```(size_t) totalNumberOfItems```\
```(size_t) arraySize```

```(size_t) id[]```\
```(char[]) title[]```\
```(char[]) version[]```\
```(size_t) duration[]```\
```(size_t) popularity[]```\
```(size_t) numberOfTracks[]```\
```(size_t) numberOfVideos[]```\
```(size_t) numberOfVolumes[]```\
```(char[]) releaseDate[]```\
```(char[]) cover[]```\
```(char[]) videoCover[]```\
```(char[]) quality[]```\
```(char[]) copyright[]```\
```(size_t) explicitItem[]```\
```(size_t) allowStreaming[]```\
```(size_t) streamReady[]```

```(size_t) subArraySize[]```\
```(size_t) artistId[][]```\
```(char[]) artistName[][]```
 
###### Playlists
```(int) status```

```(size_t) limit```\
```(size_t) offset```\
```(size_t) totalNumberOfItems```\
```(size_t) arraySize```

```(char[]) uuid[]```\
```(char[]) title[]```\
```(char[]) description[]```\
```(size_t) duration[]```\
```(size_t) popularity[]```\
```(size_t) numberOfTracks[]```\
```(size_t) numberOfVideos[]```\
```(size_t) publicPlaylist[]```\
```(char[]) image[]```\
```(char[]) squareImage[]```\
```(char[]) lastUpdated[]```\
```(char[]) created[]```\
```(char[]) type[]```

###### Items
```(int) status```

```(size_t) limit```\
```(size_t) offset```\
```(size_t) totalNumberOfItems```\
```(size_t) arraySize```

```(size_t) id[]```\
```(char[]) title[]```\
```(char[]) version[]```\
```(size_t) duration[]```\
```(size_t) popularity[]```\
```(size_t) trackNumber[]```\
```(size_t) volumeNumber[]```\
```(size_t) explicitItem[]```\
```(size_t) allowStreaming[]```\
```(size_t) streamReady[]```\
```(double) replayGain[]```\
```(double) peak[]```\
```(char[]) imageId[]```\
```(char[]) audioQuality[]```\
```(char[]) quality[]```\
```(char[]) type[]```

```(size_t) albumId[]```\
```(char[]) albumTitle[]```\
```(char[]) cover[]```\
```(char[]) videoCover[]```

```(size_t) subArraySize[]```\
```(size_t) artistId[][]```\
```(char[]) videoCover[]```

```(size_t) subArraySize[]```\
```(size_t) artistId[][]```\
```(char[]) artistName[][]```

###### Pages Mix
```(int) status```

```(size_t) limit```\
```(size_t) offset```\
```(size_t) totalNumberOfItems```\
```(size_t) arraySize```

```(char[]) id[]```\
```(char[]) title[]```\
```(char[]) subTitle[]```\
```(char[]) mixType[]```

```(size_t) smallImageWidth[]```\
```(size_t) smallImageHeight[]```\
```(char[]) smallImageUrl[]```

```(size_t) mediumImageWidth[]```\
```(size_t) mediumImageHeight[]```\
```(char[]) mediumImageUrl[]```

```(size_t) largeImageWidth[]```\
```(size_t) largeImageHeight[]```\
```(char[]) largeImageUrl[]```

###### Mix
```(int) status```

```(char[]) id```

###### Artist Links
```(int) status```

```(size_t) limit```\
```(size_t) offset```\
```(size_t) totalNumberOfItems```\
```(size_t) arraySize```

```(char[]) url[]```\
```(char[]) siteName[]```\
```(char[]) source```

###### Contributors
```(int) status```

```(size_t) limit```\
```(size_t) offset```\
```(size_t) totalNumberOfItems```\
```(size_t) arraySize```

```(char[]) name[]```\
```(char[]) role[]```
###### Stream
```(int) status```

```(char[]) url```\
```(size_t) trackId```\
```(size_t) videoId```\
```(char[]) assetPresentation```\
```(char[]) audioQuality```\
```(char[]) audioMode```\
```(char[]) videoQuality```\
```(char[]) manifestMimeType```\
```(char[]) mimeType```\
```(char[]) codec```\
```(char[]) encryptionType```
