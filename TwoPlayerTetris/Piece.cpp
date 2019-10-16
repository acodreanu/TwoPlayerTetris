#include "Piece.h"
#include <iostream>

Piece::Piece(PieceType type, glm::ivec2 position, int player, std::vector<std::string>* boardData) : _pieceType(type), _position(position), _player(player), _collisioned(false)
{
	switch (_pieceType) {
	case I:
		for (int i = 0; i < 4; i++) {
			std::vector<char> aux;
			for (int j = 0; j < 4; j++) {
				aux.push_back(line[i][j]);
			}
			_piece.push_back(aux);
		}
		break;
	case L:
		for (int i = 0; i < 4; i++) {
			std::vector<char> aux;
			for (int j = 0; j < 4; j++) {
				aux.push_back(l1[i][j]);
			}
			_piece.push_back(aux);
		}
	case LINV:
		for (int i = 0; i < 4; i++) {
			std::vector<char> aux;
			for (int j = 0; j < 4; j++) {
				aux.push_back(lInv1[i][j]);
			}
			_piece.push_back(aux);
		}
	case Z:
		for (int i = 0; i < 4; i++) {
			std::vector<char> aux;
			for (int j = 0; j < 4; j++) {
				aux.push_back(z1[i][j]);
			}
			_piece.push_back(aux);
		}
	case ZINV:
		for (int i = 0; i < 4; i++) {
			std::vector<char> aux;
			for (int j = 0; j < 4; j++) {
				aux.push_back(zInv1[i][j]);
			}
			_piece.push_back(aux);
		}
	case WAT:
		for (int i = 0; i < 4; i++) {
			std::vector<char> aux;
			for (int j = 0; j < 4; j++) {
				aux.push_back(wat1[i][j]);
			}
			_piece.push_back(aux);
		}
	case SQUARE:
		for (int i = 0; i < 4; i++) {
			std::vector<char> aux;
			for (int j = 0; j < 4; j++) {
				aux.push_back(pat[i][j]);
			}
			_piece.push_back(aux);
		}
	}
	_boardData = boardData;
}


Piece::~Piece()
{
}

bool Piece::update() {
	if (_collisioned)
		return _collisioned;
	if (_player == 1) {
		_position.x += 1;
		if (checkCollision()) {
			_position.x -= 1;
			lockPiece();
			_collisioned = true;
		}
	} else {
		_position.x -= 1;
		if (checkCollision()) {
			_position.x += 1;
			lockPiece();
			_collisioned = true;
		}
	}
	return _collisioned;
}

void Piece::translate() {
	if (_collisioned)
		return;
	switch (_pieceType) {
	case PieceType::I:
		if (isEqual(line)) {
			if (!checkTransitionCollision(lineInv))
				setPiece(lineInv);
		}
		else if (!checkTransitionCollision(line))
			setPiece(line);
		break;
	case PieceType::Z:
		if (isEqual(z1)) {
			if (!checkTransitionCollision(z2))
				setPiece(z2);
		}
		else if (!checkTransitionCollision(z1))
			setPiece(z1);
		break;
	case PieceType::ZINV:
		if (isEqual(zInv1)) {
			if (!checkTransitionCollision(zInv2))
				setPiece(zInv2);
		}
		else if (!checkTransitionCollision(zInv1))
			setPiece(zInv1);
		break;
	case PieceType::WAT:
		if (isEqual(wat1)) {
			if (!checkTransitionCollision(wat2))
				setPiece(wat2);
		}
		else if (isEqual(wat2)) {
			if (!checkTransitionCollision(wat3))
				setPiece(wat3);
		}
		else if (isEqual(wat3)) {
			if (!checkTransitionCollision(wat4))
				setPiece(wat4);
		} else
		if (!checkTransitionCollision(wat1))
			setPiece(wat1);
		break;
	case PieceType::L:
		if (isEqual(l1)) {
			if (!checkTransitionCollision(l2))
				setPiece(l2);
		}
		else if (isEqual(l2)) {
			if (!checkTransitionCollision(l3))
				setPiece(l3);
		}
		else if (isEqual(l3)) {
			if (!checkTransitionCollision(l4))
				setPiece(l4);
		}
		else if (!checkTransitionCollision(l1))
			setPiece(l1);
		break;
	case PieceType::LINV:
		if (isEqual(lInv1)) {
			if (!checkTransitionCollision(lInv2))
				setPiece(lInv2);
		}
		else if (isEqual(lInv2)) {
			if (!checkTransitionCollision(lInv3))
				setPiece(lInv3);
		}
		else if (isEqual(lInv3)) {
			if (!checkTransitionCollision(lInv4))
				setPiece(lInv4);
		}
		else if (!checkTransitionCollision(lInv1))
			setPiece(lInv1);
		break;
	case PieceType::SQUARE:
		break;
		}
	}

bool Piece::isEqual(char(&arg)[4][4]) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (arg[i][j] != _piece[i][j])
				return false;
		}
	}
	return true;
}

void Piece::setPiece(char(&arg)[4][4]) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			_piece[i][j] = arg[i][j];
		}
	}
}

void Piece::moveUp() {
	if (_collisioned)
		return;
	_position.y -= 1;
	if (checkCollision()) {
		_position.y += 1;
	}
}

void Piece::moveDown() {
	if (_collisioned)
		return;
	_position.y += 1;
	if (checkCollision()) {
		_position.y -= 1;
	}
}

void Piece::moveForward() {
	if (_collisioned)
		return;
	if (_player == 1) {
		_position.x += 1;
		if (checkCollision()){
			_position.x -= 1;
			lockPiece();
			_collisioned = true;
		}
	}
	else {
		_position.x -= 1;
		if (checkCollision()) {
			_position.x += 1;
			lockPiece();
			_collisioned = true;
		}
	}
}

void Piece::lockPiece(){
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (_piece[i][j] == 1) {
				_boardData[0][_position.y + i][_position.x + j] = 'P';
			}
		}
	}

}

bool Piece::checkCollision() {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (_piece[i][j] == 1) {
				if (_boardData[0][_position.y + i][_position.x + j] != '.' && _boardData[0][_position.y + i][_position.x + j] != ',') {
					return true;
				}
			}
		}
	}
	return false;
}

bool Piece::checkTransitionCollision(char(&arg)[4][4]) {
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			if (arg[i][j] == 1) {
				if (_boardData[0][_position.y + i][_position.x + j] != '.' && _boardData[0][_position.y + i][_position.x + j] != ',') {
					return true;
				}
			}
		}
	}
	return false;
}
