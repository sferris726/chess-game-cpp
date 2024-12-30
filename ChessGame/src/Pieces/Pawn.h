#include "PieceUtilities.h"
#include "interfaces/IPiece.h"

#pragma once

class Pawn : public IPiece {
public:
  Pawn(PieceColor color);

  MoveInfo getMoveInfo(
      const std::string &from_pos, const std::string &to_pos,
      const std::map<std::string, std::unique_ptr<IPiece>> &board_map) override;

  void setOrigin(const int col, const int row) override;

  char getSymbol() const override;

  PieceColor getColor() const override;

  std::string getColorStr() const override;

  std::pair<std::string, std::string> getLastMove() const override;

private:
  /**
   * @brief Check whether the en passant move is valid
   *
   * @param board_map
   * @param start Piece start pos
   * @param end Piece end pos
   * @returns a pair [valid, opponent position]
   */
  std::pair<bool, std::string> checkEnPassant(
      const std::map<std::string, std::unique_ptr<IPiece>> &board_map,
      const std::pair<int, int> &start, const std::pair<int, int> &end);

  bool enablePawnPromotion(int row);

  PieceColor m_color;
  bool m_first_move_made;
  std::pair<int, int> m_origin;
  std::pair<std::string, std::string> m_last_move;
};
