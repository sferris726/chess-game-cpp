#include "StalemateTracker.h"

StalemateTracker::StalemateTracker() {}

void StalemateTracker::onStalemate(std::function<void()> callback) {
  m_stalemate_callback = callback;
}

void StalemateTracker::scanBoard(
    const IPiece::PieceColor king_color, const std::string &king_pos,
    const std::map<std::string, std::unique_ptr<IPiece>> &board_map) {
  const bool king_has_legal_move =
      doesKingHaveLegalMove(king_color, king_pos, board_map);

  if (!king_has_legal_move) {
    std::set<std::string> excluded_positions;
    int king_col = PieceUtilities::getColNum(king_pos[0]);
    int king_row = std::atoi(&king_pos[1]);

    for (const auto direction : PieceUtilities::DIRECTIONS) {
      int c = king_col;
      int r = king_row;

      while (true) {
        PieceUtilities::moveDirection(direction, c, r);
        std::string new_pos =
            PieceUtilities::getColLetter(c) + std::to_string(r);
        if (board_map.find(new_pos) == board_map.end()) {
          break;
        }

        if (board_map.at(new_pos) != nullptr) {
          if (board_map.at(new_pos)->getColor() == king_color) {
            // Check possible positions piece can move to and if it will leave
            // King in check
            auto movable_positions =
                getMovablePositionsFromPosition(king_color, new_pos, board_map);
            for (const std::string &pos : movable_positions) {
              if (!PieceUtilities::doesMovePutKingInCheck(
                      king_color, king_pos, new_pos, pos, board_map)) {
                return;
              }
            }

            if (movable_positions.size() == 0) {
              excluded_positions.insert(
                  new_pos); // This piece has no legal move
            }
          }
        }
      }
    }

    for (const auto &[pos, piece] : board_map) {
      if (piece && piece->getColor() == king_color) {
        if (excluded_positions.count(pos) > 0) {
          if (getMovablePositionsFromPosition(king_color, pos, board_map)
                  .size() > 0) {
            return; // Piece still has a legal move
          }
        }
      }
    }

    m_stalemate_callback();
  }
}

bool StalemateTracker::doesKingHaveLegalMove(
    const IPiece::PieceColor king_color, const std::string &king_pos,
    const std::map<std::string, std::unique_ptr<IPiece>> &board_map) {
  int king_col = PieceUtilities::getColNum(king_pos[0]);
  int king_row = std::atoi(&king_pos[1]);
  // Gather the possible moves for the King
  std::vector<std::string> possible_king_moves;
  for (const auto direction : PieceUtilities::DIRECTIONS) {
    int c = king_col;
    int r = king_row;
    PieceUtilities::moveDirection(direction, c, r);
    if ((c >= 0 && c < 8) && (r > 0 && r <= 8)) {
      std::string pos = PieceUtilities::getColLetter(c) + std::to_string(r);
      if (board_map.at(pos) == nullptr ||
          board_map.at(pos)->getColor() != king_color) {
        possible_king_moves.push_back(pos);
      }
    }
  }

  for (const std::string &pos : possible_king_moves) {
    int pos_col = PieceUtilities::getColNum(pos[0]);
    int pos_row = std::atoi(&pos[1]);
    bool all_directions_safe = true;
    bool can_king_move = false;

    const bool has_l_pattern_threat =
        checkLPatternThreat(king_color, pos_col, pos_row, board_map);

    if (!has_l_pattern_threat) {
      // Scan all directions to check if King can make a legal move (not into
      // check)
      for (const auto direction : PieceUtilities::DIRECTIONS) {
        int c = pos_col;
        int r = pos_row;
        bool is_one_pace = true;

        while (true) {
          PieceUtilities::moveDirection(direction, c, r);
          std::string moving_pos =
              PieceUtilities::getColLetter(c) + std::to_string(r);
          if (board_map.find(moving_pos) == board_map.end()) {
            break;
          }

          if (board_map.at(moving_pos) != nullptr) {
            if (board_map.at(moving_pos)->getColor() == king_color) {
              if (!is_one_pace) {
                can_king_move = true;
              }
              break;
            } else {
              auto attack_patterns =
                  board_map.at(moving_pos)->getAttackPatterns();
              for (const auto attack : attack_patterns) {
                if (PieceUtilities::canAttackPatternThreaten(
                        PieceUtilities::getOppositeDirection(direction), attack,
                        is_one_pace)) {
                  all_directions_safe = false;
                  break;
                }
              }
            }

            is_one_pace = false;
          }
        }
      }
    }

    if (can_king_move && !has_l_pattern_threat && all_directions_safe) {
      return true;
    }
  }

  return false;
}

bool StalemateTracker::checkLPatternThreat(
    const IPiece::PieceColor king_color, const int king_col, const int king_row,
    const std::map<std::string, std::unique_ptr<IPiece>> &board_map) {
  for (const auto &[pos, piece] : board_map) {
    if (piece != nullptr && piece->getColor() != king_color &&
        piece->getSymbol() == 'N') {
      int knight_col = PieceUtilities::getColNum(pos[0]);
      int knight_row = std::atoi(&pos[1]);
      int col_diff = std::abs(knight_col - king_col);
      int row_diff = std::abs(knight_row - king_row);
      if ((col_diff == 2 && row_diff == 1) ||
          (row_diff == 2 && col_diff == 1)) {
        return true;
      }
    }
  }

  return false;
}

