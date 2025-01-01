#include "IBoard.h"
#include "InputHandler.h"
#include "PieceFactory.h"
#include "PieceUtilities.h"
#include "interfaces/ICheckMateTracker.h"
#include <algorithm>
#include <cctype>
#include <map>
#include <regex>

#pragma once

class Board : public IBoard {
public:
  Board(PieceFactory &piece_factory, ICheckMateTracker &checkmate_tracker);

  void displayBoard() override;
  bool movePiece(const IPiece::PieceColor piece_color,
                 const std::string &from_pos,
                 const std::string &to_pos) override;
  bool tryCastling(const IPiece::PieceColor piece_color,
                   const std::string &target_pos) override;
  void onGameOver(std::function<void()> callback) override;

private:
  void generateBoard();
  void generatePawnRow(std::map<std::string, std::unique_ptr<IPiece>> &in_map,
                       IPiece::PieceColor color);
  void
  generateNonPawnRow(std::map<std::string, std::unique_ptr<IPiece>> &in_map,
                     IPiece::PieceColor color);
  void handleKingInCheckUpdate(const IPiece::PieceColor color,
                               const bool in_check);
  void handlePawnPromotion(const std::string &pos,
                           const IPiece::PieceColor piece_color);

  PieceFactory &m_piece_factory;
  ICheckMateTracker &m_checkmate_tracker;
  std::function<void()> m_game_over_callback;
  std::map<std::string, std::unique_ptr<IPiece>> m_board_map;
  bool m_white_king_in_check;
  bool m_black_king_in_check;
  std::vector<std::unique_ptr<IPiece>> m_white_pieces_captured;
  std::vector<std::unique_ptr<IPiece>> m_black_pieces_captured;
};
