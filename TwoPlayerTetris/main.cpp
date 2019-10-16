#include "mainGame.h"
#include <SDL\SDL.h>
#include <SDL/SDL_ttf.h>
#include <GL\glew.h>

int main(int argc, char* argv[]) {
	mainGame game;
	game.run();
	return 0;
}