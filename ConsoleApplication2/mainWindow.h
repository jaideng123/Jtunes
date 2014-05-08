//mainWndow.h
//Jaiden Gerig
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <iostream>
#include <windows.h>
#include <stdio.h>
#include "BASS/bass.h"//audio library
#include "BASS_FX/bass_fx.h"
#include <conio.h>//for _kbhit()
#include <vector>
#include <string>
#include "dirent.h"
#include "Song.h"
#include <string>
#include "std_lib_facilities_3.h"//for GUI
#include "GUI.h"  
#include "Graph.h"
#include "Window.h"
using namespace Graph_lib;

class mainWindow : Graph_lib::Window
{
public:
	mainWindow(Point xy, int w, int h, const string& title);
	const int getState(){ return mainState; }

private:
	//states
	const enum winStates{ goQuit, goNext, goPrev, goPause, goNextPage, goPrevPage };
	int mainState = 0;

	void main_loop();
	bool button_pushed;

	Text titleLabel;
	Text artLabel;
	Text albLabel;
	Text noSongs;

	Button exit_button;
	Button next_button;
	Button prev_button;
	Button pause_button;
	Button play_button;
	Button nextPage_button;
	Button prevPage_button;
	Button addFile_button;
	Button clearIndex_button;

	Out_box songName;
	Out_box currTime;
	Out_box endTime;

	Vector_ref<Out_box> nameList;
	Vector_ref<Out_box> artList;
	Vector_ref<Out_box> albList;
	
	Slider progBar;
	Slider volumeSlider;

	//find a folder to read from
	string fileChooser();
	vector<string> fileCollect(string indexFile);
	void fileAdd(string newDir, string indexFile);
	string seconds2Minutes(int time);

	static void cb_exit(Address, Address pw);
	void exit();
	static void cb_next(Address, Address pw);
	void next();
	static void cb_prev(Address, Address pw);
	void prev();
	static void cb_pause(Address, Address pw);
	void pause();
	static void cb_nextPage(Address, Address pw);
	void nextPage();
	static void cb_prevPage(Address, Address pw);
	void prevPage();
	static void cb_addFile(Address, Address pw);
	void addFile();
	static void cb_clearIndex(Address, Address pw);
	void clearIndex();

	void mainWindow::searchDirectory(string adr, vector<Song> &outVec);
	int winState = 0;
	vector<Song> songVec;
};

#endif