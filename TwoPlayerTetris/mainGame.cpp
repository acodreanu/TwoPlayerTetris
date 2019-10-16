#include "mainGame.h"
#include <iostream>

static int TILE_SIZE = 40;

mainGame::mainGame() : _gameState(gameState::MENU)
{
}


mainGame::~mainGame()
{
}

void mainGame::run() {
	init();
	gameLoop();
}

void mainGame::init() {
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	Uint32 flags = SDL_WINDOW_OPENGL;
	flags |= SDL_WINDOW_BORDERLESS;
	_window = SDL_CreateWindow("Tetris", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, TILE_SIZE * 41, TILE_SIZE * 13, SDL_WINDOW_SHOWN);
	if (_window == nullptr) {
		std::cout << "SDL Window could not be created!\n";
	}
	_board.init(_window, TILE_SIZE);
}


void mainGame::gameLoop() {
	SDL_Renderer* _renderer = SDL_CreateRenderer(_window, 0, SDL_RENDERER_ACCELERATED);
	int frames = 0;
	std::cout << "Author: Blazzer\n\n**** CONTROLS ****\nPlayer 1:\nW/S -> Up/Down\nD-> Forward\nA -> Rotate Piece\n\nPlayer 2:\nI/K -> Up/Down\nJ -> Forward\nL -> Rotate\n\nGLHF\n";
	while (_gameState != gameState::EXIT) {
		float now = SDL_GetTicks();
		if (_gameState == gameState::MENU) {
				processInput();
				draw();
		} else {
			processInput();
			if (frames >= 45) {
				if (_board.update()) {
					_gameState = gameState::MENU;
					_board.reset();
				}
				draw();
				frames = 0;
			}
		}

		float then = SDL_GetTicks();
		float waitTime = 16 - then + now;
		if (waitTime > 0)
			SDL_Delay(waitTime);
		frames++;
	}
}

void mainGame::processInput() {
	SDL_Event evnt;

	while (SDL_PollEvent(&evnt)) {
		if (_gameState == gameState::MENU) {
			switch (evnt.type) {
			case SDL_QUIT:
				_gameState = gameState::EXIT;
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (evnt.button.x >= TILE_SIZE * 16 && evnt.button.x <= TILE_SIZE * 24 && evnt.button.y <= TILE_SIZE * 4 && evnt.button.y >= TILE_SIZE * 2) {
					_gameState = gameState::PLAY;
				}
				if (evnt.button.x >= TILE_SIZE * 18 + 80 && evnt.button.x <= TILE_SIZE * 21 + 20 && evnt.button.y <= TILE_SIZE * 10 && evnt.button.y >= TILE_SIZE * 8) {
					_gameState = gameState::EXIT;
				}
				break;
			}
		}
		else {
			switch (evnt.type){
			case SDL_QUIT:
				_gameState = gameState::EXIT;
				break;
			case SDL_KEYDOWN:
				switch (evnt.key.keysym.sym) {
				case SDLK_a:
					if (!_board.getPiecesP1().empty()) {
						_board.getPiecesP1().back().translate();
						_board.draw();
					}
					break;
				case SDLK_w:
					_board.moveCurrentPiece(MoveDirection::UP, 1);
					_board.draw();
					break;
				case SDLK_s:
					_board.moveCurrentPiece(MoveDirection::DOWN, 1);
					_board.draw();
					break;
				case SDLK_d:
					_board.moveCurrentPiece(MoveDirection::FORWARD, 1);
					_board.draw();
					break;
				case SDLK_l:
					if (!_board.getPiecesP1().empty()) {
						_board.getPiecesP2().back().translate();
						_board.draw();
					}
					break;
				case SDLK_i:
					_board.moveCurrentPiece(MoveDirection::UP, 2);
					_board.draw();
					break;
				case SDLK_k:
					_board.moveCurrentPiece(MoveDirection::DOWN, 2);
					_board.draw();
					break;
				case SDLK_j:
					_board.moveCurrentPiece(MoveDirection::FORWARD, 2);
					_board.draw();
					break;
				}
			}
		}
	}
}

void mainGame::draw() {
	if (_gameState == gameState::PLAY) {
		_board.draw();
	}
	else {
		_board.drawMenu();
	}
}