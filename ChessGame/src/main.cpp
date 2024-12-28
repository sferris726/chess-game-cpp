#include "Board.h"
#include "Game.h"
#include "PieceManager.h"
#include "PieceFactory.h"
#include <iostream>
#include <memory>

int main() {
  std::shared_ptr<IBoard> board = std::make_shared<Board>();
  std::unique_ptr<PieceFactory> piece_factory = std::make_unique<PieceFactory>();
  std::shared_ptr<IPieceManager> piece_manager =
      std::make_shared<PieceManager>(*piece_factory);
  std::unique_ptr<IGame> game = std::make_unique<Game>(*board, *piece_manager);
  return 0;
}
