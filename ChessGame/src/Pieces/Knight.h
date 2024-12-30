#include "PieceUtilities.h"
#include "interfaces/IPiece.h"

#pragma once

class Knight : public IPiece {
public:
  Knight(PieceColor color);

  bool isMoveValid(
      const std::string &from_pos, const std::string &to_pos,
      const std::map<std::string, std::unique_ptr<IPiece>> &board_map) override;

  void setOrigin(const int col, const int row) override;

  char getSymbol() const override;

  PieceColor getColor() const override;

  std::string getColorStr() const override;

private:
  PieceColor m_color;
  std::pair<int, int> m_origin;
};
