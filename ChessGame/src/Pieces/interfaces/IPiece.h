#include <cstdlib>
#include <iostream>
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

  struct MoveInfo {
    bool is_valid;
    bool en_passant_valid;
    bool castling_valid;
    std::string en_passant_opponent;
    std::pair<std::string, std::string>
        castling_pair; // Where the [King, Rook] should move
  };

  virtual MoveInfo getMoveInfo(
      const std::string &from_pos, const std::string &to_pos,
      const std::map<std::string, std::unique_ptr<IPiece>> &board_map) = 0;
  virtual void setOrigin(const int col, const int row) = 0;
  virtual char getSymbol() const = 0;
  virtual PieceColor getColor() const = 0;
  virtual std::string getColorStr() const = 0;
  virtual std::pair<std::string, std::string> getLastMove() const = 0;
  virtual ~IPiece(){};
};
