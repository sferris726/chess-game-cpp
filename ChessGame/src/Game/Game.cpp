#include "Game.h"

static const std::regex PATTERN("^[a-h1-8]+$");

Game::Game(IBoard &board, IPieceManager &piece_manager)
    : m_board{board}, m_piece_manager{piece_manager}, m_is_game_over{false},
      m_turn_count{0} {
  m_board.onGameOver([this]() { m_is_game_over = true; });
}

void Game::start() {
  std::cout << "Welcome to cmd line chess.\nTo play, enter the locations you "
               "want to move (e.g. a2 a4). In the example a2 is the start "
               "position and a4 is the end position\n\n";

  while (true) {
    m_board.displayBoard();

    std::string input;
    std::string from_pos;
    std::string to_pos;
    IPiece::PieceColor color_move;

    if (m_turn_count % 2 == 0) {
      color_move = IPiece::PieceColor::WHITE;
      std::cout << "White turn, please enter move: ";
    } else {
      color_move = IPiece::PieceColor::BLACK;
      std::cout << "Black turn, please enter move: ";
    }

    std::getline(std::cin, input);
    std::stringstream ss(input);
    if (ss >> from_pos >> to_pos) {
      if (ss.eof()) {
        if (!std::regex_match(from_pos, PATTERN) ||
            !std::regex_match(to_pos, PATTERN)) {
          std::cout << "Invalid input, please try again" << std::endl;
          continue;
        }

        const bool success = m_board.movePiece(color_move, from_pos, to_pos);
        if (success) {
          m_turn_count++;
        } else {
          std::cout << "Invalid move, please try again" << std::endl;
        }
      } else {
        std::cout << "Please enter exactly 2 inputs (start, end). Ex. a2 a4"
                  << std::endl;
      }
    } else {
      std::cout << "Please enter exactly 2 inputs (start, end). Ex. a2 a4"
                << std::endl;
    }

    if (m_is_game_over) {
      break;
    }
  }
}
