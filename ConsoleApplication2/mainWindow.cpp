//mainWindow.cpp
//Jaiden Gerig
#include "Mainwindow.h"
#include <cmath>
#include <FL/Fl_Native_File_Chooser.H>
#include <iostream>
using namespace Graph_lib;

mainWindow::mainWindow(Point xy, int w, int h, const string& title)
	:Window(xy,w,h,title),
	exit_button(Point((x_max() - 77), 0), 70, 20, "Quit", cb_exit),
	next_button(Point((x_max() / 2) + 40, 30), 70, 20, "Next", cb_next),
	prev_button(Point((x_max() / 2) - 130, 30), 70, 20, "Previous", cb_prev),
	pause_button(Point((x_max() / 2)-60, 30), 100, 20, "Pause", cb_pause),
	play_button(Point((x_max() / 2) - 60, 30), 100, 20, "Play", cb_pause),
	nextPage_button(Point(690, 95), 70, 20, "--->", cb_nextPage),
	prevPage_button(Point(620, 95), 70, 20, "<---", cb_prevPage),
	clearIndex_button(Point(0, 40), 70, 20, "Clear Lib.", cb_clearIndex),
	songName(Point((x_max() / 2) - 150, 70),250,20,"Current Song:"),
	titleLabel(Point(15, 110),"Title"),
	artLabel(Point(270, 110), "Artist"),
	albLabel(Point(450, 110), "Album"),
	noSongs(Point((x_max() / 2) - 150, 70), "Library is empty, click add file to add songs"),
	addFile_button(Point(0, 20), 70, 20, "Add File", cb_addFile)
{
	lockSize();//set up window
	removeX();
	int device = -1; // Default Sounddevice
	int freq = 44100; // Sample rate (Hz)
	/* Initialize output device */
	BASS_Init(device, freq, 0, 0, NULL);
	vector<string> dir = fileCollect("index.txt");//round up file list
	for (int i = 0; i < dir.size(); ++i){
		searchDirectory(dir[i], songVec);
	}


	int index = 0;
	while (index < songVec.size()){
		for (int i = 0; i < 25 && index < songVec.size(); ++i){//Set up outboxes for songs
			cout << albList.size()<<endl;
			nameList.push_back(new Out_box(Point(15, (120 + i * 18)), 250, 20, ""));
			artList.push_back(new Out_box(Point(270, (120 + i * 18)), 170, 20, ""));
			albList.push_back(new Out_box(Point(450, (120 + i * 18)), 220, 20, ""));
			++index;
		}
	}
	for (int i = 0; i < songVec.size() && i < 25; ++i){
		attach(nameList[i]);
		attach(artList[i]);
		attach(albList[i]);
		nameList[i].put(songVec[i].getName());
		artList[i].put(songVec[i].getArtist());
		albList[i].put(songVec[i].getAlbum());
	}
	attach(addFile_button);
	attach(clearIndex_button);
	attach(exit_button);
	if (songVec.size() > 0){
		attach(next_button);
		attach(prev_button);
		attach(play_button);
		attach(nextPage_button);
		attach(prevPage_button);
		attach(songName);
		titleLabel.set_font_size(16);
		attach(titleLabel);
		artLabel.set_font_size(16);
		attach(artLabel);
		albLabel.set_font_size(16);
		attach(albLabel);
	}
	else
		attach(noSongs);
	winState = 6;//do nothing
	main_loop();
}
void mainWindow::main_loop()
{

	int playIndex = 0;
	int pageIndex = 0;
	int pageLimit = ceil((songVec.size() / 25.0));
	
	while (winState != goQuit){
		button_pushed = false;
		while (!button_pushed){
			if (songVec.size() > 0){
				if (songVec[playIndex].isOver()){//checks for end of song
					songVec[playIndex].stop();
					nameList[playIndex].unhighlight();
					artList[playIndex].unhighlight();
					albList[playIndex].unhighlight();
					songName.put("");
					if (playIndex + 1 != songVec.size()){
						playIndex = (playIndex + 1) % songVec.size();
						songVec[playIndex].play();
						songName.put(songVec[playIndex].getName());
						nameList[playIndex].highlight();
						artList[playIndex].highlight();
						albList[playIndex].highlight();
					}
					else{
						playIndex = (playIndex + 1) % songVec.size();
						songName.put("");
						detach(pause_button);
						attach(play_button);
					}
				}
			}
			Fl::check();//wait but also check for end of song
		}
		if (winState == goNext){//next song
			if (!songVec[playIndex].isActive()){
				detach(play_button);
				attach(pause_button);
			}

			songVec[playIndex].stop();
			nameList[playIndex].unhighlight();
			artList[playIndex].unhighlight();
			albList[playIndex].unhighlight();
			playIndex = (playIndex + 1) % songVec.size();
			songVec[playIndex].play();
			songName.put(songVec[playIndex].getName());
			nameList[playIndex].highlight();
			artList[playIndex].highlight();
			albList[playIndex].highlight();
		}
		else if (winState == goPrev){//previous song
			if (!songVec[playIndex].isActive()){
				detach(play_button);
				attach(pause_button);
			}
			songVec[playIndex].stop();
			nameList[playIndex].unhighlight();
			artList[playIndex].unhighlight();
			albList[playIndex].unhighlight();
			if (playIndex != 0)
				playIndex = (playIndex - 1) % songVec.size();
			else
				playIndex = songVec.size() - 1;
			songVec[playIndex].play();
			songName.put(songVec[playIndex].getName());
			nameList[playIndex].highlight();
			artList[playIndex].highlight();
			albList[playIndex].highlight();
		}
		else if (winState == goPause){//Pause current Song
			if (songVec[playIndex].isActive()){
				songVec[playIndex].pause();
				nameList[playIndex].unhighlight();
				artList[playIndex].unhighlight();
				albList[playIndex].unhighlight();
				detach(pause_button);
				attach(play_button);
			}
			else{
				songVec[playIndex].play();
				nameList[playIndex].highlight();
				artList[playIndex].highlight();
				albList[playIndex].highlight();
				songName.put(songVec[playIndex].getName());
				detach(play_button);
				attach(pause_button);
			}
		}
		else if (winState == goNextPage){//go to next page of songs
			for (int i = 25*pageIndex; i < songVec.size() && i < (25 + (25*pageIndex)); ++i){
				detach(nameList[i]);
				detach(artList[i]);
				detach(albList[i]);
			}
			pageIndex = (pageIndex + 1) % pageLimit;
			for (int i = 25*pageIndex; i < songVec.size() && i < 25+25*pageIndex; ++i){
				attach(nameList[i]);
				attach(artList[i]);
				attach(albList[i]);
				nameList[i].put(songVec[i].getName());
				artList[i].put(songVec[i].getArtist());
				albList[i].put(songVec[i].getAlbum());
			}
		}

		else if (winState == goPrevPage){//go to prev. page of songs
			for (int i = 25 * pageIndex; i < songVec.size() && i < (25 + (25 * pageIndex)); ++i){
				detach(nameList[i]);
				detach(artList[i]);
				detach(albList[i]);
			}
			if (pageIndex == 0)
				pageIndex = pageLimit;
			--pageIndex;
			for (int i = 25 * pageIndex; i < songVec.size() && i < 25 + 25 * pageIndex; ++i){
				attach(nameList[i]);
				attach(artList[i]);
				attach(albList[i]);
				nameList[i].put(songVec[i].getName());
				artList[i].put(songVec[i].getArtist());
				albList[i].put(songVec[i].getAlbum());
			}
		}
	}
	BASS_Free();
	hide();
}

