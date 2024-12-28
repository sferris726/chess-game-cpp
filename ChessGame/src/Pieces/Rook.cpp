#include "Rook.h"

Rook::Rook(PieceColor piece_color)
    : m_color{piece_color}, m_pos_x{0}, m_pos_y{0} {}

bool Rook::isMoveValid(int start_x, int start_y, int end_x, int end_y,
                       IBoard &board) {
  return false;
}

void Rook::setPosition(int x, int y) {
  m_pos_x = x;
  m_pos_y = y;
}

std::pair<int, int> Rook::getPosition() const {
  return std::pair(m_pos_x, m_pos_y);
}

char Rook::getSymbol() const { return 'R'; }

IPiece::PieceColor Rook::getColor() const { return m_color; }
