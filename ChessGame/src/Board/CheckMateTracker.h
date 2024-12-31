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

  bool castlingScan(
      const IPiece::PieceColor color, const std::string &pos1,
      const std::string &pos2,
      const std::map<std::string, std::unique_ptr<IPiece>> &board_map) override;

  void onKingInCheckChange(
      std::function<void(const IPiece::PieceColor color, const bool in_check)>
          callback) override;

  void onCheckMate(std::function<void()> callback) override;

private:
  /**
   * @brief Check L Shape attack threats
   *
   * @param king_pos
   * @param knight_pos
   * @returns Pair of if the king is in check and Directions threatened that
   * should be excluded
   */
  std::pair<bool, std::set<Direction>>
  checkLPatternThreat(const std::string &king_pos,
                      const std::string &knight_pos);

  /**
   * @brief Is the king in check for this direction and is the direction safe to
   * move out of check
   *
   * @param direction The direction the King can go
   * @param king_pos The current position of the King
   * @param king_color The piece color
   * @param board_map The board
   * @returns Pair of [is_check, is_direction_movable]
   */
  std::pair<bool, bool> isCheckAndDirectionMovable(
      Direction direction, const std::string &king_pos,
      const IPiece::PieceColor king_color,
      const std::map<std::string, std::unique_ptr<IPiece>> &board_map);

  bool inBoundsCheck(Direction direction, const std::string &pos);

  bool isOneRankFromKing(Direction direction, int king_col, int king_row,
                         const std::string &piece_pos);

  std::function<void(const IPiece::PieceColor color, const bool in_check)>
      m_king_in_check_callback;
  std::function<void()> m_checkmate_callback;
  bool m_white_king_in_check;
  bool m_black_king_in_check;
};
