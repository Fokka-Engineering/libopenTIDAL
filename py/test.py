from _openTIDAL_cffi import ffi, lib
import music_tag
import urllib.request
import os

location = "/Users/admin/Documents/Git/openTIDAL/bin/persistent.json"
downloadLocation = "/tmp/"

lib.init(location.encode("utf-8"))

print("Enter AlbumId: ")
albumId = input()

album = lib.get_album(int(albumId))
if (album.status == 1):
  print("Generating File Strukture...")
  folder = downloadLocation + ffi.string(album.title[0]).decode("utf-8")
  decodedCover = ffi.string(album.cover[0]).decode("utf-8").replace('-', '/')
  cover = "https://resources.tidal.com/images/" + decodedCover +  "/640x640.jpg"
  os.mkdir(folder)
  urllib.request.urlretrieve(cover, folder + "/cover.jpg")
  
  album_items = lib.get_album_items(int(albumId), 50, 0)
  if (album_items.status == 1):
    for i in range(album_items.arraySize):
      track_stream = lib.get_track_stream(album_items.id[i])
      track_title = ffi.string(album_items.title[i]).decode("utf-8")
      print("Retrieving Track: " + track_title)
      
      if (track_stream.status == 1):
        if (album_items.hasVersion[i] == 0):
          track_folder = folder + "/" + track_title + ".flac"
        else:
          track_folder = folder + "/" + track_title + " (" + ffi.string(album_items.version[i]).decode("utf-8")  + ").flac"
        urllib.request.urlretrieve(ffi.string(track_stream.url).decode("utf-8"), track_folder)
        
        if (album_items.hasVersion[i] == 0):
          f = music_tag.load_file(folder + "/" + track_title + ".flac")
          f['title'] = track_title
        else:
          f = music_tag.load_file(folder + "/" + track_title + " (" + ffi.string(album_items.version[i]).decode("utf-8") + ").flac")
          f['title'] = track_title + " (" + ffi.string(album_items.version[i]).decode("utf-8") + ")"
        
        f['album'] = ffi.string(album.title[0]).decode("utf-8")
        f['artist'] = ffi.string(album.artistName[0][0]).decode("utf-8")
        with open(folder + "/cover.jpg", 'rb') as img_in:
          f['artwork'] = img_in.read()
        f.save()
        print("Downloaded & Processed")


lib.cleanup()
