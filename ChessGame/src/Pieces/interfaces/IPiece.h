#include <string>
#include <utility>

#pragma once

class IPiece {
public:
  enum class PieceColor { WHITE, BLACK };
  enum class PieceType { PAWN, BISHOP, KNIGHT, ROOK, QUEEN, KING };

  virtual bool isMoveValid(const std::string &from_pos,
                           const std::string &to_pos) = 0;
  virtual char getSymbol() const = 0;
  virtual PieceColor getColor() const = 0;
  virtual ~IPiece(){};
};
