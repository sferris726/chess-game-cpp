
#include "Board.h"
#include "iostream"

static const int PAWNS = 8;
static const int B_K_R = 2; // Bishops, Knights and Rooks are all 2 pieces

Board::Board(PieceFactory &piece_factory) : m_piece_factory{piece_factory} {}

void Board::displayBoard() {
  for (int i = 0; i < 8; ++i) {
    for (int j = 0; j < 8; ++j) {
      std::cout << "[]";
    }
    std::cout << std::endl;
  }
}

bool Board::movePiece(const std::string &from_pos, const std::string &to_pos) {
  // check if move is valid in manager
  // check if board spot is occupied
  return false;
}

void Board::generateBoard() {
  for (int i = 1; i <= 8; ++i) {
    if (i == 1 || i == 8) {
      IPiece::PieceColor color =
          i == 1 ? IPiece::PieceColor::WHITE : IPiece::PieceColor::BLACK;
      generateNonPawnRow(m_board_map, color);
    } else if (i == 2 || i == 7) {
      IPiece::PieceColor color =
          i == 2 ? IPiece::PieceColor::WHITE : IPiece::PieceColor::BLACK;
      generatePawnRow(m_board_map, color);
    } else {
      for (int j = 0; j < 8; ++j) {
        m_board_map.emplace(getColLetter(j) + std::to_string(i), nullptr);
      }
    }
  }
}

std::string Board::getColLetter(int col) {
  switch (col) {
  case 0:
    return "a";
  case 1:
    return "b";
  case 2:
    return "c";
  case 3:
    return "d";
  case 4:
    return "e";
  case 5:
    return "f";
  case 6:
    return "g";
  case 7:
    return "h";
  default:
    throw std::runtime_error("Invalid Col passed");
  }
}

void Board::generateNonPawnRow(
    std::map<std::string, std::unique_ptr<IPiece>> &in_map,
    IPiece::PieceColor color) {
  in_map.emplace(color == IPiece::PieceColor::WHITE ? "a1" : "a8",
                 m_piece_factory.createPiece(IPiece::PieceType::ROOK, color));
  in_map.emplace(color == IPiece::PieceColor::WHITE ? "b1" : "b8",
                 m_piece_factory.createPiece(IPiece::PieceType::KNIGHT, color));
  in_map.emplace(color == IPiece::PieceColor::WHITE ? "c1" : "c8",
                 m_piece_factory.createPiece(IPiece::PieceType::BISHOP, color));
  in_map.emplace(color == IPiece::PieceColor::WHITE ? "d1" : "d8",
                 m_piece_factory.createPiece(IPiece::PieceType::QUEEN, color));
  in_map.emplace(color == IPiece::PieceColor::WHITE ? "e1" : "e8",
                 m_piece_factory.createPiece(IPiece::PieceType::KING, color));
  in_map.emplace(color == IPiece::PieceColor::WHITE ? "f1" : "f8",
                 m_piece_factory.createPiece(IPiece::PieceType::BISHOP, color));
  in_map.emplace(color == IPiece::PieceColor::WHITE ? "g1" : "g8",
                 m_piece_factory.createPiece(IPiece::PieceType::KNIGHT, color));
  in_map.emplace(color == IPiece::PieceColor::WHITE ? "h1" : "h8",
                 m_piece_factory.createPiece(IPiece::PieceType::ROOK, color));
}

void Board::generatePawnRow(
    std::map<std::string, std::unique_ptr<IPiece>> &in_map,
    IPiece::PieceColor color) {
  for (int i = 0; i < 8; ++i) {
    const std::string key = color == IPiece::PieceColor::WHITE
                                ? getColLetter(i) + "2"
                                : getColLetter(i) + "7";
    in_map.emplace(key,
                   m_piece_factory.createPiece(IPiece::PieceType::PAWN, color));
  }
}
