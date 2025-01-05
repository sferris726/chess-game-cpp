#include "Queen.h"

Queen::Queen(PieceColor color)
    : m_color{color}, m_last_move{std::pair("-", "-")} {}

IPiece::MoveInfo Queen::getMoveInfo(
    const std::string &from_pos, const std::string &to_pos,
    const std::string &king_pos, const bool king_in_check,
    const std::map<std::string, std::unique_ptr<IPiece>> &board_map) {
  MoveInfo move_info = MoveInfo{};
  const auto &board_positions =
      PieceUtilities::convertBoardPosition(from_pos, to_pos);

  if (board_positions.size() < 2) {
    return move_info;
  }

  int col = board_positions[0].first;
  int row = board_positions[0].second;

  if (col == board_positions[1].first) {
    // Moving vertically
    while (true) {
      if (row < board_positions[1].second) {
        ++row;
      } else {
        --row;
      }

      std::string pos = PieceUtilities::getColLetter(col) + std::to_string(row);
      if (board_map.at(pos) != nullptr) {
        if (board_map.at(pos)->getColor() != m_color && pos == to_pos) {
          break;
        }
        return move_info;
      }

      if (row == board_positions[1].second) {
        break;
      }
    }
  } else if (row == board_positions[1].second) {
    // Moving horizontally
    while (true) {
      if (col < board_positions[1].first) {
        ++col;
      } else {
        --col;
      }

      std::string pos = PieceUtilities::getColLetter(col) + std::to_string(row);
      if (board_map.at(pos) != nullptr) {
        if (board_map.at(pos)->getColor() != m_color && pos == to_pos) {
          break;
        }
        return move_info;
      }

      if (col == board_positions[1].first) {
        break;
      }
    }
  } else {
    // Check moving diagonally
    if (std::abs(col - board_positions[1].first) !=
        std::abs(row - board_positions[1].second)) {
      return move_info; // Not moving diagonally
    }

    while (true) {
      if (board_positions[0].second < board_positions[1].second) {
        ++row;
      } else {
        --row;
      }

      if (board_positions[0].first < board_positions[1].first) {
        ++col;
      } else {
        --col;
      }

      std::string pos = PieceUtilities::getColLetter(col) + std::to_string(row);
      if (board_map.at(pos) != nullptr) {
        if (board_map.at(pos)->getColor() != m_color && pos == to_pos) {
          break;
        }
        return move_info;
      }

      if (col == board_positions[1].first || row == board_positions[1].second) {
        break;
      }
    }

    // Make sure we are at the target position
    if (col != board_positions[1].first && row != board_positions[1].second) {
      return move_info;
    }
  }

  if (!king_in_check) {
    if (PieceUtilities::doesMovePutKingInCheck(m_color, king_pos, from_pos,
                                               to_pos, board_map)) {
      return move_info;
    }
  }

  m_last_move.first = from_pos;
  m_last_move.second = to_pos;
  move_info.is_valid = true;
  return move_info;
}

void Queen::setOrigin(const int col, const int row) {
  m_origin.first = col;
  m_origin.second = row;
}

char Queen::getSymbol() const { return 'Q'; }

std::string Queen::getDisplayPiece() const {
  return m_color == PieceColor::WHITE ? "\u265B" : "\u2655";
}

IPiece::PieceColor Queen::getColor() const { return m_color; }

std::pair<std::string, std::string> Queen::getLastMove() const {
  return m_last_move;
}

std::set<IPiece::Direction> Queen::getMovableDirections() const {
  return PieceUtilities::DIRECTIONS;
}

std::set<IPiece::AttackPattern> Queen::getAttackPatterns() const {
  std::set<AttackPattern> ret;
  ret.insert(AttackPattern::HORIZONTAL_ALL);
  ret.insert(AttackPattern::VERTICAL_ALL);
  ret.insert(AttackPattern::DIAGONAL_ALL);
  return ret;
}
