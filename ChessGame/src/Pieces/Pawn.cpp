#include "Pawn.h"

Pawn::Pawn(PieceColor piece_color) : m_color{piece_color} {}

bool Pawn::isMoveValid(int start_x, int start_y, int end_x, int end_y,
                       IBoard &board) {
  return false;
}

char Pawn::getSymbol() const { return 'P'; }

IPiece::PieceColor Pawn::getColor() const { return m_color; }
