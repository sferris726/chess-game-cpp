#include "IBoard.h"

#pragma once

class Board : public IBoard {
public:
  Board();

  void displayBoard() override;
  bool movePiece(int start_x, int start_y, int end_x, int end_y) override;
};