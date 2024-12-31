#include "CheckMateTracker.h"

CheckMateTracker::CheckMateTracker() {
  m_directions = {Direction::NORTH, Direction::NORTH_EAST,
                  Direction::EAST,  Direction::SOUTH_EAST,
                  Direction::SOUTH, Direction::SOUTH_WEST,
                  Direction::WEST,  Direction::NORTH_WEST};
}

void CheckMateTracker::scanBoard(
    const IPiece::PieceColor king_color,
    const std::map<std::string, std::unique_ptr<IPiece>> &board_map) {
  // Locate the King
  std::string king_pos;
  for (const auto &[pos, piece] : board_map) {
    if (piece->getSymbol() == 'K' && piece->getColor() == king_color) {
      king_pos = pos;
      break;
    }
  }

  // Scan the board from the King position
  std::set<Direction> movable_directions;
  for (int i = 0; i < 8; ++i) {
    for (Direction direction : m_directions) {
      if (isDirectionSafe(direction, king_pos, king_color, board_map)) {
        movable_directions.insert(direction);
      }
    }
  }

  if (movable_directions.empty()) {
    // Checkmate
  }
}

bool CheckMateTracker::isDirectionSafe(
    Direction direction, const std::string &king_pos,
    const IPiece::PieceColor king_color,
    const std::map<std::string, std::unique_ptr<IPiece>> &board_map) {
  int king_col = PieceUtilities::getColNum(king_pos[0]);
  int king_row = std::atoi(&king_pos[1]);

  while (true) {
    switch (direction) {
    case Direction::NORTH:
      ++king_row;
      break;
    case Direction::NORTH_EAST:
      ++king_col;
      ++king_row;
      break;
    case Direction::EAST:
      ++king_col;
      break;
    case Direction::SOUTH_EAST:
      ++king_col;
      --king_row;
      break;
    case Direction::SOUTH:
      --king_row;
      break;
    case Direction::SOUTH_WEST:
      --king_col;
      --king_row;
      break;
    case Direction::WEST:
      --king_col;
      break;
    case Direction::NORTH_WEST:
      --king_col;
      ++king_row;
      break;
    }

    // Bounds check
    if ((king_col > 8 || king_col < 0) || (king_row > 8 || king_row < 0)) {
      return false;
    }

    std::string curr_pos = PieceUtilities::getColLetter(king_col) + std::to_string(king_row);
    if (board_map.at(curr_pos) != nullptr && board_map.at(curr_pos)->getColor() != king_color) {
        // Found an opponent piece, check if the attack pattern can threaten the King
    }
  }
}
