#include <cstdlib>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

#pragma once

class IPiece {
public:
  enum class PieceColor { WHITE, BLACK };
  enum class PieceType { PAWN, BISHOP, KNIGHT, ROOK, QUEEN, KING };

  virtual bool isMoveValid(
      const std::string &from_pos, const std::string &to_pos,
      const std::map<std::string, std::unique_ptr<IPiece>> &board_map) = 0;
  virtual char getSymbol() const = 0;
  virtual std::string getColor() const = 0;
  virtual ~IPiece(){};
};
