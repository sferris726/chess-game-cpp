#include "PieceUtilities.h"
#include "interfaces/IPiece.h"

#pragma once

class Rook : public IPiece {
public:
  Rook(PieceColor color);

  bool isMoveValid(const std::string &from_pos, const std::string &to_pos,
                   std::unique_ptr<IPiece> to_pos_piece) override;

  char getSymbol() const override;

  std::string getColor() const override;

private:
  PieceColor m_color;
};
