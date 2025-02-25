#include "Knight.h"

Knight::Knight(PieceColor color)
    : m_color{color}, m_last_move{std::pair("-", "-")} {}

IPiece::MoveInfo Knight::getMoveInfo(
    const std::string &from_pos, const std::string &to_pos,
    const std::string &king_pos, const bool king_in_check,
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

  // Start with from position
  int col = board_positions[0].first;
  int row = board_positions[0].second;
  int col_diff = std::abs(col - board_positions[1].first);
  int row_diff = std::abs(row - board_positions[1].second);

  if (col_diff == 2) {
    // Moving vertically
    if (col < board_positions[1].first) {
      col = col + 2;
    } else {
      col = col - 2;
    }

    if (col != board_positions[1].first ||
        (row + 1 != board_positions[1].second &&
         row - 1 != board_positions[1].second)) {
      return move_info;
    }
  } else if (row_diff == 2) {
    // Moving horizontally
    if (row < board_positions[1].second) {
      row = row + 2;
    } else {
      row = row - 2;
    }

    if (row != board_positions[1].second ||
        (col + 1 != board_positions[1].first &&
         col - 1 != board_positions[1].first)) {
      return move_info;
    }
  } else {
    return move_info; // Not in an L shape pattern
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

void Knight::setOrigin(const int col, const int row) {
  m_origin.first = col;
  m_origin.second = row;
}

char Knight::getSymbol() const { return 'N'; }

std::string Knight::getDisplayPiece() const {
  return m_color == PieceColor::WHITE ? "\u265E" : "\u2658";
}

IPiece::PieceColor Knight::getColor() const { return m_color; }

std::pair<std::string, std::string> Knight::getLastMove() const {
  return m_last_move;
}

std::set<IPiece::Direction> Knight::getMovableDirections() const {
  // Not implemented for Knight
  return std::set<Direction>{};
}

std::set<IPiece::AttackPattern> Knight::getAttackPatterns() const {
  std::set<AttackPattern> ret;
  ret.insert(AttackPattern::L_SHAPE);
  return ret;
}
