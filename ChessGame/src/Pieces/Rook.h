#include "IPiece.h"

#pragma once

class Rook : public IPiece {
public:
  Rook(PieceColor piece_color);

  bool isMoveValid(int start_x, int start_y, int end_x, int end_y,
                   IBoard &board) override;

  char getSymbol() const override;

  PieceColor getColor() const override;

  private:
  PieceColor m_color;
};