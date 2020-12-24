#include <stdio.h>
#include <stdlib.h>

#include "../src/include/openTIDAL.h"


void print_album(openTIDAL_AlbumModel *album);
void print_playlist(openTIDAL_PlaylistModel *playlist);
void print_mix(openTIDAL_MixModel *mix);

void recentlyPlayed(openTIDAL *o);
void mixesForYou(openTIDAL *o);
void radioStationsForYou(openTIDAL *o);
void yourHistory(openTIDAL *o);
void featuredPlaylists(openTIDAL *o);
void becauseYouListenedTo(openTIDAL *o);

int main()
{
    /* Initialise openTIDAL*/
    openTIDAL_Init("/Users/hugo/Documents/oT-config");

    /* Print errors 
     * Loglevel:
     *  0 = Disabled
     *  1 = Error
     *  2 = Debug
     *  3 = Trace*/
    openTIDAL_Verbose(1);

    /* Request personalized homepage */
    openTIDAL home = openTIDAL_GetHome();
    if (1) /* Check if request succeded. (Docs: StatusCodes) */
    {
        recentlyPlayed(&home);
        //mixesForYou(&home);
        //radioStationsForYou(&home);
        //yourHistory(&home);
        //featuredPlaylists(&home);
        //becauseYouListenedTo(&home);
    }

    /* Deallocate structure from heap after use */
    openTIDAL_StructDelete(&home);

    /* */
    openTIDAL_Cleanup();
}

void recentlyPlayed(openTIDAL *o)
{
    /* Total number of items in module */
    int total = o->home.recentlyPlayed_Total;
    int i = 0;

    printf("\nModule: Recently Played (%d Items)\n", total);
    for (i = 0; i < total; i++)
    {
        /* openTIDAL array identifier to determine type of item */
        int type = o->home.recentlyPlayed_ArrayTypes[i];
        /* after checking for the type specify the correct position
         * in the array. */
        int position = o->home.recentlyPlayed_ArrayPosition[i];

        /* */
        switch(type)
        {
            case 0:
                printf("#%d is an album @ position %d\n", i, position);
                /* Pointer to the correct item at the specified position */
                print_album(&o->albums[position]);
                printf("\n");
                break;

            case 1:
                printf("#%d is a video or a track @ position %d\n", i, position);
                printf("\n");
                break;

            case 2:
                printf("#%d is an artist @ position %d\n", i, position);
                printf("\n");
                break;

            case 3:
                printf("#%d is a playlist @ position %d\n", i, position);
                print_playlist(&o->playlists[position]);
                printf("\n");
                break;

            case 4:
                printf("#%d is a mix @ position %d\n", i, position);
                print_mix(&o->mixes[position]);
                printf("\n");
                break;
        }

    }

}

void mixesForYou(openTIDAL *o)
{
    /* offset in the mixes array */
    int start = o->home.mixesForYou_Start;
    /* total number of items in the module */
    int total = o->home.mixesForYou_Total;
    int i = 0;

    printf("\nModule: Mixes For You (%d Items)\n", total);
    for (i = 0; i < total; ++i)
    {
        printf("\nMix #%d\n", i);
        /* get mix */
        print_mix(&o->mixes[i + start]);
    }
    printf("\n");
}

void radioStationsForYou(openTIDAL *o)
{
    int numberOfModules = o->home.radioStationsForYou_ArraySize;
    int i;
    printf("Module: Radiostations For You (NumberOfModules: %d)\n", numberOfModules);

    for (i = 0; i < numberOfModules; ++i)
    {
        int total = o->home.radioStationsForYou_Total[i];
        int offset = o->home.radioStationsForYou_Start[i];
        int index = 0;

        printf("\nModule: Radio Stations For You #%d (%d Items)\n", i, total);
        for (index = 0; index < total; ++index)
        {
            print_mix(&o->mixes[index + offset]);
            printf("\n");
        }
        printf("\n");
    }
}

void yourHistory(openTIDAL *o)
{
    int start = o->home.yourHistory_Start;
    int total = o->home.yourHistory_Total;
    int i = 0;

    printf("\nModule: Your History (%d Items)\n", total);
    for (i = 0; i < total; ++i)
    {
        printf("\nMix #%d\n", i);
        print_mix(&o->mixes[i + start]);
    }
    printf("\n");
}

void featuredPlaylists(openTIDAL *o)
{
    int start = o->home.featuredPlaylists_Start;
    int total = o->home.featuredPlaylists_Total;
    int i = 0;

    printf("\nModule: Featured Playlists    (%d Items)\n", total);
    for (i = 0; i < total; ++i)
    {
        printf("\nPlaylist #%d\n", i);
        print_playlist(&o->playlists[i + start]);
    }
    printf("\n");
}

void becauseYouListenedTo(openTIDAL *o)
{
    int numberOfModules = o->home.becauseYouListenedTo_ArraySize;
    int i;
    printf("Module: Because You Listened To (NumberOfModules: %d)\n", numberOfModules);

    for (i = 0; i < numberOfModules; ++i)
    {
        int total = o->home.becauseYouListenedTo_Total[i];
        int offset = o->home.becauseYouListenedTo_Start[i];
        char *title = o->home.becauseYouListenedTo_Title[i];
        int index = 0;

        printf("\nModule: Because You Listened To %s #%d (%d Items)\n", title, i, total);
        for (index = 0; index < total; ++index)
        {
            print_album(&o->albums[index + offset]);
            printf("\n");
        }
        printf("\n");
    }
}

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

