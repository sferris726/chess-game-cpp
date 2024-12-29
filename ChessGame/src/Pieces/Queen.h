#include "PieceUtilities.h"
#include "interfaces/IPiece.h"

#pragma once

class Queen : public IPiece {
public:
  Queen(PieceColor color);

  bool isMoveValid(
      const std::string &from_pos, const std::string &to_pos,
      const std::map<std::string, std::unique_ptr<IPiece>> &board_map) override;

  char getSymbol() const override;

  std::string getColor() const override;

private:
  PieceColor m_color;
};
