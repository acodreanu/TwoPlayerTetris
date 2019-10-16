#include "Board.h"
#include <iostream>
#include <fstream>
#include "PiecePatterns.h"
#include <stdlib.h>
#include <time.h>

Board::Board() : _renderer(nullptr), _player1Score(0), _player2Score(0){
	std::ifstream in("Level/tetris.txt");
	if (in.fail()) {
		std::cout << "Error opening level file!\n";
	}
	std::string line;
	while (std::getline(in, line)) {
		_boardData.push_back(line);
	}
}

Board::~Board()
{}

void Board::reset() {
	for (int i = 0; i < _boardData.size(); i++) {
		_boardData[i].clear();
	}
	_boardData.clear();

	std::ifstream in("Level/tetris.txt");
	if (in.fail()) {
		std::cout << "Error opening level file!\n";
	}
	std::string line;
	while (std::getline(in, line)) {
		_boardData.push_back(line);
	}
}

void Board::init(SDL_Window* window, int tileSize) {
	TILE_SIZE = tileSize;
	_renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);
	if (TTF_Init() == -1) {
		std::cout << "SDL_ttf could not initialize! SDL_ttf Error: " << TTF_GetError() << "\n";
	}
	_sans = TTF_OpenFont("Sans.ttf", 128);
	if (!_sans) {
		std::cout << "Cannot open font file!!\n";
	}
	_pieceTypes.push_back(PieceType::I);
	_pieceTypes.push_back(PieceType::Z);
	_pieceTypes.push_back(PieceType::ZINV);
	_pieceTypes.push_back(PieceType::L);
	_pieceTypes.push_back(PieceType::LINV);
	_pieceTypes.push_back(PieceType::SQUARE);
	_pieceTypes.push_back(PieceType::WAT);
	srand(time(NULL));
	_piecesP1.emplace_back(_pieceTypes[rand() % 7], glm::ivec2(0, 3), 1, &_boardData);
	_piecesP2.emplace_back(_pieceTypes[rand() % 7], glm::ivec2(_boardData[0].size() - 5, 3), 2, &_boardData);
	_borderLocation = _boardData[0].size() / 2 + 1;
}

bool Board::update() {
	for (int i = 0; i < _piecesP1.size(); i++) {
		if (_piecesP1[i].update()){
			_piecesP1.erase(_piecesP1.begin() + i);
			int random = rand() % 7;
			_piecesP1.emplace_back(_pieceTypes[random], glm::vec2(0, 3), 1, &_boardData);
		}
	}
	for (int i = 0; i < _piecesP2.size(); i++) {
		if (_piecesP2[i].update()){
			_piecesP2.erase(_piecesP2.begin() + i);
			int random = rand() % 7;
			_piecesP2.emplace_back(_pieceTypes[random], glm::vec2(_boardData[0].size() - 5, 3), 2, &_boardData);
		}
	}
	pushBar();
	if (checkLose()) {
		return true;
	}
	return false;
}

bool Board::checkLose() {
	for (int i = 0; i < _boardData.size(); i++) {
		if (!(_boardData[i][2] == '.' || _boardData[i][2] == 'Z')) {
			std::cout << "Player 2 wins!\n";
			return true;
		}
	}
	for (int i = 0; i < _boardData.size(); i++) {
		if (!(_boardData[i][_boardData[0].size() - 3] == '.' || _boardData[i][_boardData[0].size() - 3] == 'Z')) {
			std::cout << "Player 1 wins!\n";
			return true;
		}
	}
	return false;
}

