#include "Knight.h"

Knight::Knight(PieceColor piece_color) : m_color{piece_color} {}

bool Knight::isMoveValid(int start_x, int start_y, int end_x, int end_y,
                         IBoard &board) {
  return false;
}

char Knight::getSymbol() const { return 'N'; }

IPiece::PieceColor Knight::getColor() const { return m_color; }
