#include <cstdlib>
#include <iostream>
#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#pragma once

class IPiece {
public:
  enum class PieceColor { WHITE, BLACK };
  enum class PieceType { NONE, PAWN, BISHOP, KNIGHT, ROOK, QUEEN, KING };
  enum class AttackPattern {
    VERTICAL_ALL,
    HORIZONTAL_ALL,
    VERTICAL_ONE,
    HORIZONTAL_ONE,
    DIAGONAL_ALL,
    DIAGONAL_ONE,
    L_SHAPE
  };

  struct MoveInfo {
    bool is_valid;
    bool en_passant_valid;
    bool castling_valid;
    bool can_pawn_promote;
    std::string en_passant_opponent;
    std::pair<std::string, std::string>
        castling_locations; // Where the [King, Rook] should move
  };

  virtual MoveInfo getMoveInfo(
      const std::string &from_pos, const std::string &to_pos,
      const std::map<std::string, std::unique_ptr<IPiece>> &board_map) = 0;
  virtual void setOrigin(const int col, const int row) = 0;
  virtual char getSymbol() const = 0;
  virtual PieceColor getColor() const = 0;
  virtual std::string getColorStr() const = 0;
  virtual std::pair<std::string, std::string> getLastMove() const = 0;
  virtual std::vector<AttackPattern> getAttackPatterns() const = 0;
  virtual ~IPiece(){};
};
