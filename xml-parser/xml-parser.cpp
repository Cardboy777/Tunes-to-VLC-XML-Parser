
#include "xml-parser.h"
#include <ctime>
#include <map>
#include <iterator>
#include <vector>
#include <sstream>
#include <boost/algorithm/string/trim.hpp>
#include <boost/algorithm/string.hpp>

/*
	Goals: 
	1) Create a header file up to the Tracks key
	2) Parse through song tracks, save song data + key in song object. Hash the song id to the song object in a hash table
	3) Begin parsing playlist data
		a) Upon the start of a new playlist, add header to new file
		b) export songs from song id
		c) repeat b) until the end of the playlist is reached
		e) Export closing tags and close file
		f) repeat a-e until finished
	Extras:
		Don't create files out of certain playlists ("Downloads, Movies, etc")
		Each playlist file should be named the name of the playlist
*/

const char * HEADER = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<playlist xmlns=\"http://xspf.org/ns/0/\" xmlns:vlc=\"http://www.videolan.org/vlc/playlist/ns/0/\" version=\"1\">\n";
const char * FOOTER = "\t</trackList>\n</playlist>";

std::vector<std::string> defaultPlaylistNames = {"Audiobooks", "Downloaded", "Library", "Movies", "Music", "Podcasts", "TV Shows"};

void printUsage(){
	printf("Usage: xml-parser < <iTunes Music Library.xml> \n");
}

void printHeader(std::ofstream& f, std::string playlistName){
	f << HEADER << "\t<title>" << playlistName << "</title>\n\t<trackList>" << std::endl;
}

void printFooter(std::ofstream& f){
	f << FOOTER << std::endl;
}

bool isSkippable(std::string name){
	if(std::find(defaultPlaylistNames.begin(), defaultPlaylistNames.end(), name) != defaultPlaylistNames.end())
		return true;
	return false;
}

std::string generateTabSpace(int num){
	std::stringstream ss;
	for(int i = 0; i < num; i++){
		ss << "\t";
	}
	return ss.str();
}
// Credits to Raúl Roa for this solution
std::string extractString(std::string source, std::string start, std::string end){
	std::size_t startIndex = source.find(start);

	// If the starting delimiter is not found on the string
	// stop the process, you're done!
	//
	if(startIndex == std::string::npos){
		return std::string("");
	}

	// Adding the length of the delimiter to our starting index
	// this will move us to the beginning of our sub-string.
	//
	startIndex += start.length();

	// Looking for the end delimiter
	//
	std::string::size_type endIndex = source.find(end, startIndex);

	// Returning the substring between the start index and
	// the end index. If the endindex is invalid then the
	// returned value is empty string.
	return source.substr(startIndex, endIndex - startIndex);
}

void Song::printSongInfo(std::ofstream& f, std::string whitespace){
	if(this->location){
		f << whitespace << "<location>" << *this->location << "</location>" << std::endl;
	}
	if(this->title){
		f << whitespace << "<title>" << *this->title << "</title>" << std::endl;
	}
	if(this->creator){
		f << whitespace << "<creator>" << *this->creator << "</creator>" << std::endl;
	}
	if(this->album){
		f << whitespace << "<album>" << *this->album << "</album>" << std::endl;
	}
	if(this->tracknum){
		f << whitespace << "<tracknum>" << *this->tracknum << "</tracknum>" << std::endl;
	}
	if(this->image){
		f << whitespace << "<image>" << *this->image << "</image>" << std::endl;
	}
	if(this->duration){
		f << whitespace << "<duration>" << *this->duration << "</duration>" << std::endl;
	}
}

