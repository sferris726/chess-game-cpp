#include "IStalemateTracker.h"
#include "InputHandler.h"
#include "PieceUtilities.h"
#include "interfaces/IPiece.h"

#pragma once

class StalemateTracker : public IStalemateTracker {
public:
  using Direction = IPiece::Direction;

  StalemateTracker();

  void scanBoard(
      const IPiece::PieceColor king_color, const std::string &king_pos,
      const std::map<std::string, std::unique_ptr<IPiece>> &board_map) override;

  void onStalemate(std::function<void()> callback) override;

private:
  bool doesKingHaveLegalMove(
      const IPiece::PieceColor king_color, const std::string &king_pos,
      const std::map<std::string, std::unique_ptr<IPiece>> &board_map);

  bool checkLPatternThreat(
      const IPiece::PieceColor king_color, const int king_col,
      const int king_row,
      const std::map<std::string, std::unique_ptr<IPiece>> &board_map);

  std::vector<std::string> getMovablePositionsFromPosition(
      const IPiece::PieceColor piece_color, const std::string &curr_pos,
      const std::map<std::string, std::unique_ptr<IPiece>> &board_map);

  std::vector<std::string> getPossiblePositionsForKnight(const int curr_col,
                                                         const int curr_row);

  std::function<void()> m_stalemate_callback;
};
