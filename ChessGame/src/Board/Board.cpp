
#include "Board.h"
#include "iostream"

Board::Board() {}

void Board::displayBoard() {
  for (int i = 0; i < 8; ++i) {
    for (int j = 0; j < 8; ++j) {
      std::cout << "[]";
    }
    std::cout << std::endl;
  }
}

bool Board::movePiece(int start_x, int start_y, int end_x, int end_y) {
  return false;
}