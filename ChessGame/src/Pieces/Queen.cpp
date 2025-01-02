#include "Queen.h"

Queen::Queen(PieceColor color)
    : m_color{color}, m_last_move{std::pair("-", "-")} {}

IPiece::MoveInfo Queen::getMoveInfo(
    const std::string &from_pos, const std::string &to_pos,
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
      if (board_map.at(pos) != nullptr &&
          board_map.at(to_pos)->getColor() == getColor()) {
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
      if (board_map.at(pos) != nullptr &&
          board_map.at(to_pos)->getColor() == getColor()) {
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
      if (board_map.at(pos) && board_map.at(pos)->getColor() == getColor()) {
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

IPiece::PieceColor Queen::getColor() const { return m_color; }

std::string Queen::getColorStr() const {
  return PieceUtilities::convertPieceColorToStr(m_color);
}

std::pair<std::string, std::string> Queen::getLastMove() const {
  return m_last_move;
}

bool Queen::canMoveInDirection(Direction direction,
                               const std::string &target) const {
  return direction == Direction::EAST || direction == Direction::WEST ||
         direction == Direction::NORTH || direction == Direction::SOUTH ||
         direction == Direction::NORTH_EAST ||
         direction == Direction::NORTH_WEST ||
         direction == Direction::SOUTH_EAST ||
         direction == Direction::SOUTH_WEST;
}

std::set<IPiece::AttackPattern> Queen::getAttackPatterns() const {
  std::set<AttackPattern> ret;
  ret.insert(AttackPattern::HORIZONTAL_ALL);
  ret.insert(AttackPattern::VERTICAL_ALL);
  ret.insert(AttackPattern::DIAGONAL_ALL);
  return ret;
}
