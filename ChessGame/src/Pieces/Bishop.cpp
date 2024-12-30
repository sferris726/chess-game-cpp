#include "Bishop.h"

Bishop::Bishop(PieceColor color) : m_color{color} {}

bool Bishop::isMoveValid(
    const std::string &from_pos, const std::string &to_pos,
    const std::map<std::string, std::unique_ptr<IPiece>> &board_map) {
  const auto &board_positions =
      PieceUtilities::convertBoardPosition(from_pos, to_pos);

  if (board_positions[0].second == board_positions[1].second) {
    return false; // Not moving diagonally
  }

  // Start at from position
  int col = board_positions[0].first;
  int row = board_positions[0].second;

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

    std::string pos = std::to_string(col) + std::to_string(row);
    if (board_map.at(pos) && board_map.at(pos)->getColor() == getColor()) {
      return false;
    }

    if (col == board_positions[1].first || row == board_positions[1].second) {
      break;
    }
  }

  // Make sure we are at the target position
  if (col != board_positions[1].first && row != board_positions[1].second) {
    return false;
  }

  return true;
}

char Bishop::getSymbol() const { return 'B'; }

std::string Bishop::getColor() const {
  return PieceUtilities::convertPieceColorToStr(m_color);
}
