#include "Board.h"
#include "CheckMateTracker.h"
#include "Game.h"
#include "PieceFactory.h"
#include "StalemateTracker.h"
#include <iostream>
#include <memory>

int main() {
  std::unique_ptr<PieceFactory> piece_factory =
      std::make_unique<PieceFactory>();
  std::unique_ptr<ICheckMateTracker> checkmate_tracker =
      std::make_unique<CheckMateTracker>();
  std::unique_ptr<IStalemateTracker> stalemate_tracker =
      std::make_unique<StalemateTracker>();
  std::shared_ptr<IBoard> board = std::make_shared<Board>(
      *piece_factory, *checkmate_tracker, *stalemate_tracker);
  std::unique_ptr<IGame> game = std::make_unique<Game>(*board);

  game->start();
  return 0;
}