void Board::pushBar(){
	bool full;
	for (int i = 0; i < _boardData[0].size(); i++) {
		full = true;
		for (int j = 1; j < _boardData.size() - 1; j++) {
			if (_boardData[j][i] != 'P') {
				full = false;
			}
		}
		if (full) {
			if (i < _borderLocation){
				_player1Score++;
				std::vector<std::string> aux;
				std::string line;

				for (int x = 0; x < _boardData.size(); x++) {
					if (x == 0 || x == _boardData.size() - 1){
						line.push_back('Z');
						line.push_back('Z');
					}
					else {
						line.push_back(',');
						line.push_back(',');
					}
					for (int y = 0; y < _boardData[0].size() - 1; y++){
						if (y < 3) {
							if (_boardData[x][y] == ',') {
								line.push_back('.');
								continue;
							}
						}
						if (y == i) {
							continue;
						}
						if (y == _boardData[0].size() - 3) {
							if (x == 0 || x == _boardData.size() - 1) {
								line.push_back('Z');
							}
							else {
								line.push_back(',');
							}
							continue;
						}
						line.push_back(_boardData[x][y]);
					}
					aux.push_back(line);
					line.clear();
				}
				_boardData.clear();
				for (int x = 0; x < aux.size(); x++) {
					_boardData.push_back(aux[x]);
				}
				_borderLocation++;
			}
			else {
				_player2Score++;
				std::vector<std::string> aux;
				std::string line;

				for (int x = 0; x < _boardData.size(); x++) {
					if (x == 0 || x == _boardData.size() - 1) {
						line.push_back('Z');
						line.push_back('Z');
					}
					else {
						line.push_back(',');
						line.push_back(',');
					}
					for (int y = 3; y < _boardData[0].size(); y++){
						if (y == i) {
							continue;
						}
						if (y >= 3) {
							if (_boardData[x][y] == ',') {
								line.push_back('.');
								continue;
							}
						}
						line.push_back(_boardData[x][y]);
					}
					if (x == 0 || x == _boardData.size() - 1){
						line.push_back('Z');
						line.push_back('Z');
					}
					else {
						line.push_back(',');
						line.push_back(',');
					}
					aux.push_back(line);
					line.clear();
				}
				_boardData.clear();
				for (int x = 0; x < aux.size(); x++) {
					_boardData.push_back(aux[x]);
				}
				_borderLocation--;
			}
		}
	}
}

void Board::drawMenu() {
	SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
	SDL_RenderClear(_renderer);
	SDL_Color Red = { 255, 0, 0 };

	char* ng;
	ng = (char*)std::calloc(20, 4);
	sprintf_s(ng, 80, "%s", "New Game");
	char* quit;
	quit = (char*)std::calloc(20, 4);
	sprintf_s(quit, 80, "%s", "Quit");

	SDL_Surface* surfaceMessage1 = TTF_RenderText_Solid(_sans, ng, Red);
	SDL_Surface* surfaceMessage2 = TTF_RenderText_Solid(_sans, quit, Red);

	SDL_Texture* p1Text = SDL_CreateTextureFromSurface(_renderer, surfaceMessage1);
	SDL_Texture* p2Text = SDL_CreateTextureFromSurface(_renderer, surfaceMessage2);

	SDL_Rect ngRect;
	ngRect.x = TILE_SIZE * 16;
	ngRect.y = TILE_SIZE * 2;
	ngRect.w = 8 * TILE_SIZE;
	ngRect.h = TILE_SIZE * 2;

	SDL_Rect quitRect;
	quitRect.x = TILE_SIZE * 18 + 20;
	quitRect.y = TILE_SIZE * 8;
	quitRect.w = 3 * TILE_SIZE;
	quitRect.h = TILE_SIZE * 2;

	SDL_RenderCopy(_renderer, p1Text, NULL, &ngRect);
	SDL_RenderCopy(_renderer, p2Text, NULL, &quitRect);

	SDL_RenderPresent(_renderer);
}

