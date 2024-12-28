#include "Pawn.h"

Pawn::Pawn() {}

bool Pawn::isMoveValid(int start_x, int start_y, int end_x, int end_y,
                       IBoard &board) {
  return false;
}

char Pawn::getSymbol() const { return 'P'; }
