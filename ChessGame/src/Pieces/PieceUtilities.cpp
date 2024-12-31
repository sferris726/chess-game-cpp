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

bool canAttackPatternThreaten(ICheckMateTracker::Direction direction,
                              IPiece::AttackPattern attack,
                              bool is_one_rank_from) {
  switch (direction) {
  case ICheckMateTracker::Direction::NORTH:
  case ICheckMateTracker::Direction::SOUTH:
    return attack == IPiece::AttackPattern::VERTICAL_ALL ||
           (is_one_rank_from && attack == IPiece::AttackPattern::VERTICAL_ONE);

  case ICheckMateTracker::Direction::NORTH_EAST:
  case ICheckMateTracker::Direction::SOUTH_EAST:
  case ICheckMateTracker::Direction::NORTH_WEST:
  case ICheckMateTracker::Direction::SOUTH_WEST:
    return attack == IPiece::AttackPattern::DIAGONAL_ALL ||
           (is_one_rank_from && attack == IPiece::AttackPattern::DIAGONAL_ONE);

  case ICheckMateTracker::Direction::EAST:
  case ICheckMateTracker::Direction::WEST:
    return attack == IPiece::AttackPattern::HORIZONTAL_ALL ||
           (is_one_rank_from &&
            attack == IPiece::AttackPattern::HORIZONTAL_ONE);

  default:
    return false;
  }
}
} // namespace PieceUtilities
