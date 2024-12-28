#include "Bishop.h"
#include "King.h"
#include "Knight.h"
#include "Pawn.h"
#include "Queen.h"
#include "Rook.h"
#include "interfaces/IPiece.h"
#include <memory>
#include <stdexcept>

#pragma once

class PieceFactory {
public:
  PieceFactory();

  std::unique_ptr<IPiece> createPiece(IPiece::PieceType piece, IPiece::PieceColor color);
};
