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
} // namespace PieceUtilities
