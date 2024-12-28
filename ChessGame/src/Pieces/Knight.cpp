#include "Knight.h"

Knight::Knight(PieceColor piece_color)
    : m_color{piece_color}, m_pos_x{0}, m_pos_y{0} {}

bool Knight::isMoveValid(int start_x, int start_y, int end_x, int end_y,
                         IBoard &board) {
  return false;
}

void Knight::setPosition(int x, int y) {
  m_pos_x = x;
  m_pos_y = y;
}

std::pair<int, int> Knight::getPosition() const {
  return std::pair(m_pos_x, m_pos_y);
}

char Knight::getSymbol() const { return 'N'; }

IPiece::PieceColor Knight::getColor() const { return m_color; }
