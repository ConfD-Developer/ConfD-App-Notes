#!/usr/bin/env python
# This example demonstrates how to use generated swagger python client
# (against ConfD RESTCONF northbound interface)
#
# This script is part of 'ConfD RESTCONF and Swagger' application note.
#
# (C) 2018 Tail-f Systems
# Permission to use this code as a starting point hereby granted

from __future__ import print_function
import swagger_client
from swagger_client import DataExampleJukeboxJukeboxLibraryArtistArtistName, \
    DataExampleJukeboxJukeboxLibraryArtistArtistNameAlbumAlbumName, \
    DataExampleJukeboxJukeboxLibraryArtistArtistNameAlbumAlbumNameSongSongName
from swagger_client.rest import ApiException
from pprint import pprint

# Configure HTTP basic authorization: basicAuth
configuration = swagger_client.Configuration()
configuration.username = 'admin'
configuration.password = 'admin'
# Configure debug level, if needed
configuration.debug = False

try:
    api_instance = swagger_client.DataApi(
        swagger_client.ApiClient(configuration))

    print("---- GET library")
    api_response = api_instance.data_example_jukebox_jukebox_library_get()
    pprint(api_response)

    # Not working. When debug is enabled, one can see the response is correctly received,
    # but not correctly parsed and returned.
    print("---- GET artist - not working correctly")
    configuration.debug = True
    api_response = api_instance. \
        data_example_jukebox_jukebox_library_artist_artist_name_get(
        "Nick Cave and the Bad Seeds")
    pprint(api_response)
    configuration.debug = False

    print("---- GET album name")
    api_response = api_instance. \
        data_example_jukebox_jukebox_library_artist_artist_name_album_album_name_genre_get(
        "Nick Cave and the Bad Seeds",
        "Tender Prey")
    pprint(api_response)

    print("---- PUT artist")
    artist = DataExampleJukeboxJukeboxLibraryArtistArtistName({
        "name": "Karel Gott",
        "album": [
            {
                "name": "Zpiva Karel Gott",
                "genre": "example-jukebox:pop",
                "year": 2017,
                "song": [
                    {
                        "name": "Maria",
                        "location": "Maria.mp3",
                        "format": "mp3",
                        "length": 174
                    }
                ]
            }
        ]
    })
    api_instance.data_example_jukebox_jukebox_library_artist_artist_name_put(
        "Karel Gott", artist)

    print("---- PATCH artist")
    artist = DataExampleJukeboxJukeboxLibraryArtistArtistName({
        "name": "Karel Gott",
        "album": [
            {
                "name": "Zpiva Karel Gott",
                "genre": "example-jukebox:pop",
                "year": 2017,
                "song": [
                    {
                        "name": "That Old Lucky Sun",
                        "location": "That Old Lucky Sun.mp3",
                        "format": "mp3",
                        "length": 160
                    }
                ]
            }
        ]
    })
    api_instance. \
        data_example_jukebox_jukebox_library_artist_artist_name_patch(
        "Karel Gott", artist)

    print("---- PUT artist song")
    song = DataExampleJukeboxJukeboxLibraryArtistArtistNameAlbumAlbumNameSongSongName(
        {
            "name": "Jezebel",
            "location": "Jezebel.mp3",
            "format": "mp3",
            "length": 169
        })
    api_instance. \
        data_example_jukebox_jukebox_library_artist_artist_name_album_album_name_song_song_name_put(
        "Karel Gott", "Zpiva Karel Gott", "Jezebel", song)

    print("---- GET library")
    api_response = api_instance.data_example_jukebox_jukebox_library_get()
    pprint(api_response)

    print("---- DELETE artist song")
    api_instance. \
        data_example_jukebox_jukebox_library_artist_artist_name_album_album_name_song_song_name_delete(
        "Karel Gott", "Zpiva Karel Gott", "Jezebel")

    print("---- GET library")
    api_response = api_instance.data_example_jukebox_jukebox_library_get()
    pprint(api_response)

except ApiException as e:
    print(
        "Exception when calling DataApi->data_example_jukebox_jukebox_get: %s\n" % e)
