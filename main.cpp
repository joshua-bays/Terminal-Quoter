#include <ncurses.h>

#include <deque>
#include <fstream>
#include <random>
#include <string>

#include <stdio.h>
#include <time.h>

#define SourceFile "verses.txt" //File the verses are read from
#define SepChar '|' //Character to distinguish between references and verses

#define StandardColorIndex 1
#define StandardColors StandardColorIndex, 7, 0

#define IncorrectColorIndex 2
#define IncorrectColors IncorrectColorIndex, 7, 1

#define MinPercent 90

char make_lowercase(char &ch);
float type_verse(std::string verse, std::string ref);
int get_file_line_count(std::string fileName);
int random_int(int max);
std::string read_file_line(std::string &fileName, int &lineNum);
void remove_element(std::deque<std::string> &dqe, int &index);

int main(){	
	srand(time(0));
	
	std::ifstream readfile; std::string line;
	readfile.open(SourceFile, std::ios::in);
	std::deque<std::string> verseList; int pos;
	
	WINDOW *win = initscr();
	scrollok(win, true);
	noecho(); start_color();
	init_pair(StandardColors); init_pair(IncorrectColors);
	attron(COLOR_PAIR(StandardColorIndex));
	refresh();
	
	while(getline(readfile, line)){
		verseList.push_back(line);
		//printw("%s\n", line.substr(line.find(SepChar) + 1, line.size() - 1).c_str());
		//printw("%s\n", line.substr(0, line.find(SepChar)).c_str());
	}
	while(verseList.size() > 0){
		pos = random_int(verseList.size());
		if(type_verse(verseList[pos].substr(verseList[pos].find(SepChar) + 1, verseList[pos].size() - 1).c_str(), verseList[pos].substr(0, verseList[pos].find(SepChar)).c_str()) >= 90){
			remove_element(verseList, pos);
		}
	}
	getch();
	endwin();
	return 0;
}

char make_lowercase(char &ch){
	return (ch >= 65 && ch <= 90) ? ch + 32 : ch;
}

float type_verse(std::string verse, std::string ref){
	char key;
	float errorKeys, totalKeys = 0;
	printw("%s\n", ref.c_str());
	
	for(char &i : verse){
		if(i == ',' || i == '.' || i == '!' || i == ':' || i == ';' || i == '?' || i == '|'){
			printw("%c", i);
		}else{
			key = getch();
			if(make_lowercase(key) == make_lowercase(i)){
				printw("%c", i);
			}else{
				if(i == ' '){
					do{
						key = getch();
					}while(key != ' ');
					printw(" ");
					totalKeys--;
				}else{
					attron(COLOR_PAIR(IncorrectColorIndex));
					printw("%c", i);
					attron(COLOR_PAIR(StandardColorIndex));
					errorKeys++;
				}
			}
			totalKeys++;
		}
	}
	printw("\n%.0f\%\n\nPress enter to continue\n\n", (1 - (errorKeys / totalKeys)) * 100);
	do{
		key = getch();
	}while(key != '\n');
	scroll(NULL);
	return (1 - (errorKeys / totalKeys)) * 100;
}

int get_file_line_count(std::string &fileName){
	int ret = 0; std::ifstream readfile; std::string line;
	readfile.open(fileName);
	while(getline(readfile, line)){
		ret++;
	}
	return ret;
}

int random_int(int max){
	return rand() % (max + 1);
}

std::string read_file_line(std::string &fileName, int &lineNum){
	std::ifstream readfile; std::string line;
	readfile.open(fileName);
	for(int i = 0; i < lineNum; i++){
		getline(readfile, line);
	}
	return line;
}

void remove_element(std::deque<std::string> &dqe, int &index){
	std::deque<std::string> ret;
	for(int i = dqe.size(); i--;){
		if(i != index){
			ret.push_front(dqe[i]);
		}
	}
	dqe = ret;
	ret.clear();
}

