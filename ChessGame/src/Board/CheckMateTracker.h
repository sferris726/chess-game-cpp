#include "ICheckMateTracker.h"
#include "InputHandler.h"
#include "PieceUtilities.h"
#include "interfaces/IPiece.h"

#pragma once

class CheckMateTracker : public ICheckMateTracker {
public:
  using Direction = IPiece::Direction;

  CheckMateTracker();

  void scanBoard(
      const IPiece::PieceColor king_color, const std::string &king_pos,
      const bool has_next_move,
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
  struct ThreatInfo {
    bool has_check;
    bool direction_movable_for_king;
    std::string threat_pos;
  };

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
   * @returns ThreatInfo
   */
  ThreatInfo getThreatInfo(
      Direction direction, const std::string &king_pos,
      const IPiece::PieceColor king_color, const bool king_has_next_turn,
      const bool potential_king_pos,
      const std::map<std::string, std::unique_ptr<IPiece>> &board_map);

  /**
   * @brief King is in check and cannot move, scan if there are pieces that can
   * move to protect the king
   *
   * @param color
   * @param king_pos
   * @param direction
   * @param board_map
   * @returns If the King can be protected or not
   */
  bool scanForProtections(
      const IPiece::PieceColor color, const std::string &king_pos,
      const std::string &threat_pos, const Direction direction,
      const std::map<std::string, std::unique_ptr<IPiece>> &board_map);

  bool inBoundsCheck(Direction direction, const std::string &pos);

  bool isOneRankFromKing(Direction direction, int king_col, int king_row,
                         const std::string &piece_pos);

  std::vector<std::string> getOpposingPiecesInDirection(
      const Direction direction, const IPiece::PieceColor color, int start_col,
      int start_row,
      const std::map<std::string, std::unique_ptr<IPiece>> &board_map);

  bool canKingBeProtected(
      const IPiece::PieceColor ally_color, const std::string &king_pos,
      const std::string &threat_pos, const Direction threat_direction,
      const std::map<std::string, std::unique_ptr<IPiece>> &board_map);

  bool canPieceMoveIntoBounds(
      const IPiece &piece, const std::string &piece_pos,
      const std::vector<std::pair<int, int>> bounds,
      const std::map<std::string, std::unique_ptr<IPiece>> &board_map);

  void moveDirection(Direction direction, int &col, int &row);

  void checkUpdate(const bool king_in_check, const IPiece::PieceColor color);

  std::function<void(const IPiece::PieceColor color, const bool in_check)>
      m_king_in_check_callback;
  std::function<void()> m_checkmate_callback;
  bool m_white_king_in_check;
  bool m_black_king_in_check;
};
