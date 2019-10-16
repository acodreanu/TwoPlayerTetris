#pragma once
#include "PiecePatterns.h"
#include <glm\glm.hpp>
#include <vector>

enum PieceType { I, L, LINV, SQUARE, Z, ZINV, WAT };

class Piece
{
public:
	Piece(PieceType type, glm::ivec2 position, int player, std::vector<std::string>* boardData);
	~Piece();

	bool update();
	void translate();
	bool isEqual(char (&arg)[4][4]);
	void moveUp();
	void moveDown();
	void moveForward();
	void lockPiece();
	bool checkCollision();
	bool checkTransitionCollision(char(&arg)[4][4]);

	// Getters
	bool isCollisioned() const { return _collisioned; }
	void setPiece(char(&arg)[4][4]);
	glm::ivec2 getPosition() const { return _position; };
	std::vector<std::vector<char>>& getPiece() { return _piece; };
private:
	std::vector<std::string>* _boardData;
	PieceType _pieceType;
	std::vector<std::vector<char>> _piece;
	glm::ivec2 _position;
	int _player;
	bool _collisioned;
};

