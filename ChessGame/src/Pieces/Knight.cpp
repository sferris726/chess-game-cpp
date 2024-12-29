#include "Knight.h"

Knight::Knight(PieceColor color) : m_color{color} {}

bool Knight::isMoveValid(const std::string &from_pos,
                         const std::string &to_pos) {
  return false;
}

char Knight::getSymbol() const { return 'N'; }

IPiece::PieceColor Knight::getColor() const { return m_color; }
