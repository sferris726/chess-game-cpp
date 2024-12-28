#include "King.h"

King::King(PieceColor piece_color)
    : m_color{piece_color}, m_pos_x{0}, m_pos_y{0} {}

bool King::isMoveValid(int start_x, int start_y, int end_x, int end_y,
                       IBoard &board) {
  return false;
}

void King::setPosition(int x, int y) {
  m_pos_x = x;
  m_pos_y = y;
}

std::pair<int, int> King::getPosition() const {
  return std::pair(m_pos_x, m_pos_y);
}

char King::getSymbol() const { return 'K'; }

IPiece::PieceColor King::getColor() const { return m_color; }
