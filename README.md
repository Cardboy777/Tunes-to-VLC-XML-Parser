# iTunes to VLC XML Parser/Converter

This is a simple XML parser that I created so that I can easily move my playlists from iTunes over to VLC media player without having to go through the hassle of exporting playlists. If I have the time and the motivation in the future I will consider turning this into a proper VLC plugin.

## Usage:

The program takes 'iTunes Music Library.xml' file as standard input (via '<' in the command line) and outputs playlist files in the current directory as <Playlist Name>.xspf. These files can then be used as playlists in VLC's Media Library.

Currently, the playlist order is NOT maintained in the conversion.

### **If you don't see 'iTunes Music Library.xml'**

Open iTunes. Go to Edit->Preferences->Advanced and make sure "Share iTunes Library XML with other applications" is checked. The file is usually in the [YourUser]/Music/iTunes/ directory.

## Future plans for this project (If I continue working on it)

- Add options for maintaining iTunes' default playlists (Podcasts, Videos, etc.)
- Look into media files stored in the cloud
- Convert project into a proper VLC plugin

## Scenarios I have not tested:
- Usage on systems that are not Windows 10
- Cloud files & Apple Store Purchases

***This tool was designed to be used for my locally-stored media files and therefore will most likely not support other use cases. Additionally, this program was created overnight and is not thoroughly tested. Your results may vary.***


Anyone interested in using and editing the code for themselves is free to do so.  
You don't have to link me I suppose, but I will be ***very sad*** if you don't. 😢
