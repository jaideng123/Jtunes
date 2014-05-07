//Bass audio mp3 player prototype
//Jaiden Gerig
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
#include "Window.h"//for GUI
#include "Graph.h"
#include "GUI.h"
#include "mainWindow.h"
#include <iomanip>
using namespace std;

int main(){
	int mainState = 1;
	while (mainState != 0){
		mainWindow win(Point(20, 20), 1000, 600, "Main_Window");
		mainState = win.getState();
	}
	BASS_Free();
	return 0;
}