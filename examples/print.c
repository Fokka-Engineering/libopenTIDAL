#include <stdio.h>

#include "../src/include/openTIDAL.h"

void print_album(openTIDAL_AlbumModel *album)
{
    int i = 0;
                        
    printf("Title: %s\n", album->title);
    printf("Version: %s\n", album->version);
    printf("Id: %zu\n", album->id);
    printf("NumberOfTracks: %d\n", album->numberOfTracks);
    printf("NumberOfVideos: %d\n", album->numberOfVideos);
    printf("NumberOfVolumes: %d\n", album->numberOfVolumes);
    printf("Popularity: %d\n", album->popularity);
    printf("Duration: %zu\n", album->duration);
    printf("ReleaseDate: %s\n", album->releaseDate);
    printf("Explicit: %d\n", album->explicitItem);
    printf("AllowStreaming: %d\n", album->allowStreaming);
    printf("StreamReady: %d\n", album->streamReady);
    printf("Quality: %s\n", album->quality);
    printf("Cover: %s\n", album->cover);
    printf("VideoCover: %s\n", album->videoCover);
    printf("Copyright: %s\n", album->copyright);

    for (i = 0; i < album->subArraySize; i++)
    {
        printf("ArtistName #%d: %s\n", i, album->artistName[i]);
        printf("ArtistId #%d: %zu\n", i, album->artistId[i]);
    }
}

void print_item(openTIDAL_ItemsModel *item)
{
    int i = 0;

    printf("Title: %s\n", item->title);
    printf("Id: %zu\n", item->id);
    printf("Version: %s\n", item->version);
    printf("Duration: %zu\n", item->duration);
    printf("Popularity: %d\n", item->popularity);
    printf("TrackNumber: %d\n", item->trackNumber);
    printf("VolumeNumber: %d\n", item->volumeNumber);
    printf("Explicit: %d\n", item->explicitItem);
    printf("AllowStreaming: %d\n", item->allowStreaming);
    printf("StreamReady: %d\n", item->streamReady);
    printf("ReplayGain: %f\n", item->replayGain);
    printf("Peak: %f\n", item->peak);
    printf("AlbumId: %zu\n", item->albumId);
    printf("AlbumTitle: %s\n", item->albumTitle);
    printf("Cover: %s\n", item->cover);
    printf("VideoCover: %s\n", item->videoCover);
    printf("ImageId: %s\n", item->imageId);
    printf("AudioQuality: %s\n", item->audioQuality);
    printf("Quality: %s\n", item->quality);  
    printf("IsVideo: %d\n", item->isVideo);

    for (i = 0; i < item->subArraySize; i++)
    {
        printf("ArtistName #%d: %s\n", i, item->artistName[i]);
        printf("ArtistId #%d: %zu\n", i, item->artistId[i]);
    }
}

void print_playlist(openTIDAL_PlaylistModel *playlist)
{
    printf("Title: %s\n", playlist->title);
    printf("Uuid: %s\n", playlist->uuid);
    printf("Description: %s\n", playlist->description);
    printf("Duration: %zu\n", playlist->duration);
    printf("Popularity: %d\n", playlist->popularity);
    printf("NumberOfTracks: %d\n", playlist->numberOfTracks);
    printf("NumberOfVideos: %d\n", playlist->numberOfVideos);
    printf("PublicPlaylist: %d\n", playlist->publicPlaylist);
    printf("Image: %s\n", playlist->image);
    printf("SquareImage: %s\n", playlist->squareImage);
    printf("Created: %s\n", playlist->created);
    printf("LastUpdated: %s\n", playlist->lastUpdated);
    printf("Type: %s\n", playlist->type);
}

void print_mix(openTIDAL_MixModel *mix)
{
    printf("Title: %s\n", mix->title);
    printf("SubTitle: %s\n", mix->subTitle);
    printf("Id: %s\n", mix->id);
    
    printf("SmallImageWidth: %d\n", mix->smallImageWidth);
    printf("SmallImageHeight: %d\n", mix->smallImageHeight);
    printf("SmallImageUrl: %s\n", mix->smallImageUrl);

    printf("MediumImageWidth: %d\n", mix->mediumImageWidth);
    printf("MediumImageHeight: %d\n", mix->mediumImageHeight);
    printf("MediumImageUrl: %s\n", mix->mediumImageUrl);

    printf("LargeImageWidth: %d\n", mix->largeImageWidth);
    printf("LargeImageHeight: %d\n", mix->largeImageHeight);
    printf("LargeImageUrl: %s\n", mix->largeImageUrl);

    printf("MixType: %s\n", mix->mixType);
}


