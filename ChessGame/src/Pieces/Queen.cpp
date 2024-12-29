#include "Queen.h"

Queen::Queen(PieceColor color) : m_color{color} {}

bool Queen::isMoveValid(const std::string &from_pos,
                        const std::string &to_pos) {
  return false;
}

char Queen::getSymbol() const { return 'Q'; }

std::string Queen::getColor() const {
  return PieceUtilities::convertPieceColorToStr(m_color);
}
