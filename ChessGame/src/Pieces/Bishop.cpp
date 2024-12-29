#include "Bishop.h"

Bishop::Bishop(PieceColor color) : m_color{color} {}

bool Bishop::isMoveValid(const std::string &from_pos,
                         const std::string &to_pos) {
  return false;
}

char Bishop::getSymbol() const { return 'B'; }

IPiece::PieceColor Bishop::getColor() const { return m_color; }
