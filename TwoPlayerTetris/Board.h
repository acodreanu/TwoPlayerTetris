#pragma once
#include <vector>
#include <string>
#include <SDL\SDL.h>
#include "Piece.h"
#include <SDL\SDL_ttf.h>

enum class MoveDirection { UP, DOWN, FORWARD };

class Board
{
public:
	Board();
	~Board();

	void init(SDL_Window* window, int tileSize);
	bool update();
	void draw();
	void drawMenu();
	void moveCurrentPiece(MoveDirection move, int player);
	void pushBar();
	bool checkLose();
	void reset();

	std::vector<Piece>& getPiecesP1() { return _piecesP1; }
	std::vector<Piece>& getPiecesP2() { return _piecesP2; }

	int TILE_SIZE;
private:
	std::vector<std::string> _boardData;
	std::vector<PieceType> _pieceTypes;
	std::vector<Piece> _piecesP1;
	std::vector<Piece> _piecesP2;
	SDL_Renderer* _renderer;
	SDL_Window* _window;
	int _borderLocation;
	int _player1Score;
	int _player2Score;
	TTF_Font* _sans;
};

