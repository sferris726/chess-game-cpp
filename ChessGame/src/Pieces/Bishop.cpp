#include "Bishop.h"

Bishop::Bishop(PieceColor color) : m_color{color} {}

bool Bishop::isMoveValid(const std::string &from_pos,
                         const std::string &to_pos) {
  return false;
}

char Bishop::getSymbol() const { return 'B'; }

std::string Bishop::getColor() const {
  return PieceUtilities::convertPieceColorToStr(m_color);
}
