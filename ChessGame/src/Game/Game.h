#include "IGame.h"
#include "InputHandler.h"
#include "interfaces/IBoard.h"
#include "interfaces/IPiece.h"
#include <iostream>
#include <regex>
#include <sstream>

#pragma once

class Game : public IGame {
public:
  Game(IBoard &board);

  void start() override;

private:
  bool validInput(const std::string &s1, const std::string &s2);

  void getHelpMessage();

  IBoard &m_board;
  bool m_is_game_over;
  int m_turn_count;
  bool m_display_board;
};
