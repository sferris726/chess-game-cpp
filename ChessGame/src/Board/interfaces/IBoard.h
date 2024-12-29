#include <string>

#pragma once

class IBoard {
public:
  virtual void displayBoard() = 0;
  virtual bool movePiece(const std::string &from_pos,
                         const std::string &to_pos) = 0;
};