void Board::draw() {
	// Set render color
	SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);

	// Clear winow
	SDL_RenderClear(_renderer);
	
	// Render the board tiles
	SDL_Rect r;
	for (int i = 0; i < _boardData.size(); i++) {
		for (int j = 0; j < _boardData[0].size(); j++) {
			switch (_boardData[i][j]) {
			case 'Z':
				r.x = j * TILE_SIZE;
				r.y = i * TILE_SIZE;
				r.w = TILE_SIZE - 1;
				r.h = TILE_SIZE - 1;
				SDL_SetRenderDrawColor(_renderer, 0, 0, 255, 255);
				SDL_RenderFillRect(_renderer, &r);
				break;
			case 'B':
				r.x = j * TILE_SIZE;
				r.y = i * TILE_SIZE;
				r.w = TILE_SIZE - 1;
				r.h = TILE_SIZE - 1;
				SDL_SetRenderDrawColor(_renderer, 255, 0, 0, 255);
				SDL_RenderFillRect(_renderer, &r);
				break;
			case 'P':
				r.x = j * TILE_SIZE;
				r.y = i * TILE_SIZE;
				r.w = TILE_SIZE - 1;
				r.h = TILE_SIZE - 1;
				SDL_SetRenderDrawColor(_renderer, 75, 75, 75, 255);
				SDL_RenderFillRect(_renderer, &r);
				break;
			case '.':
				r.x = j * TILE_SIZE;
				r.y = i * TILE_SIZE;
				r.w = TILE_SIZE - 1;
				r.h = TILE_SIZE - 1;
				SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
				SDL_RenderFillRect(_renderer, &r);
				break;
			case ',':
				r.x = j * TILE_SIZE;
				r.y = i * TILE_SIZE;
				r.w = TILE_SIZE;
				r.h = TILE_SIZE;
				SDL_SetRenderDrawColor(_renderer, 255, 255, 0, 255);
				SDL_RenderFillRect(_renderer, &r);
				break;
			}
		}
	}

	// Render the pieces
	for (int x = 0; x < _piecesP1.size(); x++){
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (_piecesP1[x].getPiece()[i][j] == 1) {
					r.x = _piecesP1[x].getPosition().x * TILE_SIZE + j * TILE_SIZE;
					r.y = _piecesP1[x].getPosition().y * TILE_SIZE + i * TILE_SIZE;
					r.w = TILE_SIZE - 1;
					r.h = TILE_SIZE - 1;
					SDL_SetRenderDrawColor(_renderer, 0, 255, 0, 255);
					SDL_RenderFillRect(_renderer, &r);
				}
			}
		}
	}
	for (int x = 0; x < _piecesP2.size(); x++){
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				if (_piecesP2[x].getPiece()[i][j] == 1) {
					r.x = _piecesP2[x].getPosition().x * TILE_SIZE + j * TILE_SIZE;
					r.y = _piecesP2[x].getPosition().y * TILE_SIZE + i * TILE_SIZE;
					r.w = TILE_SIZE - 1;
					r.h = TILE_SIZE - 1;
					SDL_SetRenderDrawColor(_renderer, 0, 255, 0, 255);
					SDL_RenderFillRect(_renderer, &r);
				}
			}
		}
	}

	// Render the text
	SDL_Color White = { 255, 255, 255 }; 
	char* p1;
	p1 = (char*)std::calloc(20, 4);
	sprintf_s(p1, 80, "%s: %d", "Player 1 Score", _player1Score);
	char* p2;
	p2 = (char*)std::calloc(20, 4);
	sprintf_s(p2, 80, "%s: %d", "Player 2 Score", _player2Score);
	SDL_Surface* surfaceMessage1 = TTF_RenderText_Solid(_sans, p1, White);
	SDL_Surface* surfaceMessage2 = TTF_RenderText_Solid(_sans, p2, White);
	
	SDL_Texture* p1Text = SDL_CreateTextureFromSurface(_renderer, surfaceMessage1);
	SDL_Texture* p2Text = SDL_CreateTextureFromSurface(_renderer, surfaceMessage2);

	SDL_Rect Message_rect1;
	Message_rect1.x = TILE_SIZE;
	Message_rect1.y = _boardData.size() * TILE_SIZE;
	Message_rect1.w = 8 * TILE_SIZE; 
	Message_rect1.h = TILE_SIZE * 2; 

	SDL_Rect Message_rect2;
	Message_rect2.x = _boardData[0].size() * TILE_SIZE / 2 + TILE_SIZE + 20;
	Message_rect2.y = _boardData.size() * TILE_SIZE;
	Message_rect2.w = 8 * TILE_SIZE;
	Message_rect2.h = TILE_SIZE * 2;

	SDL_RenderCopy(_renderer, p1Text, NULL, &Message_rect1);
	SDL_RenderCopy(_renderer, p2Text, NULL, &Message_rect2);
	
	// Render everything to the screen
	SDL_RenderPresent(_renderer);
	delete p1;
	delete p2;
}


void Board::moveCurrentPiece(MoveDirection move, int player) {
	if (player == 1){
		switch (move) {
		case MoveDirection::DOWN:
			if (!_piecesP1.empty())
				_piecesP1.back().moveDown();
			break;
		case MoveDirection::UP:
			if (!_piecesP1.empty())
				_piecesP1.back().moveUp();
			break;
		case MoveDirection::FORWARD:
			if (!_piecesP1.empty())
				_piecesP1.back().moveForward();
			break;
		}
	} else {
		switch (move) {
		case MoveDirection::DOWN:
			if (!_piecesP2.empty())
				_piecesP2.back().moveDown();
			break;
		case MoveDirection::UP:
			if (!_piecesP2.empty())
				_piecesP2.back().moveUp();
			break;
		case MoveDirection::FORWARD:
			if (!_piecesP2.empty())
				_piecesP2.back().moveForward();
			break;
		}
	}
}
