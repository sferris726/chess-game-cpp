#include "Pawn.h"

Pawn::Pawn(PieceColor color) : m_color{color} {}

bool Pawn::isMoveValid(const std::string &from_pos, const std::string &to_pos) {
  return false;
}

char Pawn::getSymbol() const { return 'P'; }

std::string Pawn::getColor() const {
  return PieceUtilities::convertPieceColorToStr(m_color);
}
