#include "IGame.h"
#include "interfaces/IBoard.h"
#include "interfaces/IPiece.h"
#include "interfaces/IPieceManager.h"
#include <iostream>
#include <regex>
#include <sstream>

#pragma once

class Game : public IGame {
public:
  Game(IBoard &board, IPieceManager &piece_manager);

  void start() override;

private:
  IBoard &m_board;
  IPieceManager &m_piece_manager;
  bool m_is_game_over;
  int m_turn_count;
  bool m_invalid_input;
};
