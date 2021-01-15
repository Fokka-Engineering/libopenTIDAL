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

/* openTIDAL session refresh service
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "OTJson.h"
#include "openTIDAL.h"

enum OTStatus
OTSessionRefresh (struct OTSessionContainer *session)
{
    enum OTStatus status = SUCCESS;
    time_t currentTimeStamp = time (NULL);
    double diffTimeStamp;
    struct OTContentContainer *req;

    if (session->verboseMode)
        printf ("* Compare OAuth2 accessToken timestamp with SystemRTC timestamp...\n");

    if (session->restrictedMode)
        goto end;

    /* Check if OAuth2 Bearer timestamp is in the future. */
    if (currentTimeStamp > session->expiresIn)
        diffTimeStamp = difftime (currentTimeStamp, session->expiresIn);
    else
        diffTimeStamp = difftime (session->expiresIn, currentTimeStamp);

    /* If OAuth Bearer timestamp is not in the future with a difference of more than five minutes.
     */
    if (!(currentTimeStamp < session->expiresIn && (time_t)diffTimeStamp >= 300))
        {
            if (session->verboseMode)
                printf ("* Performing OTServiceRefreshBearerToken...\n");
            req = OTServiceRefreshBearerToken (session, session->refreshToken, NULL);
            if (req)
                {
                    if (req->status == SUCCESS)
                        {
                            struct OTJsonContainer *accessToken = NULL;
                            struct OTJsonContainer *timeFrame = NULL;
                            char *accessTokenString = NULL;
                            time_t timeFrameNumber = 0;
                            /* Cleanup previous request. */
                            OTJsonDelete (session->renewalTree);
                            session->renewalTree = req->tree;
                            free (req);

                            accessToken = OTJsonGetObjectItem (req->tree, "access_token");
                            accessTokenString = OTJsonGetStringValue (accessToken);
                            timeFrame = OTJsonGetObjectItem (req->tree, "expires_in");
                            timeFrameNumber = OTJsonGetNumberValue (timeFrame);
                            if (accessToken && timeFrameNumber != 0)
                                {
                                    status = SUCCESS;
                                    session->accessToken = accessTokenString;
                                    session->expiresIn = currentTimeStamp + timeFrameNumber;
                                    /* Update config. */
                                    OTPersistentCreate (session, session->persistentFileLocation);
                                }
                            else
                                status = MALLOC_ERROR;
                        }
                    else
                        {
                            enum OTTypes type = CONTENT_CONTAINER;
                            status = req->status;
                            OTDeallocContainer (req, type);
                        }
                }
            else
                status = MALLOC_ERROR;
        }
end:
    return status;
}
