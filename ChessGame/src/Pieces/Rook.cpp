#include "Rook.h"

Rook::Rook(PieceColor color) : m_color{color} {}

bool Rook::isMoveValid(
    const std::string &from_pos, const std::string &to_pos,
    const std::map<std::string, std::unique_ptr<IPiece>> &board_map) {
  return false;
}

char Rook::getSymbol() const { return 'R'; }

std::string Rook::getColor() const {
  return PieceUtilities::convertPieceColorToStr(m_color);
}
