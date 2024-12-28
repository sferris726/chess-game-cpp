#include "IBoard.h"

#pragma once

class IPiece {
public:
  enum class PieceColor {
    WHITE,
    BLACK
  };

  virtual bool isMoveValid(int start_x, int start_y, int end_x, int end_y,
                           IBoard &board) = 0;
  virtual char getSymbol() const = 0;
  virtual PieceColor getColor() const = 0;
  virtual ~IPiece(){};
};