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
    if (board_map.at(pos) != nullptr) {
      return move_info;
    }
  } else if (row == board_positions[1].second) {
    // Moving veritcally
    int target_col = col < board_positions[1].first ? ++col : --col;
    std::string pos =
        PieceUtilities::getColLetter(target_col) + std::to_string(row);
    if (board_map.at(pos) != nullptr) {
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

IPiece::PieceColor King::getColor() const { return m_color; }

std::string King::getColorStr() const {
  return PieceUtilities::convertPieceColorToStr(m_color);
}

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
