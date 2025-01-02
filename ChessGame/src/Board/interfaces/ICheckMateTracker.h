#include "interfaces/IPiece.h"
#include <functional>
#include <set>
#include <string>

#pragma once

class ICheckMateTracker {
public:
  virtual void scanBoard(
      const IPiece::PieceColor king_color, const std::string &king_pos,
      const bool has_next_move,
      const std::map<std::string, std::unique_ptr<IPiece>> &board_map) = 0;

  /**
   * @brief Scan if the positions the King is moving through can generate check
   *
   * @param color
   * @param pos1
   * @param pos2
   * @param board_map
   * @returns True if not in check at either position, false otherwise
   */
  virtual bool castlingScan(
      const IPiece::PieceColor color, const std::string &pos1,
      const std::string &pos2,
      const std::map<std::string, std::unique_ptr<IPiece>> &board_map) = 0;

  virtual void onKingInCheckChange(
      std::function<void(const IPiece::PieceColor color, const bool in_check)>
          callback) = 0;

  virtual void onCheckMate(std::function<void()> callback) = 0;
};