int main(int argc, const char** argv){
	if(argc > 1 && boost::iequals(argv[1], "help")){
		printUsage();
		exit(1);
	}

	std::map<int, Song*> songs;
	std::string line;
	enum ParseState { none, songId, songInfo, playlistSearch, skipPlaylist, playlistHeader, playlistSongs, end};
	ParseState state = none;
	int indent = 0;
	std::ofstream file;
	Song * newSong = NULL;
	std::string * playlistName = NULL;

	std::clock_t start = std::clock();
	double duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
	double lastTimed = duration;

	while(std::getline(std::cin, line)){

		duration = (std::clock() - start) / (double)CLOCKS_PER_SEC;
		if((duration-lastTimed) > 0.5){
			lastTimed = duration;
			printf(".");
		}

		boost::trim(line);
		switch(state){
			case none:{
				if(boost::iequals(line, "<dict>")){
					indent++;
					if(indent == 2){
						printf("Analyzing iTunes Library");
						state = songId;
					}
					break;
				}
				if(boost::iequals(line, "</dict>")){
					indent--;
					break;
				}
				break;
			}
			case songId:{
				if(boost::iequals(line, "<dict>")){
					indent++;
					state = songInfo;
					break;
				}
				if(boost::iequals(line, "</dict>")){
					indent--;
					state = playlistSearch;
					break;
				}
				newSong = new Song();
				newSong->id = new int(std::stoi(extractString(line, "<key>", "</key>")));
				break;
			}
			case songInfo:{
				if(boost::iequals(line, "</dict>")){
					indent--;
					songs.insert(std::pair<int, Song*>(*(newSong->id), newSong));
					state = songId;
					break;
				}
				std::string linetype = extractString(line, "<key>", "</key>");

				if(boost::iequals(linetype, "Location")){
					newSong->location = new std::string(extractString(line, "<string>", "</string>"));
				}
				else if(boost::iequals(linetype, "Name")){
					newSong->title = new std::string(extractString(line, "<string>", "</string>"));
				}
				else if(boost::iequals(linetype, "Artist")){
					newSong->creator = new std::string(extractString(line, "<string>", "</string>"));
				}
				else if(boost::iequals(linetype, "Album")){
					newSong->album = new std::string(extractString(line, "<string>", "</string>"));
				}
				else if(boost::iequals(linetype, "Track Number")){
					newSong->tracknum = new int(std::stoi(extractString(line, "<integer>", "</integer>")));
				}
				else if(boost::iequals(linetype, "Total Time")){
					newSong->duration = new int(std::stoi(extractString(line, "<integer>", "</integer>")));
				}
				/*else if(boost::iequals(linetype, "Image")){
					newSong->image = new std::string(extractString(line, "<string>", "</string>"));
				}*/
				break;
			}
			case playlistSearch:{
				if(boost::iequals(line, "<array>")){
					indent++;
					break;
				}
				if(boost::iequals(line, "</array>")){
					indent++;
					state = end;
					break;
				}
				if(boost::iequals(line, "<dict>")){
					indent++;
					state = playlistHeader;
					break;
				}
			}
			case skipPlaylist:{
				if(indent == 3){
					if(boost::iequals(line, "</dict>")){
						indent--;
						state = playlistSearch;
						break;
					}
					if(boost::iequals(line, "<array>")){
						indent++;
						break;
					}
				}
				if(boost::iequals(line, "</array>")){
					indent--;
					break;
				}
				break;
			}
			case playlistHeader:{
				if(boost::iequals(line, "<array>")){
					indent++;
					if(indent > 2){
						state = playlistSongs;
					}
					break;
				}
				if(boost::iequals(line, "</dict>")){
					indent--;
					printFooter(file);
					file.close();
					state = playlistSearch;
					break;
				}
				std::string linetype = extractString(line, "<key>", "</key>");
				if(boost::iequals(linetype, "Name")){
					std::string playlistName = extractString(line, "<string>", "</string>");

					if(isSkippable(playlistName)){
						state = skipPlaylist;
						break;
					}
					std::cout << "\nGenerating "<< playlistName << ".xspf";
					file.open(playlistName + std::string(".xspf"));
					printHeader(file, playlistName);
				}
				break;
			}
			case playlistSongs:{
				if(boost::iequals(line, "</array>")){
					indent--;
					state = playlistHeader;
					break;
				}
				std::string linetype = extractString(line, "<key>", "</key>");
				if(boost::iequals(linetype, "Track ID")){
					int id = std::stoi(extractString(line, "<integer>", "</integer>"));
					Song * s = songs[id];
					file << "\t\t<track>" << std::endl;
					s->printSongInfo(file, generateTabSpace(3));
					file << "\t\t</track>" << std::endl;
				}
				break;
			}
			case end:{
				//Do Nothing
			}
		}
		//std::cout << lineInput << std::endl;
	}
	printf("\nDone\n");
	printf("\nFinished in %f\n", duration);
	return 0;
}