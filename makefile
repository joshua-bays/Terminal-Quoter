all: main

main: main.cpp
	g++ main.cpp -o quoting-app -lncurses