//looks through directory for mp3 files and adds them to a vector of songs
void mainWindow::searchDirectory(string adr, vector<Song> &outVec){
	DIR *dir;
	struct dirent *ent;
	//extract all mp3 file names
	if ((dir = opendir(adr.c_str())) != NULL) {
		while ((ent = readdir(dir)) != NULL) {
			string temp = ent->d_name;
			//pick only mp3's
			if (temp.size() > 4 && temp.substr(temp.size() - 4) == ".mp3")
				outVec.push_back(Song(adr +"\\" + temp));
		}
		closedir(dir);
	}
	else {
		/* could not open directory */
		//throw EXIT_FAILURE;
		return;
	}
	return;
}

string mainWindow::fileChooser(){
	Fl_Native_File_Chooser fnfc;
	fnfc.title("Pick a file");
	fnfc.type(Fl_Native_File_Chooser::BROWSE_DIRECTORY);
	fnfc.directory("");           // default directory to use
	// Show native chooser
	switch (fnfc.show()) {
	case -1: printf("ERROR: %s\n", fnfc.errmsg());    break;  // ERROR
	case  1: return "";                      break;  // CANCEL
	default: return fnfc.filename(); break;  // FILE CHOSEN
	}
}

vector<string> mainWindow::fileCollect(string indexFile){
	vector<string> out;
	ifstream myFile(indexFile);
	if (myFile.is_open()){
		string temp;
		while (getline(myFile, temp)){
			out.push_back(temp);
		}
	}
	return out;
}

void mainWindow::fileAdd(string newDir,string indexFile){
	ofstream myFile(indexFile,ios::app);
	if (myFile.is_open()){
		myFile << newDir << endl;
	}
	return;
}

void mainWindow::cb_exit(Address, Address pw)
{
	reference_to<mainWindow>(pw).exit();
}
void mainWindow::exit()
{
	button_pushed = true;
	winState = goQuit;
}
void mainWindow::cb_next(Address, Address pw)
{
	reference_to<mainWindow>(pw).next();
}
void mainWindow::next()
{
	button_pushed = true;
	winState = goNext;
}
void mainWindow::cb_prev(Address, Address pw)
{
	reference_to<mainWindow>(pw).prev();
}
void mainWindow::prev()
{
	button_pushed = true;
	winState = goPrev;
}
void mainWindow::cb_pause(Address, Address pw)
{
	reference_to<mainWindow>(pw).pause();
}
void mainWindow::pause()
{
	button_pushed = true;
	winState = goPause;
}
void mainWindow::cb_nextPage(Address, Address pw)
{
	reference_to<mainWindow>(pw).nextPage();
}
void mainWindow::nextPage()
{
	button_pushed = true;
	winState = goNextPage;
}
void mainWindow::cb_prevPage(Address, Address pw)
{
	reference_to<mainWindow>(pw).prevPage();
}
void mainWindow::prevPage()
{
	button_pushed = true;
	winState = goPrevPage;
}
void mainWindow::cb_addFile(Address, Address pw)
{
	reference_to<mainWindow>(pw).addFile();
}
void mainWindow::addFile()
{
	button_pushed = true;
	winState = goQuit;
	mainState = 1;
	BASS_Free();
	fileAdd(fileChooser(), "index.txt");
	return;
}
void mainWindow::cb_clearIndex(Address, Address pw)
{
	reference_to<mainWindow>(pw).clearIndex();
}
void mainWindow::clearIndex()
{
	button_pushed = true;
	winState = goQuit;
	mainState = 1;
	ofstream indexclear;
	indexclear.open("index.txt", ios::trunc);
	indexclear.close();
	return;

}