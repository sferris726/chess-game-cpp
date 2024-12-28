#include "Pawn.h"

Pawn::Pawn(PieceColor piece_color)
    : m_color{piece_color}, m_pos_x{0}, m_pos_y{0} {}

bool Pawn::isMoveValid(int start_x, int start_y, int end_x, int end_y,
                       IBoard &board) {
  return false;
}

void Pawn::setPosition(int x, int y) {
  m_pos_x = x;
  m_pos_y = y;
}

std::pair<int, int> Pawn::getPosition() const {
  return std::pair(m_pos_x, m_pos_y);
}

char Pawn::getSymbol() const { return 'P'; }

IPiece::PieceColor Pawn::getColor() const { return m_color; }
