
#include "Board.h"
#include "iostream"

static const int PAWNS = 8;
static const std::regex PIECE_PATTERN("^[rnbqRNBQ]$");

Board::Board(PieceFactory &piece_factory, ICheckMateTracker &checkmate_tracker,
             IStalemateTracker &stalemate_tracker)
    : m_piece_factory{piece_factory}, m_checkmate_tracker{checkmate_tracker},
      m_stalemate_tracker{stalemate_tracker}, m_white_king_pos{"e1"},
      m_black_king_pos{"e8"}, m_white_king_in_check{false},
      m_black_king_in_check{false}, m_end_game_info(EndGameInfo{}) {
  m_checkmate_tracker.onKingInCheckChange(
      [this](const IPiece::PieceColor color, const bool in_check) {
        handleKingInCheckUpdate(color, in_check);
      });
  m_checkmate_tracker.onCheckMate(
      [this](const IPiece::PieceColor checkmated_color) {
        handleGameOver(true, false,
                       checkmated_color == IPiece::PieceColor::WHITE ? "Black"
                                                                     : "White");
      });

  m_stalemate_tracker.onStalemate(
      [this]() { handleGameOver(false, true, ""); });
  generateBoard();
}

void Board::displayBoard() {
  std::cout << "  +----+----+----+----+----+----+----+----+" << std::endl;
  for (int i = 8; i > 0; --i) {
    std::cout << i << " |";
    for (int j = 0; j < 8; ++j) {
      std::string pos = PieceUtilities::getColLetter(j) + std::to_string(i);
      std::string display = m_board_map.at(pos)
                                ? m_board_map.at(pos)->getDisplayPiece() + " "
                                : "  ";

      if ((i + j) % 2 == 0) {
        std::cout << "\x1b[48;5;234m ";
      } else {
        std::cout << "\x1b[48;5;94m ";
      }

      if (display != "  ") {
        if (m_board_map.at(pos)->getColor() == IPiece::PieceColor::WHITE) {
          std::cout << "\x1b[38;5;15m" << display;
        } else {
          std::cout << "\x1b[1m\x1b[38;5;214m" << display;
        }
      } else {
        std::cout << "  ";
      }
      std::cout << " \x1b[0m|";
    }
    std::cout << std::endl
              << "  +----+----+----+----+----+----+----+----+" << std::endl;
  }

  std::cout << "     a    b    c    d    e    f    g    h" << std::endl;
  std::cout << "\n";
}

void Board::displayGameOver() {
  displayBoard();

  if (m_end_game_info.is_stalemate) {
    std::cout << "Game over, Stalemate!";
  } else {
    std::cout << m_end_game_info.winning_color << " win"
              << (m_end_game_info.is_checkmate ? ", Checkmate!" : "")
              << std::endl;
  }
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

  const std::string king_pos = piece_color == IPiece::PieceColor::WHITE
                                   ? m_white_king_pos
                                   : m_black_king_pos;
  const bool king_in_check = piece_color == IPiece::PieceColor::WHITE
                                 ? m_white_king_in_check
                                 : m_black_king_in_check;

  const auto move_info = m_board_map.at(from_pos)->getMoveInfo(
      from_pos, to_pos, king_pos, king_in_check, m_board_map);

  // check if move is valid
  if (!move_info.is_valid) {
    return false;
  }

  if (move_info.en_passant_valid == true) {
    const std::string opp_pos = move_info.en_passant_opponent;
    m_board_map.erase(opp_pos);
    m_board_map[opp_pos] = nullptr;
  }

  if (m_board_map.at(to_pos) != nullptr) {
    if (m_board_map.at(to_pos)->getSymbol() == 'K') {
      // King was captured, game over
      handleGameOver(false, false,
                     piece_color == IPiece::PieceColor::WHITE ? "White"
                                                              : "Black");
      return true;
    }

    m_board_map.erase(to_pos); // Captured
  }

  if (m_board_map.at(from_pos)->getSymbol() == 'K') {
    // Store the new King pos
    m_white_king_pos =
        piece_color == IPiece::PieceColor::WHITE ? to_pos : m_white_king_pos;
    m_black_king_pos =
        piece_color == IPiece::PieceColor::BLACK ? to_pos : m_black_king_pos;
  }

  if (move_info.can_pawn_promote) {
    handlePawnPromotion(to_pos, piece_color);
  } else {
    m_board_map[to_pos] = std::move(m_board_map[from_pos]);
  }
  m_board_map[from_pos] = nullptr;

  // Check and Checkmate tracking
  m_checkmate_tracker.scanBoard(IPiece::PieceColor::WHITE, m_white_king_pos,
                                piece_color != IPiece::PieceColor::WHITE,
                                m_board_map);
  m_checkmate_tracker.scanBoard(IPiece::PieceColor::BLACK, m_black_king_pos,
                                piece_color != IPiece::PieceColor::BLACK,
                                m_board_map);

  // Stalemate tracking
  if (!m_white_king_in_check && !m_black_king_in_check) {
    m_stalemate_tracker.scanBoard(IPiece::PieceColor::WHITE, m_white_king_pos,
                                  m_board_map);
    m_stalemate_tracker.scanBoard(IPiece::PieceColor::BLACK, m_black_king_pos,
                                  m_board_map);
  }

  return true;
}

