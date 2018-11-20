#include <string>
#include <iostream>
#include <fstream>

class Song{
public :
	int * id;
	std::string * location;
	std::string * title;
	std::string * creator;
	std::string * album;
	int * tracknum;
	std::string * image;
	int * duration;

	Song(){
		this->id = NULL;
		this->location = NULL;
		this->title = NULL;
		this->creator = NULL;
		this->album = NULL;
		this->tracknum = NULL;
		//this->image = NULL; //Image File location not saved in iTunes .xml
		this->duration = NULL;
	}
	~Song(){
		if(this->id)
			delete this->id;
		if(this->location)
			delete this->location;
		if(this->title)
			delete this->title;
		if(this->creator)
			delete this->creator;
		if(this->album)
			delete this->album;
		if(this->tracknum)
			delete this->tracknum;
		if(this->image)
			delete this->image;
		if(this->duration)
			delete this->duration;
	}
	void printSongInfo(std::ofstream&, std::string);
};

void printUsage();
void printHeader();
void printFooter();
