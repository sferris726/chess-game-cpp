#include "Board.h"
#include "CheckMateTracker.h"
#include "Game.h"
#include "PieceFactory.h"
#include "PieceManager.h"
#include <iostream>
#include <memory>

int main() {
  std::unique_ptr<PieceFactory> piece_factory =
      std::make_unique<PieceFactory>();
  std::unique_ptr<ICheckMateTracker> checkmate_tracker =
      std::make_unique<CheckMateTracker>();
  std::shared_ptr<IBoard> board =
      std::make_shared<Board>(*piece_factory, *checkmate_tracker);
  std::shared_ptr<IPieceManager> piece_manager =
      std::make_shared<PieceManager>(*piece_factory);
  std::unique_ptr<IGame> game = std::make_unique<Game>(*board, *piece_manager);

  game->start();
  return 0;
}
