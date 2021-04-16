/*
    Copyright (c) 2020-2021 Hugo Melder and openTIDAL contributors

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to
   deal in the Software without restriction, including without limitation the
   rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
   sell copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
   IN THE SOFTWARE.
*/

/* OTService oauth polling tests
 */

#include <stdio.h>
#include <time.h>
#include <unistd.h>

#include "../../Source/openTIDAL.h"

const char *clientId = "OmDtrzFgyVVL6uW56OnFA2COiabqm";
const char *clientSecret = "zxen1r3pO0hgtOC7j6twMo9UAqngGrmRiWpV7QC1zJ8=";
const char *fileLocation = "/tmp/persist";

int main(void) {
    struct OTSessionContainer *session = NULL;
    struct OTContentContainer *deviceCode = NULL;
    struct OTContentContainer *bearerToken = NULL;
    char *deviceCodeUuid = NULL;
    time_t expiryTimestamp = 0;

    /* Create a new libopenTIDAL handle. */
    session = OTSessionInit();
    if (!session) goto end;

    /* Specify the clientId and clientSecret. */
    if (!(OTSessionClientPair(session, clientId, clientSecret) == 0)) goto end;

    /* Request a user- and device-code pair from the TIDAL auth server. */
    deviceCode = OTServiceGetDeviceCode(session, NULL);
    /* Check if OTContentContainer is allocated. */
    if (deviceCode) {
        if (deviceCode->status == SUCCESS) {
            /* The user_code is the code that the user should enter once they’ve
             * authenticated with the authorization server. */
            char *userCode =
                OTJsonGetObjectItemStringValue(deviceCode->tree, "userCode");
            /* The verification_uri is the URL that the user should navigate to
             * on another device. */
            char *verificationUriComplete = OTJsonGetObjectItemStringValue(
                deviceCode->tree, "verificationUriComplete");
            /* The timeframe in which the device code is valid. */
            int expired = (int)OTJsonGetObjectItemNumberValue(deviceCode->tree,
                                                              "expiresIn");
            /* Pointer to deviceCode for polling. */
            deviceCodeUuid =
                OTJsonGetObjectItemStringValue(deviceCode->tree, "deviceCode");
            /* Calculate timestamp. */
            expiryTimestamp = time(NULL) + expired;

            /* Check if the pointers are valid. */
            if (userCode && verificationUriComplete) {
                printf("UserCode: %s\n", userCode);
                printf("VerificationUri: https://%s\n",
                       verificationUriComplete);
            }
        } else
            goto end;
    } else
        goto end;

    /* If the deviceCode is valid, the client should attempt to acquire an
     * access token every few seconds (at a rate specified by interval). */
    while (time(NULL) <= expiryTimestamp) {
        /* Sleep to match the specified polling interval. */
        sleep(OTJsonGetObjectItemNumberValue(deviceCode->tree, "interval"));
        /* Request an access- and refresh-token. */
        bearerToken = OTServiceGetBearerToken(session, deviceCodeUuid, NULL);
        /* Check if OTContentContainer is allocated. */
        if (bearerToken) {
            if (bearerToken->status == SUCCESS) {
                /* Get user object. */
                struct OTJsonContainer *user =
                    OTJsonGetObjectItem(bearerToken->tree, "user");

                /* Point session values to auth data. */
                session->accessToken = OTJsonGetObjectItemStringValue(
                    bearerToken->tree, "access_token");
                session->refreshToken = OTJsonGetObjectItemStringValue(
                    bearerToken->tree, "refresh_token");
                session->userId =
                    OTJsonGetObjectItemStringValue(user, "userId");
                session->countryCode =
                    OTJsonGetObjectItemStringValue(user, "countryCode");
                session->timeFrame = (time_t)OTJsonGetObjectItemNumberValue(
                    bearerToken->tree, "expires_in");
                /* Calculate oAuth2 accessToken timestamp. */
                session->expiresIn = time(NULL) + session->timeFrame;

                /* Create the persistent config file. */
                OTPersistentCreate(session, fileLocation);
                break;
            }
        }
    }

end:
    /* Deallocate containers from heap. */
    OTDeallocContainer(deviceCode, CONTENT_CONTAINER);
    OTDeallocContainer(bearerToken, CONTENT_CONTAINER);
    /* Close the libopenTIDAL session handle. */
    OTSessionCleanup(session);
}
