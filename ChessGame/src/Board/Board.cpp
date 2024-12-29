
#include "Board.h"
#include "iostream"

static const int PAWNS = 8;

Board::Board(PieceFactory &piece_factory) : m_piece_factory{piece_factory} {
  generateBoard();
}

void Board::displayBoard() {
  for (int i = 8; i >= 0; --i) {
    if (i > 0) {
      std::cout << std::to_string(i) << " ";
    } else {
      std::cout << "  ";
    }

    for (int j = 0; j < 8; ++j) {
      if (i == 0) {
        std::cout << "   " << PieceUtilities::getColLetter(j) << "  ";
      } else {
        std::cout << "[ ";
        const std::string board_pos =
            PieceUtilities::getColLetter(j) + std::to_string(i);

        if (m_board_map[board_pos]) {
          std::cout << m_board_map[board_pos]->getSymbol()
                    << m_board_map[board_pos]->getColor() << " ]";
        } else {
          std::cout << "   ]";
        }
      }
    }

    std::cout << "\n";
  }

  std::cout << "\n";
}

bool Board::movePiece(const IPiece::PieceColor piece_color,
                      const std::string &from_pos, const std::string &to_pos) {
  // check if board spot is occupied
  // check the occupied piece is right color
  // check if move is valid
  // use std::move to move ownership of the ptr
  return false;
}

void Board::onGameOver(std::function<void()> callback) {
  m_game_over_callback = callback;
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
        m_board_map.emplace(PieceUtilities::getColLetter(j) + std::to_string(i),
                            nullptr);
      }
    }
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
  for (int i = 0; i < PAWNS; ++i) {
    const std::string col_letter = PieceUtilities::getColLetter(i);
    const std::string key = color == IPiece::PieceColor::WHITE
                                ? col_letter + "2"
                                : col_letter + "7";
    in_map.emplace(key,
                   m_piece_factory.createPiece(IPiece::PieceType::PAWN, color));
  }
}
