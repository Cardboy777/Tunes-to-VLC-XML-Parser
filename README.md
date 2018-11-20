#iTunes to VLC XML Parser/Converter#

This is a simple XML parser that I created so that I can easily move my playlists from iTunes over to VLC media player without having to go through the hassle of exporting playlists. If I have the time and the drive in the future I've considered turning this into a proper VLC plugin.

##Usage:##
Right now the program takes iTunes' 'iTunes Music Library.xml' file as standard input (via '<' in the command line) and outputs playlist files in the current directory as <Playlist Name>.xspf. These files can then be dragged into VLC's Media Library.

Right now the playlist order is NOT maintained in the conversion.

##Future plans for this project (If I continue working on it)##
-Add options for maintaining iTunes' default playlists (Podcasts, Videos, etc.)
-Look into media files stored in the cloud
-Convert project into a proper VLC plugin

**If you don't see the 'iTunes Music Library.xml'**, open iTunes go to Edit>Preferences>Advanced and check "Share iTunes Library XML with other applications"

##Things I have not tested for:##
-Compatibility on systems that are not Windows 10
-Cloud files & Store Purchases

Anyone interested in using and editing the code for themselves is free to do so. You don't have to link me I suppose, but I will be ** *very sad* ** if you don't.