std::vector<std::string> StalemateTracker::getMovablePositionsFromPosition(
    const IPiece::PieceColor piece_color, const std::string &curr_pos,
    const std::map<std::string, std::unique_ptr<IPiece>> &board_map) {
  std::vector<std::string> positions;
  int col = PieceUtilities::getColNum(curr_pos[0]);
  int row = std::atoi(&curr_pos[1]);
  const char symbol = board_map.at(curr_pos)->getSymbol();

  if (symbol == 'N') { // Knight checking
    auto possible_moves = getPossiblePositionsForKnight(col, row);
    for (const auto &move : possible_moves) {
      if (board_map.at(move) == nullptr ||
          board_map.at(move)->getColor() != piece_color) {
        positions.push_back(move);
      }
    }
  } else {
    auto movable_directions = board_map.at(curr_pos)->getMovableDirections();
    for (const auto direction : movable_directions) {
      int c = col;
      int r = row;
      PieceUtilities::moveDirection(direction, c, r);
      std::string target = PieceUtilities::getColLetter(c) + std::to_string(r);
      if (board_map.find(target) == board_map.end()) {
        continue;
      }

      if (board_map.at(target) != nullptr) {
        if (board_map.at(target)->getColor() != piece_color) {
          // Pawn handling
          if (symbol == 'P') {
            int col_diff = c - col;
            int row_diff = r - row;
            if (piece_color == IPiece::PieceColor::WHITE) {
              if ((col_diff == 1 && row_diff == 1) ||
                  (col_diff == -1 && row_diff == 1)) {
                // moving northeast or northwest
                positions.push_back(target);
              }
            } else {
              if ((col_diff == 1 && row_diff == -1) ||
                  (col_diff == -1 && row_diff == -1)) {
                // moving southeast or southwest
                positions.push_back(target);
              }
            }
          } else {
            positions.push_back(target);
          }
        } // Else can't move, blocked by an ally piece
      } else {
        if (symbol == 'P') {
          if ((piece_color == IPiece::PieceColor::WHITE &&
               direction == Direction::NORTH) ||
              (piece_color == IPiece::PieceColor::BLACK &&
               direction == Direction::SOUTH)) {
            positions.push_back(target);
          }
        } else {
          positions.push_back(target);
        }
      }
    }
  }

  return positions;
}

std::vector<std::string>
StalemateTracker::getPossiblePositionsForKnight(const int curr_col,
                                                const int curr_row) {
  std::vector<std::string> ret;
  int target_col;
  int target_row;

  // Start with moving left
  // Going left 2 up 1
  target_col = curr_col - 2;
  target_row = curr_row + 1;
  if (target_col >= 0 && target_row <= 8) {
    ret.push_back(PieceUtilities::getColLetter(target_col) +
                  std::to_string(target_row));
  }

  // Going left 1, up 2
  target_col = curr_col - 1;
  target_row = curr_row + 2;
  if (target_col >= 0 && target_row <= 8) {
    ret.push_back(PieceUtilities::getColLetter(target_col) +
                  std::to_string(target_row));
  }

  // Going left 2, down 1
  target_col = curr_col - 2;
  target_row = curr_row - 1;
  if (target_col >= 0 && target_row > 0) {
    ret.push_back(PieceUtilities::getColLetter(target_col) +
                  std::to_string(target_row));
  }

  // Going left 1, down 2
  target_col = curr_col - 1;
  target_row = curr_row - 2;
  if (target_col >= 0 && target_row > 0) {
    ret.push_back(PieceUtilities::getColLetter(target_col) +
                  std::to_string(target_row));
  }

  // Now try the right side
  // Going right 2 up 1
  target_col = curr_col + 2;
  target_row = curr_row + 1;
  if (target_col < 8 && target_row <= 8) {
    ret.push_back(PieceUtilities::getColLetter(target_col) +
                  std::to_string(target_row));
  }

  // Going right 1, up 2
  target_col = curr_col + 1;
  target_row = curr_row + 2;
  if (target_col < 8 && target_row <= 8) {
    ret.push_back(PieceUtilities::getColLetter(target_col) +
                  std::to_string(target_row));
  }

  // Going right 2, down 1
  target_col = curr_col + 2;
  target_row = curr_row - 1;
  if (target_col < 8 && target_row > 0) {
    ret.push_back(PieceUtilities::getColLetter(target_col) +
                  std::to_string(target_row));
  }

  // Going right 1, down 2
  target_col = curr_col + 1;
  target_row = curr_row - 2;
  if (target_col < 8 && target_row > 0) {
    ret.push_back(PieceUtilities::getColLetter(target_col) +
                  std::to_string(target_row));
  }

  return ret;
}
