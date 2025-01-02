#include "CheckMateTracker.h"

static const std::set<IPiece::Direction> DIRECTIONS = {
    IPiece::Direction::NORTH, IPiece::Direction::NORTH_EAST,
    IPiece::Direction::EAST,  IPiece::Direction::SOUTH_EAST,
    IPiece::Direction::SOUTH, IPiece::Direction::SOUTH_WEST,
    IPiece::Direction::WEST,  IPiece::Direction::NORTH_WEST};

CheckMateTracker::CheckMateTracker()
    : m_white_king_in_check{false}, m_black_king_in_check{false} {}

void CheckMateTracker::onKingInCheckChange(
    std::function<void(const IPiece::PieceColor color, const bool in_check)>
        callback) {
  m_king_in_check_callback = callback;
}

void CheckMateTracker::onCheckMate(std::function<void()> callback) {
  m_checkmate_callback = callback;
}

void CheckMateTracker::scanBoard(
    const IPiece::PieceColor king_color, const bool has_next_move,
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
  std::vector<std::pair<std::string, std::set<Direction>>> threats;
  for (const auto &[pos, piece] : board_map) {
    if (piece && piece->getSymbol() == 'N' && piece->getColor() != king_color) {
      auto l_threat = checkLPatternThreat(king_pos, pos);
      king_in_check = l_threat.first;
      threats.push_back(std::pair(pos, l_threat.second));
    }
  }

  // Directions to check against
  // std::vector<Direction> directions_to_check;
  // for (const auto direction : DIRECTIONS) {
  //   if (threatened_directions.count(direction) == 0) {
  //     directions_to_check.push_back(direction);
  //   }
  // }

  // Scan the board for threats to the King position
  std::set<Direction> movable_directions;
  for (Direction direction : DIRECTIONS) {
    auto threat_info =
        getThreatInfo(direction, king_pos, king_color, board_map);

    if (threat_info.has_check) {
      threats.push_back(
          std::pair(threat_info.threat_pos, std::set<Direction>{direction}));
      // Make sure King isn't already deemed to be in check
      king_in_check = !king_in_check ? threat_info.has_check : king_in_check;
    }

    if (threat_info.direction_movable_for_king) {
      // Direction can be moved to
      movable_directions.insert(direction);
    }
  }

  if (king_in_check && movable_directions.empty()) {
    bool is_checkmate = true;

    if (has_next_move) {
      // No movable directions, scan board for pieces that can protect the king
      for (const auto &threat : threats) {
        bool can_protect = false;
        for (const auto direction : threat.second) {
          if (scanForProtections(king_color, king_pos, threat.first, direction,
                                 board_map)) {
            can_protect = true;
            break;
          }
        }

        if (can_protect) {
          is_checkmate = false;
          break;
        }
      }
    }

    if (is_checkmate) {
      m_checkmate_callback();
    } else {
      checkUpdate(king_in_check, king_color);
    }
  } else {
    checkUpdate(king_in_check, king_color);
  }
}

bool CheckMateTracker::castlingScan(
    const IPiece::PieceColor color, const std::string &pos1,
    const std::string &pos2,
    const std::map<std::string, std::unique_ptr<IPiece>> &board_map) {
  // Check if the Knights can attack from the L pattern
  for (const auto &[pos, piece] : board_map) {
    if (piece && piece->getSymbol() == 'N' && piece->getColor() != color) {
      if (checkLPatternThreat(pos1, pos).first ||
          checkLPatternThreat(pos2, pos).first) {
        // King is in check
        return false;
      }
    }
  }

  // Check if King is threatened by other pieces
  for (const Direction direction : DIRECTIONS) {
    const bool pos1_in_check =
        getThreatInfo(direction, pos1, color, board_map).has_check;
    const bool pos2_in_check =
        getThreatInfo(direction, pos2, color, board_map).has_check;

    if (pos1_in_check || pos2_in_check) {
      return false;
    }
  }

  return true;
}

