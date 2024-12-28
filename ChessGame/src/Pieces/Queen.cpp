#include "Queen.h"

Queen::Queen(PieceColor piece_color) : m_color{piece_color} {}

bool Queen::isMoveValid(int start_x, int start_y, int end_x, int end_y,
                       IBoard &board) {
  return false;
}

char Queen::getSymbol() const { return 'Q'; }

IPiece::PieceColor Queen::getColor() const {
  return m_color;
}
