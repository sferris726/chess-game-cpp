#include "PieceFactory.h"

PieceFactory::PieceFactory() {}

std::unique_ptr<IPiece> PieceFactory::createPiece(IPiece::PieceType piece,
                                                  IPiece::PieceColor color) {
  switch (piece) {
  case IPiece::PieceType::PAWN:
    return std::make_unique<Pawn>(color);
  case IPiece::PieceType::BISHOP:
    return std::make_unique<Bishop>(color);
  case IPiece::PieceType::ROOK:
    return std::make_unique<Rook>(color);
  case IPiece::PieceType::KNIGHT:
    return std::make_unique<Knight>(color);
  case IPiece::PieceType::QUEEN:
    return std::make_unique<Queen>(color);
  case IPiece::PieceType::KING:
    return std::make_unique<King>(color);
  default:
    throw std::runtime_error("Invalid Chess Piece");
  }
}
