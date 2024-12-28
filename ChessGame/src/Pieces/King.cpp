#include "King.h"

King::King(PieceColor piece_color) : m_color{piece_color} {}

bool King::isMoveValid(int start_x, int start_y, int end_x, int end_y,
                       IBoard &board) {
  return false;
}

char King::getSymbol() const { return 'K'; }

IPiece::PieceColor King::getColor() const {
  return m_color;
}
