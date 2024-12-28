#include "IBoard.h"
#include <utility>

#pragma once

class IPiece {
public:
  enum class PieceColor { WHITE, BLACK };
  enum class PieceType { PAWN, BISHOP, KNIGHT, ROOK, QUEEN, KING };

  virtual bool isMoveValid(int start_x, int start_y, int end_x, int end_y,
                           IBoard &board) = 0;
  virtual char getSymbol() const = 0;
  virtual void setPosition(int x, int y) = 0;
  virtual std::pair<int, int> getPosition() const = 0;
  virtual PieceColor getColor() const = 0;
  virtual ~IPiece(){};
};
