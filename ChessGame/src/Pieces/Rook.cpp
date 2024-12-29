#include "Rook.h"

Rook::Rook(PieceColor color) : m_color{color} {}

bool Rook::isMoveValid(const std::string &from_pos, const std::string &to_pos) {
  return false;
}

char Rook::getSymbol() const { return 'R'; }

IPiece::PieceColor Rook::getColor() const { return m_color; }
