#include "IPiece.h"

#pragma once

class Pawn : public IPiece {
public:
  Pawn();

  bool isMoveValid(int start_x, int start_y, int end_x, int end_y,
                   IBoard &board) override;

  char getSymbol() const override;
};