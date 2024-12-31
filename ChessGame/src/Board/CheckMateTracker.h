#include "ICheckMateTracker.h"
#include "InputHandler.h"
#include "PieceUtilities.h"
#include "interfaces/IPiece.h"

#pragma once

class CheckMateTracker : public ICheckMateTracker {
public:
  CheckMateTracker();

  // first for loop is to find the king pos
  // Do a nested for loop -> outer loop is 0-8. Inner loop go clockwise to check
  // every direction (N, NE, E, SE, S, SW, W, NW) call this function for the
  // opposite color that just moved
  void scanBoard(
      const IPiece::PieceColor king_color,
      const std::map<std::string, std::unique_ptr<IPiece>> &board_map) override;

private:
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

  bool isDirectionSafe(
      Direction direction, const std::string &king_pos,
      const IPiece::PieceColor king_color,
      const std::map<std::string, std::unique_ptr<IPiece>> &board_map);

  static std::array<Direction, 8> m_directions;
};
