#include "Bishop.h"

Bishop::Bishop(PieceColor piece_color) : m_color{piece_color} {}

bool Bishop::isMoveValid(int start_x, int start_y, int end_x, int end_y,
                         IBoard &board) {
  return false;
}

char Bishop::getSymbol() const { return 'B'; }

IPiece::PieceColor Bishop::getColor() const { return m_color; }