bool Board::tryCastling(const IPiece::PieceColor piece_color,
                        const std::string &target_pos) {
  if ((piece_color == IPiece::PieceColor::WHITE && m_white_king_in_check) ||
      (piece_color == IPiece::PieceColor::BLACK && m_black_king_in_check)) {
    return false; // Castling not allowed if king is in check
  }

  if (m_board_map.find(target_pos) == m_board_map.end() ||
      m_board_map.at(target_pos) != nullptr) {
    return false;
  }

  std::string king_pos;
  if (piece_color == IPiece::PieceColor::WHITE) {
    king_pos = m_white_king_pos;
  } else {
    king_pos = m_black_king_pos;
  }

  if (m_board_map.at(king_pos)->getLastMove().first != "-") {
    return false; // King has made a move already
  }

  int target_col = PieceUtilities::getColNum(target_pos[0]);
  int king_col = PieceUtilities::getColNum(king_pos[0]);
  int target_diff = std::abs(king_col - target_col);
  if (target_diff != 2) {
    return false; // Castling moves king 2 spaces
  }

  int moving_col = king_col;
  bool moving_left = false;
  std::string rook_pos;

  while (true) {
    if (king_col - target_col < 0) {
      // Moving to the right
      ++moving_col;
    } else {
      // Moving to the left
      moving_left = true;
      --moving_col;
    }

    if (moving_col < 0 || moving_col > 7) {
      return false; // Out of bounds
    }

    std::string new_pos =
        PieceUtilities::getColLetter(moving_col) + king_pos[1];
    if (m_board_map.at(new_pos) &&
        m_board_map.at(new_pos)->getSymbol() == 'R' &&
        m_board_map.at(new_pos)->getColor() == piece_color) {
      if (m_board_map.at(new_pos)->getLastMove().first == "-") {
        rook_pos = new_pos;
        break;
      } else {
        return false; // Rook has made a move before
      }
    }

    if (m_board_map.at(new_pos) != nullptr) {
      return false; // Piece between Rook and King exists
    }
  }

  std::string pos1;
  std::string pos2;
  int target_rook_col;
  if (moving_left) {
    pos1 = PieceUtilities::getColLetter(king_col - 1) + king_pos[1];
    pos2 = PieceUtilities::getColLetter(king_col - 2) + king_pos[1];
    target_rook_col = target_col + 1;
  } else {
    pos1 = PieceUtilities::getColLetter(king_col + 1) + king_pos[1];
    pos2 = PieceUtilities::getColLetter(king_col + 2) + king_pos[1];
    target_rook_col = target_col - 1;
  }

  // Scan to make sure castling move would not put King in check or through
  // check
  if (!m_checkmate_tracker.castlingScan(piece_color, pos1, pos2, m_board_map)) {
    return false;
  }

  // Move King
  m_board_map[target_pos] = std::move(m_board_map.at(king_pos));
  m_board_map[king_pos] = nullptr;
  // Move Rook
  std::string new_rook_pos =
      PieceUtilities::getColLetter(target_rook_col) + rook_pos[1];
  m_board_map[new_rook_pos] = std::move(m_board_map.at(rook_pos));
  m_board_map[rook_pos] = nullptr;

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

void Board::handleKingInCheckUpdate(const IPiece::PieceColor color,
                                    const bool in_check) {
  if (color == IPiece::PieceColor::WHITE) {
    m_white_king_in_check = in_check;
  } else {
    m_black_king_in_check = in_check;
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

void Board::handleGameOver(const bool is_checkmate, const bool is_stalemate,
                           const std::string &winning_color) {
  m_end_game_info.is_checkmate = is_checkmate;
  m_end_game_info.is_stalemate = is_stalemate;
  m_end_game_info.winning_color = winning_color;
  m_game_over_callback();
}
