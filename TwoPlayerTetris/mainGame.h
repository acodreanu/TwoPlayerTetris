#pragma once
#include <SDL\SDL.h>
#include "Board.h"

enum class gameState { EXIT, PLAY, MENU };

class mainGame
{
public:
	mainGame();
	~mainGame();

	void run();
	void init();
	void gameLoop();
	void processInput();
	void draw();
private:
	SDL_Window* _window;

	gameState _gameState;

	Board _board;
};

