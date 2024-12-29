#include "IBoard.h"
#include "PieceFactory.h"
#include "interfaces/IPieceManager.h"
#include <map>

#pragma once

class Board : public IBoard {
public:
  Board(PieceFactory &piece_factory);

  void displayBoard() override;
  bool movePiece(const IPiece::PieceColor piece_color,
                 const std::string &from_pos,
                 const std::string &to_pos) override;
  void onGameOver(std::function<void()> callback) override;

private:
  void generateBoard();
  void generatePawnRow(std::map<std::string, std::unique_ptr<IPiece>> &in_map,
                       IPiece::PieceColor color);
  void
  generateNonPawnRow(std::map<std::string, std::unique_ptr<IPiece>> &in_map,
                     IPiece::PieceColor color);
  std::string getColLetter(int col);

  PieceFactory &m_piece_factory;
  std::function<void()> m_game_over_callback;
  std::map<std::string, std::unique_ptr<IPiece>> m_board_map;
};