std::pair<bool, std::set<IPiece::Direction>>
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

CheckMateTracker::ThreatInfo CheckMateTracker::getThreatInfo(
    Direction direction, const std::string &king_pos,
    const IPiece::PieceColor king_color,
    const std::map<std::string, std::unique_ptr<IPiece>> &board_map) {
  // Bounds check
  if (!inBoundsCheck(direction, king_pos)) {
    return ThreatInfo{};
  }

  ThreatInfo threat_info;
  threat_info.direction_movable_for_king = true;
  int king_col = PieceUtilities::getColNum(king_pos[0]);
  int king_row = std::atoi(&king_pos[1]);

  while (true) {
    moveDirection(direction, king_col, king_row);

    if ((king_col <= 0 || king_col > 7) || (king_row < 1 || king_row > 8)) {
      break; // Out of bounds
    }

    std::string curr_pos =
        PieceUtilities::getColLetter(king_col) + std::to_string(king_row);
    if (board_map.at(curr_pos) != nullptr) {
      if (board_map.at(curr_pos)->getSymbol() == 'N') {
        continue; // Already checked for Knight attacks
      }

      if (board_map.at(curr_pos)->getColor() != king_color) {
        // Found an opponent piece, check if the attack pattern can threaten the
        // King
        bool attack_valid = false;
        auto attacks = board_map.at(curr_pos)->getAttackPatterns();
        for (const auto attack : attacks) {
          const bool is_one_rank =
              isOneRankFromKing(direction, king_col, king_row, curr_pos);
          if (PieceUtilities::canAttackPatternThreaten(direction, attack,
                                                       is_one_rank)) {
            threat_info.threat_pos = curr_pos;
            attack_valid = true;
            break;
          }
        }

        if (attack_valid) {
          threat_info.has_check = true;
          threat_info.direction_movable_for_king = false;
          break;
        }
      } else {
        // Found same color piece blocking direction from attack
        threat_info.has_check = false;
        threat_info.direction_movable_for_king = false;
        break;
      }
    }
  }

  return threat_info;
}

