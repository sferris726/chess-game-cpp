#include "interfaces/IPiece.h"
#include <functional>
#include <set>
#include <string>

#pragma once

class IStalemateTracker {
public:
  virtual void scanBoard(
      const IPiece::PieceColor king_color, const std::string &king_pos,
      const std::map<std::string, std::unique_ptr<IPiece>> &board_map) = 0;

  virtual void onStalemate(std::function<void()> callback) = 0;
};
