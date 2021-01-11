cmake_minimum_required(VERSION 3.10)

project(openTIDAL)

set (CMAKE_CXX_FLAGS "-fPIC -Wall -Werror -O2" )

include_directories( ${CMAKE_STAGING_PREFIX}/usr/local/include )
link_directories( ${CMAKE_STAGING_PREFIX}/usr/local/lib )

set ( src_openTIDAL
    Source/OTAlloc.c
    Source/OTBase64.c
    Source/OTDealloc.c
    Source/OTHttp.c
    Source/OTHttpParse.c
    Source/OTJson.c
    Source/OTPersistent.c
    Source/OTSession.c
    Source/OTSessionRefresh.c
    Source/OTString.c
    Source/OTUrlEncode.c
    Source/OTService/OTService.c
    Source/OTService/OTServiceStd.c
    Source/OTService/OTServiceAuth.c
    Source/OTService/OTServiceFeed.c
    Source/OTService/OTServicePlaylist.c
    Source/OTService/OTServiceFavorites.c
)

add_library( ${PROJECT_NAME} SHARED ${src_openTIDAL} )

target_include_directories( ${PROJECT_NAME} PRIVATE Source )

target_link_libraries( ${PROJECT_NAME} curl )

install (TARGETS openTIDAL DESTINATION lib)

install (FILES
    Source/openTIDAL.h
    DESTINATION include/openTIDAL
)
