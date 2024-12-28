#include "Bishop.h"

Bishop::Bishop(PieceColor piece_color)
    : m_color{piece_color}, m_pos_x{0}, m_pos_y{0} {}

bool Bishop::isMoveValid(int start_x, int start_y, int end_x, int end_y,
                         IBoard &board) {
  return false;
}

void Bishop::setPosition(int x, int y) {
  m_pos_x = x;
  m_pos_y = y;
}

std::pair<int, int> Bishop::getPosition() const {
  return std::pair(m_pos_x, m_pos_y);
}

char Bishop::getSymbol() const { return 'B'; }

IPiece::PieceColor Bishop::getColor() const { return m_color; }
