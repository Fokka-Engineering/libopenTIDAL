#include <stdio.h>
#include <stdlib.h>

#include "../src/include/openTIDAL.h"

int main()
{
    
    openTIDAL_Init("/Users/hugo/Documents/oT-config");

    openTIDAL_Verbose(1);

    printf("\nTrack Stream\n");
    
    openTIDAL resolve = openTIDAL_GetTrackStream(13479532);
    if (resolve.status == 1)
    {
        printf("%s\n", resolve.stream.url);
        printf("%zu\n", resolve.stream.trackId);
        printf("%s\n", resolve.stream.assetPresentation);
        printf("%s\n", resolve.stream.audioQuality);
        printf("%s\n", resolve.stream.audioMode);
        printf("%s\n", resolve.stream.manifestMimeType);
        printf("%s\n", resolve.stream.mimeType);
        printf("%s\n", resolve.stream.codec);
        printf("%s\n", resolve.stream.encryptionType);
    }

    printf("\nVideo Stream\n");

    openTIDAL resolveTwo = openTIDAL_GetVideoStream(58782326);
    if (resolveTwo.status == 1)
    {
        printf("%s\n", resolveTwo.stream.url);
        printf("%zu\n", resolveTwo.stream.videoId);
        printf("%s\n", resolveTwo.stream.assetPresentation);
        printf("%s\n", resolveTwo.stream.manifestMimeType);
        printf("%s\n", resolveTwo.stream.mimeType);
    }

    openTIDAL_StructDelete(&resolve);
    openTIDAL_StructDelete(&resolveTwo);
    openTIDAL_Cleanup();
    
    return 0;
}
