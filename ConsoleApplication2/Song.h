#ifndef SONG_H
#define SONG_H
//Song Class
//Jaiden Gerig
//4/14/14
#include <iostream>
#include <windows.h>
#include <stdio.h>
#include "BASS/bass.h"//audio library
#include "BASS_FX/bass_fx.h"
#include <vector>
#include <string>
#include <cstring>
#include <iomanip>
#include <id3/tag.h>
#include <id3/field.h>
#include "id3/misc_support.h"
#include "tagHelper.h"
using namespace std;

class Song{
private:
	HSTREAM stream;
	string name;
	string location;
	string artist;
	string album;
	uint32 length;

public:
	Song(){
		stream = NULL;
		name = "NULL";
	}
	Song(string tempLoc){
		stream = BASS_StreamCreateFile(FALSE, tempLoc.c_str(), 0, 0, 0);
		location = tempLoc;
		pullMetaData();
		cout << length << endl;
		if (name == ""){
			if (tempLoc.find("\\") != string::npos || tempLoc.find("/") != string::npos)
				name = tempLoc.substr(tempLoc.find_last_of("\\/")+1, tempLoc.size() - 1);
			else
				name = tempLoc.substr(0, tempLoc.size() - 4);
		}
	}
	Song(string tempLoc, HSTREAM tempStream){
		stream = tempStream;
		location = tempLoc;
		pullMetaData();
		if (tempLoc.find("\\") != string::npos || tempLoc.find("/") != string::npos)
			name = tempLoc.substr(tempLoc.find_last_of("\\/")+1, tempLoc.size() - 1);
		else
			name = tempLoc.substr(0, tempLoc.size() - 4);
	}
	void play(){BASS_ChannelPlay(stream, FALSE); }//Play song
	void stop(){//stop song entirely
		BASS_ChannelStop(stream);
		BASS_ChannelSetPosition(stream, BASS_POS_OGG, 0);
	}
	void pause(){//Pause song
		if (BASS_ChannelIsActive(stream) == BASS_ACTIVE_PLAYING){
			BASS_ChannelPause(stream);
		}
	}
	void setName(string tempName){ name = tempName; }
	void setStream(HSTREAM tempStream){ stream = tempStream; }
	HSTREAM getStream(){ return stream; }
	string getName(){ return name; }
	string getArtist(){ return artist; }
	string getAlbum(){ return album; }
	string getLoc(){ return location; }
	bool isActive(){
		if (BASS_ChannelIsActive(stream) == BASS_ACTIVE_PAUSED || BASS_ChannelIsActive(stream) == BASS_ACTIVE_STOPPED)
			return false;
		else
			return true;
	}
	bool isOver(){//tests if stream is over
		if (BASS_ChannelGetPosition(stream, BASS_POS_BYTE) == BASS_ChannelGetLength(stream, BASS_POS_BYTE))
			return true;
		else
			return false;
	}
	void pullMetaData(){//get any available metadata from file
		ID3_Tag myTag(location.c_str());
		artist = ID3_GetArtist(&myTag);
		album = ID3_GetAlbum(&myTag);
		name = ID3_GetTitle(&myTag);
		tagHelper th(location.c_str());
		length = th.getTrackLength();


	}

};
#endif
