#include "Rook.h"

Rook::Rook(PieceColor piece_color) : m_color{piece_color} {}

bool Rook::isMoveValid(int start_x, int start_y, int end_x, int end_y,
                       IBoard &board) {
  return false;
}

char Rook::getSymbol() const { return 'R'; }

IPiece::PieceColor Rook::getColor() const { return m_color; }
