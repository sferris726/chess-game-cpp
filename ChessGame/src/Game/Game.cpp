#include "Game.h"

static const std::regex PATTERN("^[a-h1-8]+$");

Game::Game(IBoard &board, IPieceManager &piece_manager)
    : m_board{board}, m_piece_manager{piece_manager}, m_is_game_over{false},
      m_turn_count{0}, m_invalid_input{false} {
  m_board.onGameOver([this]() { m_is_game_over = true; });
}

void Game::start() {
  std::cout << "Welcome to cmd line chess.\nTo play, enter the locations you "
               "want to move (e.g. a2 a4). In the example a2 is the start "
               "position and a4 is the end position\n\n";

  while (true) {
    if (!m_invalid_input) {
      m_board.displayBoard();
    }

    m_invalid_input = false; // Reset on every loop
    std::string input;
    std::string prompt;
    std::string from_pos;
    std::string to_pos;
    IPiece::PieceColor color_move;

    if (m_turn_count % 2 == 0) {
      color_move = IPiece::PieceColor::WHITE;
      prompt = "White turn, please enter move: ";
    } else {
      color_move = IPiece::PieceColor::BLACK;
      prompt = "Black turn, please enter move: ";
    }

    input = InputHandler::getInput(prompt);
    if (InputHandler::parseStr(input, from_pos, to_pos)) {
      if (!validInput(from_pos, to_pos)) {
        m_invalid_input = true;
        std::cout << "Invalid input, please try again" << std::endl;
        continue;
      }

      const bool success = m_board.movePiece(color_move, from_pos, to_pos);
      if (success) {
        m_turn_count++;
      } else {
        m_invalid_input = true;
        std::cout << "Invalid move, please try again" << std::endl;
      }
    } else {
      m_invalid_input = true;
      std::cout << "Please enter exactly 2 inputs (start, end). Ex. a2 a4"
                << std::endl;
    }

    if (m_is_game_over) {
      break;
    }
  }
}

bool Game::validInput(const std::string &s1, const std::string &s2) {
  return s1.size() == 2 && s2.size() == 2 && std::regex_match(s1, PATTERN) &&
         std::regex_match(s2, PATTERN);
}
