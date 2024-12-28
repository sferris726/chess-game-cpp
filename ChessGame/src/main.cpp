#include "Board.h"
#include "Game.h"
#include <iostream>
#include <memory>

int main() {
  std::shared_ptr<Board> board = std::make_shared<Board>();
  std::unique_ptr<Game> game = std::make_unique<Game>(*board);
  return 0;
}
