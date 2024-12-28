#include "IGame.h"
#include "interfaces/IBoard.h"

#pragma once

class Game : public IGame {
public:
  Game(IBoard &board);

  void start() override;
  void playMove(int start_x, int start_y, int end_x, int end_y) override;
  bool isGameOver() override;

private:
  IBoard &m_board;
};
