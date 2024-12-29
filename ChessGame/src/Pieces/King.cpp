#include "King.h"

King::King(PieceColor color) : m_color{color} {}

bool King::isMoveValid(const std::string &from_pos, const std::string &to_pos) {
  return false;
}

char King::getSymbol() const { return 'K'; }

std::string King::getColor() const {
  return PieceUtilities::convertPieceColorToStr(m_color);
}
