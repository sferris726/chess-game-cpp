#include "Knight.h"

Knight::Knight(PieceColor color) : m_color{color} {}

bool Knight::isMoveValid(
    const std::string &from_pos, const std::string &to_pos,
    const std::map<std::string, std::unique_ptr<IPiece>> &board_map) {
  const auto &board_positions =
      PieceUtilities::convertBoardPosition(from_pos, to_pos);

  if (board_positions.size() < 2) {
    return false;
  }

  if (board_map.at(to_pos)->getColor() == getColor()) {
    return false;
  }

  // Start with from position
  int col = board_positions[0].first;
  int row = board_positions[0].second;

  if (std::abs(col - board_positions[1].first == 2)) {
    // Moving vertically
    if (col < board_positions[1].first) {
      col = col + 2;
    } else {
      col = col - 2;
    }

    if (col != board_positions[1].first ||
        (row + 1 != board_positions[1].second &&
         row - 1 != board_positions[1].second)) {
      return false;
    }
  } else if (std::abs(row - board_positions[1].second == 2)) {
    // Moving horizontally
    if (row < board_positions[1].second) {
      row = row + 2;
    } else {
      row = row - 2;
    }

    if (row != board_positions[1].second ||
        (col + 1 != board_positions[1].first &&
         col - 1 != board_positions[1].first)) {
      return false;
    }
  } else {
    return false; // Not in an L shape pattern
  }

  return true;
}

char Knight::getSymbol() const { return 'N'; }

std::string Knight::getColor() const {
  return PieceUtilities::convertPieceColorToStr(m_color);
}
