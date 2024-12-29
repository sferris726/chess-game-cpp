#include "PieceUtilities.h"
#include "interfaces/IPiece.h"

#pragma once

class King : public IPiece {
public:
  King(PieceColor color);

  bool isMoveValid(const std::string &from_pos,
                   const std::string &to_pos) override;

  char getSymbol() const override;

  std::string getColor() const override;

private:
  PieceColor m_color;
};
