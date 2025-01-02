#include "Board.h"
#include "CheckMateTracker.h"
#include "Game.h"
#include "PieceFactory.h"
#include <iostream>
#include <locale>
#include <memory>

int main() {
  std::setlocale(LC_ALL, "");
  // std::wcout << L'♖' << std::endl;

  std::unique_ptr<PieceFactory> piece_factory =
      std::make_unique<PieceFactory>();
  std::unique_ptr<ICheckMateTracker> checkmate_tracker =
      std::make_unique<CheckMateTracker>();
  std::shared_ptr<IBoard> board =
      std::make_shared<Board>(*piece_factory, *checkmate_tracker);
  std::unique_ptr<IGame> game = std::make_unique<Game>(*board);

  game->start();
  return 0;
}
