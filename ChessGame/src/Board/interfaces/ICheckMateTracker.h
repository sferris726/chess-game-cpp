#include "interfaces/IPiece.h"
#include <functional>
#include <set>
#include <string>

#pragma once

class ICheckMateTracker {
public:
  enum class Direction {
    NORTH,
    NORTH_EAST,
    EAST,
    SOUTH_EAST,
    SOUTH,
    SOUTH_WEST,
    WEST,
    NORTH_WEST
  };

  virtual void scanBoard(
      const IPiece::PieceColor king_color,
      const std::map<std::string, std::unique_ptr<IPiece>> &board_map) = 0;
};
