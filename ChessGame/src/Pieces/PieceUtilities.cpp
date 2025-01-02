#include "PieceUtilities.h"

namespace PieceUtilities {
std::string convertPieceColorToStr(IPiece::PieceColor color_type) {
  if (color_type == IPiece::PieceColor::WHITE) {
    return "w";
  } else {
    return "b";
  }
}

std::vector<std::pair<int, int>>
convertBoardPosition(const std::string &from_pos, const std::string &to_pos) {
  std::vector<std::pair<int, int>> ret;
  if (from_pos.size() == 2 && to_pos.size() == 2) {
    ret.push_back(std::pair(getColNum(from_pos[0]), std::atoi(&from_pos[1])));
    ret.push_back(std::pair(getColNum(to_pos[0]), std::atoi(&to_pos[1])));
  } else {
    ret.push_back(std::pair(-1, -1));
  }

  return ret;
}

std::string getColLetter(int col) {
  switch (col) {
  case 0:
    return "a";
  case 1:
    return "b";
  case 2:
    return "c";
  case 3:
    return "d";
  case 4:
    return "e";
  case 5:
    return "f";
  case 6:
    return "g";
  case 7:
    return "h";
  default:
    throw std::runtime_error("Invalid Col passed");
  }
}

int getColNum(const char col) {
  if (col == 'a') {
    return 0;
  }

  if (col == 'b') {
    return 1;
  }

  if (col == 'c') {
    return 2;
  }

  if (col == 'd') {
    return 3;
  }

  if (col == 'e') {
    return 4;
  }

  if (col == 'f') {
    return 5;
  }

  if (col == 'g') {
    return 6;
  }

  if (col == 'h') {
    return 7;
  }

  throw std::runtime_error("Invalid Col letter passed");
}

IPiece::PieceType convertStrToPieceType(const std::string &str) {
  if (str == "N") {
    return IPiece::PieceType::KNIGHT;
  }

  if (str == "B") {
    return IPiece::PieceType::BISHOP;
  }

  if (str == "R") {
    return IPiece::PieceType::ROOK;
  }

  if (str == "Q") {
    return IPiece::PieceType::QUEEN;
  }

  return IPiece::PieceType::NONE;
}

bool canAttackPatternThreaten(IPiece::Direction direction,
                              IPiece::AttackPattern attack,
                              bool is_one_rank_from) {
  switch (direction) {
  case IPiece::Direction::NORTH:
  case IPiece::Direction::SOUTH:
    return attack == IPiece::AttackPattern::VERTICAL_ALL ||
           (is_one_rank_from && attack == IPiece::AttackPattern::VERTICAL_ONE);

  case IPiece::Direction::NORTH_EAST:
  case IPiece::Direction::SOUTH_EAST:
  case IPiece::Direction::NORTH_WEST:
  case IPiece::Direction::SOUTH_WEST:
    return attack == IPiece::AttackPattern::DIAGONAL_ALL ||
           (is_one_rank_from && attack == IPiece::AttackPattern::DIAGONAL_ONE);

  case IPiece::Direction::EAST:
  case IPiece::Direction::WEST:
    return attack == IPiece::AttackPattern::HORIZONTAL_ALL ||
           (is_one_rank_from &&
            attack == IPiece::AttackPattern::HORIZONTAL_ONE);

  default:
    return false;
  }
}

bool canPieceBeAttacked(const std::string &piece_pos,
                        const std::string &attack_pos,
                        const IPiece &attack_piece) {
  int piece_col = getColNum(piece_pos[0]);
  int piece_row = std::atoi(&piece_pos[1]);
  int attack_col = getColNum(attack_pos[0]);
  int attack_row = std::atoi(&attack_pos[1]);
  auto attack_patterns = attack_piece.getAttackPatterns();
  int distance;

  if (piece_col == attack_col) {
    distance = std::abs(piece_col = attack_col);
    if (attack_patterns.count(IPiece::AttackPattern::VERTICAL_ALL) > 0 ||
        (distance == 1 &&
         attack_patterns.count(IPiece::AttackPattern::VERTICAL_ONE) > 0)) {
      return true;
    }
  }

  if (piece_row == attack_row) {
    distance = std::abs(piece_row = attack_row);
    if (attack_patterns.count(IPiece::AttackPattern::HORIZONTAL_ALL) > 0 ||
        (distance == 1 &&
         attack_patterns.count(IPiece::AttackPattern::HORIZONTAL_ONE) > 0)) {
      return true;
    }
  }

  int col_diff = std::abs(piece_col - attack_col);
  int row_diff = std::abs(piece_row - attack_row);

  if (col_diff == row_diff) {
    if (attack_patterns.count(IPiece::AttackPattern::DIAGONAL_ALL) > 0 ||
        (col_diff == 1 && row_diff == 1 &&
         attack_patterns.count(IPiece::AttackPattern::DIAGONAL_ONE))) {
      return true;
    }
  }

  if ((col_diff == 2 && row_diff == 1) || (row_diff == 2 && col_diff == 1)) {
    if (attack_patterns.count(IPiece::AttackPattern::L_SHAPE) > 0) {
      return true;
    }
  }

  return false;
}

IPiece::Direction getOppositeDirection(IPiece::Direction direction) {
  switch (direction) {
  case IPiece::Direction::NORTH:
    return IPiece::Direction::SOUTH;
  case IPiece::Direction::NORTH_EAST:
    return IPiece::Direction::SOUTH_WEST;
  case IPiece::Direction::EAST:
    return IPiece::Direction::WEST;
  case IPiece::Direction::SOUTH_EAST:
    return IPiece::Direction::NORTH_WEST;
  case IPiece::Direction::SOUTH:
    return IPiece::Direction::NORTH;
  case IPiece::Direction::SOUTH_WEST:
    return IPiece::Direction::NORTH_EAST;
  case IPiece::Direction::WEST:
    return IPiece::Direction::EAST;
  case IPiece::Direction::NORTH_WEST:
    return IPiece::Direction::SOUTH_EAST;
  }
}
} // namespace PieceUtilities
