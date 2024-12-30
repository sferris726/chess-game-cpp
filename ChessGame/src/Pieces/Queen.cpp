#include "Queen.h"

Queen::Queen(PieceColor color) : m_color{color} {}

bool Queen::isMoveValid(
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
    while (true) {
      if (row < board_positions[1].second) {
        ++row;
      } else {
        --row;
      }

      std::string pos = PieceUtilities::getColLetter(col) + std::to_string(row);
      if (board_map.at(pos) != nullptr) {
        return false;
      }

      if (row == board_positions[1].second) {
        break;
      }
    }
  } else if (row == board_positions[1].second) {
    // Moving vertically
    while (true) {
      if (col < board_positions[1].second) {
        ++col;
      } else {
        --col;
      }

      std::string pos = PieceUtilities::getColLetter(col) + std::to_string(row);
      if (board_map.at(pos) != nullptr) {
        return false;
      }

      if (col == board_positions[1].first) {
        break;
      }
    }
  } else {
    // Check moving diagonally
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
  }

  return true;
}

char Queen::getSymbol() const { return 'Q'; }

IPiece::PieceColor Queen::getColor() const {
  return m_color;
}

std::string Queen::getColorStr() const {
  return PieceUtilities::convertPieceColorToStr(m_color);
}
