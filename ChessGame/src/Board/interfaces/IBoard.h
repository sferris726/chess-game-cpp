#include "interfaces/IPiece.h"
#include <functional>
#include <string>

#pragma once

class IBoard {
public:
  virtual void displayBoard() = 0;
  virtual bool movePiece(const IPiece::PieceColor piece_color,
                         const std::string &from_pos,
                         const std::string &to_pos) = 0;
  virtual void onGameOver(std::function<void()> callback) = 0;
};
