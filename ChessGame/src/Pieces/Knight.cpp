#include "Knight.h"

Knight::Knight(PieceColor color) : m_color{color} {}

bool Knight::isMoveValid(const std::string &from_pos, const std::string &to_pos,
                         std::unique_ptr<IPiece> to_pos_piece) {
  return false;
}

char Knight::getSymbol() const { return 'N'; }

std::string Knight::getColor() const {
  return PieceUtilities::convertPieceColorToStr(m_color);
}
