#include "King.h"

King::King(PieceColor color) : m_color{color} {}

bool King::isMoveValid(
    const std::string &from_pos, const std::string &to_pos,
    const std::map<std::string, std::unique_ptr<IPiece>> &board_map) {
  const auto &board_positions =
      PieceUtilities::convertBoardPosition(from_pos, to_pos);

  if (board_positions.size() < 2) {
    return false;
  }

  if (board_map.at(to_pos) && board_map.at(to_pos)->getColor() == getColor()) {
    return false;
  }

  int col = board_positions[0].first;
  int row = board_positions[0].second;

  if (col == board_positions[1].first) {
    // Moving horizontally
    int target_row = row < board_positions[1].second ? ++row : --row;
    std::string pos = std::to_string(col) + std::to_string(target_row);
    if (board_map.at(pos) != nullptr) {
      return false;
    }
  } else if (row == board_positions[1].second) {
    // Moving veritcally
    int target_col = col < board_positions[1].first ? ++col : --col;
    std::string pos = std::to_string(target_col) + std::to_string(row);
    if (board_map.at(pos) != nullptr) {
      return false;
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
      return false;
    }
  }

  return true;
}

char King::getSymbol() const { return 'K'; }

std::string King::getColor() const {
  return PieceUtilities::convertPieceColorToStr(m_color);
}
