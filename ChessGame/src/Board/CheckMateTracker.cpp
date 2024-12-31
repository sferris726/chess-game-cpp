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
  bool king_in_check = false;

  // Locate the King
  std::string king_pos;
  for (const auto &[pos, piece] : board_map) {
    if (piece && piece->getSymbol() == 'K' && piece->getColor() == king_color) {
      king_pos = pos;
      break;
    }
  }

  // Check if the Knights can threaten, need special handling for L-Shape
  // pattern attack
  std::set<Direction> excluded_directions;
  for (const auto &[pos, piece] : board_map) {
    if (piece && piece->getSymbol() == 'N' && piece->getColor() != king_color) {
      auto l_threat = checkLPatternThreat(king_pos, pos);
      king_in_check = l_threat.first;
      excluded_directions.insert(l_threat.second.begin(),
                                 l_threat.second.end());
    }
  }

  // Scan the board from the King position
  std::set<Direction> movable_directions;
  for (int i = 0; i < 8; ++i) {
    for (Direction direction : m_directions) {
      auto is_check_and_movable = isCheckAndDirectionMovable(
          direction, king_pos, king_color, board_map);

      // Make sure King isn't already deemed to be in check
      if (!king_in_check) {
        king_in_check = is_check_and_movable.first;
      }

      if (is_check_and_movable.second) {
        // Direction can't be moved to
        movable_directions.insert(direction);
      }
    }
  }

  if (movable_directions.empty()) {
    // Checkmate
  }
}

std::pair<bool, std::set<ICheckMateTracker::Direction>>
CheckMateTracker::checkLPatternThreat(const std::string &king_pos,
                                      const std::string &knight_pos) {
  bool in_check = false;
  std::set<Direction> directions_to_exclude;

  int king_col = PieceUtilities::getColNum(king_pos[0]);
  int king_row = std::atoi(&king_pos[1]);
  int knight_col = PieceUtilities::getColNum(knight_pos[0]);
  int knight_row = std::atoi(&knight_pos[1]);

  int col_diff = std::abs(king_col - knight_col);
  int row_diff = std::abs(king_row - knight_row);

  if ((col_diff == 2 && row_diff == 1) || (row_diff == 2 && col_diff == 1)) {
    // Attack can be made
    in_check = true;
    if (king_col - knight_col < 0) {
      // King is to the left of the knight
      if (col_diff == 2) {
        directions_to_exclude.insert(Direction::NORTH_EAST);
      } else {
        directions_to_exclude.insert(Direction::SOUTH_WEST);
      }
    } else {
      // King is to the right of the knight
      if (col_diff == 2) {
        directions_to_exclude.insert(Direction::NORTH_WEST);
      } else {
        directions_to_exclude.insert(Direction::SOUTH_EAST);
      }
    }
  } else if ((col_diff == 3 && row_diff == 1) ||
             (row_diff == 3 && col_diff == 1)) {
    // Knight can have King in check if King moves in a certain direction
    if (col_diff == 3) {
      if (king_col - knight_col < 0) { // King is to left of knight
        directions_to_exclude.insert(Direction::EAST);
      } else { // King is to right of knight
        directions_to_exclude.insert(Direction::WEST);
      }
    } else {
      if (king_row - knight_row < 0) { // King is to left of knight
        directions_to_exclude.insert(Direction::EAST);
      } else { // King is to right of knight
        directions_to_exclude.insert(Direction::WEST);
      }
    }
  }

  return std::pair(in_check, directions_to_exclude);
}

std::pair<bool, bool> CheckMateTracker::isCheckAndDirectionMovable(
    Direction direction, const std::string &king_pos,
    const IPiece::PieceColor king_color,
    const std::map<std::string, std::unique_ptr<IPiece>> &board_map) {
  // Bounds check
  if (!inBoundsCheck(direction, king_pos)) {
    return std::pair(false, false);
  }

  bool in_check = false;
  bool direction_movable = true;
  int king_col = PieceUtilities::getColNum(king_pos[0]);
  int king_row = std::atoi(&king_pos[1]);

  while ((king_col >=0 && king_col < 8) && (king_row > 0 && king_row <= 8)) {
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

    std::string curr_pos =
        PieceUtilities::getColLetter(king_col) + std::to_string(king_row);
    if (board_map.at(curr_pos) != nullptr) {
      if (board_map.at(curr_pos)->getColor() != king_color) {
        // Found an opponent piece, check if the attack pattern can threaten the
        // King
        bool attack_valid = false;
        auto attacks = board_map.at(curr_pos)->getAttackPatterns();
        for (const auto attack : attacks) {
            const bool is_one_rank = isOneRankFromKing(direction, king_col, king_row, curr_pos);
          if (PieceUtilities::canAttackPatternThreaten(direction, attack, is_one_rank)) {
            attack_valid = true;
            break;
          }
        }

        if (attack_valid) {
          in_check = true;
          direction_movable = false;
          break;
        }
      } else {
        // Found same color piece blocking direction from attack
        in_check = false;
        direction_movable = false;
        break;
      }
    }
  }

  return std::pair(in_check, direction_movable);
}

bool CheckMateTracker::inBoundsCheck(Direction direction,
                                     const std::string &pos) {
  int col = PieceUtilities::getColNum(pos[0]);
  int row = std::atoi(&pos[1]);

  switch (direction) {
  case Direction::NORTH:
    if (row == 8) {
      return false;
    }
    break;
  case Direction::NORTH_EAST:
    if (row == 8 || col == 7) {
      return false;
    }
    break;
  case Direction::EAST:
    if (col == 7) {
      return false;
    }
  case Direction::SOUTH_EAST:
    if (row == 1 || col == 7) {
      return false;
    }
    break;
  case Direction::SOUTH:
    if (row == 1) {
      return false;
    }
    break;
  case Direction::SOUTH_WEST:
    if (row == 1 || col == 0) {
      return false;
    }
    break;
  case Direction::WEST:
    if (row == 1) {
      return false;
    }
    break;
  case Direction::NORTH_WEST:
    if (row == 8 || col == 0) {
      return false;
    }
    break;
  default:
    return false;
  }

  return true;
}

bool CheckMateTracker::isOneRankFromKing(Direction king_direction, int king_col, int king_row, const std::string& piece_pos) {
    int piece_col = PieceUtilities::getColNum(piece_pos[0]);
    int piece_row = std::atoi(&piece_pos[1]);

    switch (king_direction) {
        case Direction::NORTH:
            return (piece_row - king_row == 1 && king_col == piece_col);
        case Direction::NORTH_EAST:
            return (piece_row - king_row == 1 && piece_col - king_col == 1);
        case Direction::EAST:
            return (piece_col - king_col == 1 && king_row == piece_row);
        case Direction::SOUTH_EAST:
            return (king_col - piece_col == 1 && king_row - piece_row == 1);
        case Direction::SOUTH:
            return (king_row - piece_row == 1 && king_col == piece_col);
        case Direction::SOUTH_WEST:
            return (king_row - piece_row == 1 && king_col - piece_col == 1);
        case Direction::WEST:
            return (king_col - piece_col == 1 && king_row == piece_row);
        case Direction::NORTH_WEST:
            return (piece_row - king_row == 1 && king_col - piece_col == 1);
        default:
            return false;
    }
}
