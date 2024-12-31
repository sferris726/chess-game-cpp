
#include "Board.h"
#include "iostream"

static const int PAWNS = 8;
static const std::regex PIECE_PATTERN("^[rnbqRNBQ]$");

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
                    << m_board_map[board_pos]->getColorStr() << " ]";
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
  // make sure from_pos and to_pos are different and from_pos has a piece
  if (from_pos == to_pos || m_board_map.at(from_pos) == nullptr) {
    return false;
  }

  // make sure from_pos is the correct color
  if (m_board_map.at(from_pos)->getColor() != piece_color) {
    return false;
  }

  const auto move_info =
      m_board_map.at(from_pos)->getMoveInfo(from_pos, to_pos, m_board_map);
  // check if move is valid
  if (!move_info.is_valid) {
    return false;
  }

  if (move_info.en_passant_valid == true) {
    const std::string opp_pos = move_info.en_passant_opponent;
    if (piece_color == IPiece::PieceColor::WHITE) {
      // Black was captured
      m_black_pieces_captured.push_back(std::move(m_board_map.at(opp_pos)));
    } else {
      // White captured
      m_white_pieces_captured.push_back(std::move(m_board_map.at(opp_pos)));
    }
    m_board_map[opp_pos] = nullptr;
  }

  // TODO: Castling

  if (m_board_map.at(to_pos) != nullptr) {
    if (piece_color == IPiece::PieceColor::WHITE) {
      // Black was captured
      m_black_pieces_captured.push_back(std::move(m_board_map.at(to_pos)));
    } else {
      // White captured
      m_white_pieces_captured.push_back(std::move(m_board_map.at(to_pos)));
    }
  }

  if (move_info.can_pawn_promote) {
    handlePawnPromotion(to_pos, piece_color);
  } else {
    m_board_map[to_pos] = std::move(m_board_map[from_pos]);
  }

  m_board_map[from_pos] = nullptr;
  return true;
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

  for (const auto &[pos, piece] : m_board_map) {
    if (piece != nullptr) {
      piece->setOrigin(PieceUtilities::getColNum(pos[0]), std::atoi(&pos[1]));
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

void Board::handlePawnPromotion(const std::string &pos,
                                const IPiece::PieceColor piece_color) {
  std::string promotion_piece = InputHandler::getInput(
      "Pawn promotion avaiable, enter the piece you would like to promote to:");
      
  if (promotion_piece.length() == 1 &&
      std::regex_match(promotion_piece, PIECE_PATTERN)) {
    std::transform(promotion_piece.begin(), promotion_piece.end(),
                   promotion_piece.begin(), ::toupper);
    const IPiece::PieceType piece =
        PieceUtilities::convertStrToPieceType(promotion_piece);
    m_board_map.erase(pos); // Remove the pawn
    m_board_map.emplace(pos, m_piece_factory.createPiece(
                                 piece, piece_color)); // Emplace promoted piece
  } else {
    std::cout << "Invalid input, please enter a valid letter:\nQ = Queen\nR = "
                 "Rook\nN = Knight\nB = Bishop\n";
    handlePawnPromotion(pos, piece_color);
  }
}
