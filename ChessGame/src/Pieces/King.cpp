#include "King.h"

King::King(PieceColor color)
    : m_color{color}, m_last_move{std::pair("-", "-")} {}

IPiece::MoveInfo King::getMoveInfo(
    const std::string &from_pos, const std::string &to_pos,
    const std::map<std::string, std::unique_ptr<IPiece>> &board_map) {
  MoveInfo move_info = MoveInfo{};
  const auto &board_positions =
      PieceUtilities::convertBoardPosition(from_pos, to_pos);

  if (board_positions.size() < 2) {
    return move_info;
  }

  if (board_map.at(to_pos) && board_map.at(to_pos)->getColor() == getColor()) {
    return move_info;
  }

  int col = board_positions[0].first;
  int row = board_positions[0].second;

  if (col == board_positions[1].first) {
    // Moving horizontally
    int target_row = row < board_positions[1].second ? ++row : --row;
    std::string pos =
        PieceUtilities::getColLetter(col) + std::to_string(target_row);
    if (row != board_positions[1].second || board_map.at(pos) != nullptr) {
      return move_info;
    }
  } else if (row == board_positions[1].second) {
    // Moving veritcally
    int target_col = col < board_positions[1].first ? ++col : --col;
    std::string pos =
        PieceUtilities::getColLetter(target_col) + std::to_string(row);
    if (col != board_positions[1].first || board_map.at(pos) != nullptr) {
      return move_info;
    }
  } else {
    // Check diagonally
    if (row < board_positions[1].second) {
      ++row;
    } else {
      --row;
    }

    if (col < board_positions[1].first) {
      ++col;
    } else {
      --col;
    }

    if (col != board_positions[1].first || row != board_positions[1].second) {
      return move_info;
    }
  }

  // Make sure move does not put King in check (Illegal move)
  if (doesMovePutKingInCheck(to_pos, board_map)) {
    return move_info;
  }

  m_last_move.first = from_pos;
  m_last_move.second = to_pos;
  move_info.is_valid = true;
  return move_info;
}

void King::setOrigin(const int col, const int row) {
  m_origin.first = col;
  m_origin.second = row;
}

char King::getSymbol() const { return 'K'; }

std::string King::getDisplayPiece() const {
  return m_color == PieceColor::WHITE ? "\u265A" : "\u2654";
}

IPiece::PieceColor King::getColor() const { return m_color; }

std::pair<std::string, std::string> King::getLastMove() const {
  return m_last_move;
}

std::set<IPiece::AttackPattern> King::getAttackPatterns() const {
  std::set<AttackPattern> ret;
  ret.insert(AttackPattern::HORIZONTAL_ONE);
  ret.insert(AttackPattern::VERTICAL_ONE);
  ret.insert(AttackPattern::DIAGONAL_ONE);
  return ret;
}

bool King::doesMovePutKingInCheck(
    const std::string &target_pos,
    const std::map<std::string, std::unique_ptr<IPiece>> &board_map) {
  int col = PieceUtilities::getColNum(target_pos[0]);
  int row = std::atoi(&target_pos[1]);

  // Check L Pattern threat first
  for (const auto &[pos, piece] : board_map) {
    if (piece && piece->getColor() != m_color && piece->getSymbol() == 'N') {
      int knight_col = PieceUtilities::getColNum(pos[0]);
      int knight_row = std::atoi(&pos[1]);
      int col_diff = std::abs(col - knight_col);
      int row_diff = std::abs(row - knight_row);
      if ((col_diff == 2 && row_diff == 1) ||
          (row_diff == 2 && col_diff == 1)) {
        return true; // Knight has king in check
      }
    }
  }

  // Check all other threats
  for (const auto direction : PieceUtilities::DIRECTIONS) {
    int c = col;
    int r = row;
    bool is_one_pace = true;

    while (true) {
      PieceUtilities::moveDirection(direction, c, r);

      std::string new_pos = PieceUtilities::getColLetter(c) + std::to_string(r);
      if (board_map.find(new_pos) == board_map.end()) {
        break;
      }

      if (board_map.at(new_pos) != nullptr) {
        if (board_map.at(new_pos)->getColor() == m_color) {
          break; // Ally piece blocking
        } else {
          auto attack_patterns = board_map.at(new_pos)->getAttackPatterns();
          for (const auto attack : attack_patterns) {
            if (PieceUtilities::canAttackPatternThreaten(direction, attack,
                                                         is_one_pace)) {
              return true; // Will be in check
            }
          }
        }
      }

      is_one_pace = false;
    }
  }

  return false;
}
