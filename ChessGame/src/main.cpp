#include "Board.h"
#include "Game.h"
#include <iostream>
#include <memory>

int main() {
  std::shared_ptr<Board> board = std::make_shared<Board>();
  board->displayBoard();
  return 0;
}