bool CheckMateTracker::scanForProtections(
    const IPiece::PieceColor color, const std::string &king_pos,
    const std::string &threat_pos, const Direction direction,
    const std::map<std::string, std::unique_ptr<IPiece>> &board_map) {
  int threat_col = PieceUtilities::getColNum(threat_pos[0]);
  int threat_row = std::atoi(&threat_pos[1]);
  std::vector<std::string> potential_protection_pieces;

  // Scan for ally pieces that can potentially attack the attacking piece
  for (const auto direction : DIRECTIONS) {
    auto pieces = getOpposingPiecesInDirection(
        direction,
        color == IPiece::PieceColor::WHITE
            ? IPiece::PieceColor::BLACK // Need the threat piece color
            : IPiece::PieceColor::WHITE,
        threat_col, threat_row, board_map);
    potential_protection_pieces.insert(potential_protection_pieces.end(),
                                       pieces.begin(), pieces.end());
  }

  // Can threat piece be attacked
  for (const std::string &pos : potential_protection_pieces) {
    if (PieceUtilities::canPieceBeAttacked(threat_pos, pos,
                                           *board_map.at(pos))) {
      return true; // Threat piece can be attacked
    }
  }

  // Can a piece move to protect the King
  if (canKingBeProtected(color, king_pos, threat_pos, direction, board_map)) {
    return true;
  }

  return false;
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

bool CheckMateTracker::isOneRankFromKing(Direction king_direction, int king_col,
                                         int king_row,
                                         const std::string &piece_pos) {
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

std::vector<std::string> CheckMateTracker::getOpposingPiecesInDirection(
    const Direction direction, const IPiece::PieceColor color, int col, int row,
    const std::map<std::string, std::unique_ptr<IPiece>> &board_map) {
  std::vector<std::string> ret;
  bool found_same_color = false;

  while (true) {
    moveDirection(direction, col, row);

    if ((col <= 0 || col > 7) || (row < 1 || row > 8)) {
      break; // Out of bounds
    }

    std::string curr_pos =
        PieceUtilities::getColLetter(col) + std::to_string(row);
    if (board_map.at(curr_pos)) {
      if (board_map.at(curr_pos)->getColor() == color) {
        found_same_color = true;
      } else {
        if (found_same_color) {
          if (board_map.at(curr_pos)->getSymbol() == 'N') {
            // Knights can jump pieces
            ret.push_back(curr_pos);
          }
        } else {
          ret.push_back(curr_pos);
        }
      }
    }
  }

  return ret;
}

bool CheckMateTracker::canKingBeProtected(
    const IPiece::PieceColor ally_color, const std::string &king_pos,
    const std::string &threat_pos, const Direction threat_direction,
    const std::map<std::string, std::unique_ptr<IPiece>> &board_map) {
  int king_col = PieceUtilities::getColNum(king_pos[0]);
  int king_row = std::atoi(&king_pos[1]);
  int threat_col = PieceUtilities::getColNum(threat_pos[0]);
  int threat_row = std::atoi(&threat_pos[1]);
  std::vector<std::pair<int, int>> bounds;

  while (true) {
    moveDirection(threat_direction, king_col, king_row);

    if ((king_col == threat_col && king_row == threat_row) ||
        ((king_col <= 0 || king_col > 7) || (king_row < 1 || king_row > 8))) {
      break; // Out of bounds
    } else {
      bounds.push_back(std::pair(king_col, king_row));
    }
  }

  std::vector<std::string> ally_pieces;
  for (const auto &[pos, piece] : board_map) {
    if (piece && piece->getColor() == ally_color) {
      const char symbol = piece->getSymbol();
      if (symbol == 'K') {
        continue;
      } else if (symbol == 'N') { // Have special handling for L pattern to
                                  // account edge case for jumping
        int knight_col = PieceUtilities::getColNum(pos[0]);
        int knight_row = std::atoi(&pos[1]);
        for (const auto &[col, row] : bounds) {
          int col_diff = std::abs(knight_col - col);
          int row_diff = std::abs(knight_row - row);
          if ((col_diff == 2 && row_diff == 1) ||
              (row_diff == 2 && col_diff == 1)) {
            std::string target =
                PieceUtilities::getColLetter(col) + std::to_string(row);
            if (board_map.at(target) == nullptr) {
              return true; // Knight can move to protect
            }
          }
        }
      } else {
        ally_pieces.push_back(pos);
      }
    }
  }

  // Check if ally pieces can move into the bounds between the King and the
  // threat Need to ensure there is nothing blocking from getting to the bounds
  for (const auto &pos : ally_pieces) {
    if (canPieceMoveIntoBounds(*board_map.at(pos), pos, bounds, board_map)) {
      return true;
    }
  }

  return false;
}

bool CheckMateTracker::canPieceMoveIntoBounds(
    const IPiece &piece, const std::string &piece_pos,
    const std::vector<std::pair<int, int>> bounds,
    const std::map<std::string, std::unique_ptr<IPiece>> &board_map) {
  int piece_col = PieceUtilities::getColNum(piece_pos[0]);
  int piece_row = std::atoi(&piece_pos[1]);
  const char symbol = piece.getSymbol();

  for (const auto &[col, row] : bounds) {
    bool can_move_once = symbol == 'P';
    int tmp_col = piece_col;
    int tmp_row = piece_row;

    while (true) {
      int col_diff = col - tmp_col;
      int row_diff = row - tmp_row;

      if (col_diff == 0 && row_diff > 0) {
        // moving north
        if (symbol == 'Q' || symbol == 'R' ||
            (symbol == 'P' && piece.getColor() == IPiece::PieceColor::WHITE)) {
          moveDirection(Direction::NORTH, tmp_col, tmp_row);
        }
      } else if (col_diff == 0 && row_diff < 0) {
        // moving south
        if (symbol == 'Q' || symbol == 'R' ||
            (symbol == 'P' && piece.getColor() == IPiece::PieceColor::BLACK)) {
          moveDirection(Direction::SOUTH, tmp_col, tmp_row);
        }
      } else if (col_diff > 0 && row_diff == 0) {
        // moving east
        if (symbol == 'Q' || symbol == 'R') {
          moveDirection(Direction::EAST, tmp_col, tmp_row);
        }
      } else if (col_diff < 0 && row_diff == 0) {
        // moving west
        if (symbol == 'Q' || symbol == 'R') {
          moveDirection(Direction::WEST, tmp_col, tmp_row);
        }
      } else if (col_diff > 0 && row_diff > 0) {
        // moving northeast
        if (symbol == 'Q' || symbol == 'B' ||
            (symbol == 'P' && piece.getColor() == IPiece::PieceColor::WHITE &&
             col_diff == 1 && row_diff == 1)) {
          moveDirection(Direction::NORTH_EAST, tmp_col, tmp_row);
        }
      } else if (col_diff > 0 && row_diff < 0) {
        // moving southeast
        if (symbol == 'Q' || symbol == 'B' ||
            (symbol == 'P' && piece.getColor() == IPiece::PieceColor::BLACK &&
             col_diff == 1 && row_diff == -1)) {
          moveDirection(Direction::NORTH_EAST, tmp_col, tmp_row);
        }
      } else if (col_diff < 0 && row_diff < 0) {
        // moving southwest
        if (symbol == 'Q' || symbol == 'B' ||
            (symbol == 'P' && piece.getColor() == IPiece::PieceColor::BLACK &&
             col_diff == -1 && row_diff == -1)) {
          moveDirection(Direction::NORTH_EAST, tmp_col, tmp_row);
        }
      } else if (col_diff < 0 && row_diff > 0) {
        // moving northwest
        if (symbol == 'Q' || symbol == 'B' ||
            (symbol == 'P' && piece.getColor() == IPiece::PieceColor::WHITE &&
             col_diff == -1 && row_diff == 1)) {
          moveDirection(Direction::NORTH_EAST, tmp_col, tmp_row);
        }
      }

      std::string new_target =
          PieceUtilities::getColLetter(tmp_col) + std::to_string(tmp_row);
      if (board_map.find(new_target) == board_map.end() ||
          board_map.at(new_target) != nullptr) {
        return false;
      }

      if (tmp_col == col && tmp_row == row) {
        return true;
      }

      if (can_move_once) {
        break;
      }
    }
  }
  return false;
}

void CheckMateTracker::moveDirection(Direction direction, int &col, int &row) {
  switch (direction) {
  case Direction::NORTH:
    ++row;
    break;
  case Direction::NORTH_EAST:
    ++col;
    ++row;
    break;
  case Direction::EAST:
    ++col;
    break;
  case Direction::SOUTH_EAST:
    ++col;
    --row;
    break;
  case Direction::SOUTH:
    --row;
    break;
  case Direction::SOUTH_WEST:
    --col;
    --row;
    break;
  case Direction::WEST:
    --col;
    break;
  case Direction::NORTH_WEST:
    --col;
    ++row;
    break;
  }
}

void CheckMateTracker::checkUpdate(const bool king_in_check,
                                   const IPiece::PieceColor color) {
  if (king_in_check) {
    std::cout << "King is in Check!\n";
  }

  // Send out updates if check status has changed
  if (color == IPiece::PieceColor::WHITE) {
    if (m_white_king_in_check != king_in_check) {
      m_white_king_in_check = king_in_check;
      m_king_in_check_callback(color, king_in_check);
    }
  } else {
    if (m_black_king_in_check != king_in_check) {
      m_black_king_in_check = king_in_check;
      m_king_in_check_callback(color, king_in_check);
    }
  }
}
