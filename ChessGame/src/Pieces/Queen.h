#include "interfaces/IPiece.h"

#pragma once

class Queen : public IPiece {
public:
  Queen(PieceColor color);

  bool isMoveValid(const std::string &from_pos,
                   const std::string &to_pos) override;

  char getSymbol() const override;

  PieceColor getColor() const override;

private:
  PieceColor m_color;
};
