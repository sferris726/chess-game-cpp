#include "PieceUtilities.h"
#include "interfaces/IPiece.h"

#pragma once

class Pawn : public IPiece {
public:
  Pawn(PieceColor color);

  bool isMoveValid(
      const std::string &from_pos, const std::string &to_pos,
      const std::map<std::string, std::unique_ptr<IPiece>> &board_map) override;

  void setOrigin(const int col, const int row) override;

  char getSymbol() const override;

  PieceColor getColor() const override;

  std::string getColorStr() const override;

private:
  bool checkEnPassant();

  PieceColor m_color;
  bool m_first_move_made;
  std::pair<int, int> m_origin;
  int m_squares_from_origin;
};
