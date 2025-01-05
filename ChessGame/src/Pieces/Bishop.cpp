#include "Bishop.h"

Bishop::Bishop(PieceColor color)
    : m_color{color}, m_last_move{std::pair("-", "-")} {}

IPiece::MoveInfo Bishop::getMoveInfo(
    const std::string &from_pos, const std::string &to_pos,
    const std::string &king_pos,
    const std::map<std::string, std::unique_ptr<IPiece>> &board_map) {
  MoveInfo move_info = MoveInfo{};
  const auto &board_positions =
      PieceUtilities::convertBoardPosition(from_pos, to_pos);

  // Start at from position
  int col = board_positions[0].first;
  int row = board_positions[0].second;

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
    if (board_map.at(pos)) {
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

  if (PieceUtilities::doesMovePutKingInCheck(m_color, king_pos, from_pos,
                                             to_pos, board_map)) {
    return move_info;
  }

  m_last_move.first = from_pos;
  m_last_move.second = to_pos;
  move_info.is_valid = true;
  return move_info;
}

void Bishop::setOrigin(const int col, const int row) {
  m_origin.first = col;
  m_origin.second = row;
}

char Bishop::getSymbol() const { return 'B'; }

std::string Bishop::getDisplayPiece() const {
  return m_color == PieceColor::WHITE ? "\u265D" : "\u2657";
}

IPiece::PieceColor Bishop::getColor() const { return m_color; }

std::pair<std::string, std::string> Bishop::getLastMove() const {
  return m_last_move;
}

std::set<IPiece::Direction> Bishop::getMovableDirections() const {
  std::set<Direction> directions;
  directions.insert(Direction::NORTH_EAST);
  directions.insert(Direction::SOUTH_EAST);
  directions.insert(Direction::SOUTH_EAST);
  directions.insert(Direction::NORTH_WEST);
  return directions;
}

std::set<IPiece::AttackPattern> Bishop::getAttackPatterns() const {
  std::set<AttackPattern> ret;
  ret.insert(AttackPattern::DIAGONAL_ALL);
  return ret;
}
