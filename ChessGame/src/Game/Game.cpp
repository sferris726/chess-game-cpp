#include "Game.h"

static const std::regex PATTERN(R"(^[a-h][1-8]$)");
static const std::regex CASTLE_PATTERN(R"(^castle$)",
                                       std::regex_constants::icase);

Game::Game(IBoard &board)
    : m_board{board}, m_is_game_over{false},
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
    std::string str1;
    std::string str2;
    IPiece::PieceColor color_move;

    if (m_turn_count % 2 == 0) {
      color_move = IPiece::PieceColor::WHITE;
      prompt = "White turn, please enter move: ";
    } else {
      color_move = IPiece::PieceColor::BLACK;
      prompt = "Black turn, please enter move: ";
    }

    input = InputHandler::getInput(prompt);
    if (InputHandler::parseStr(input, str1, str2)) {
      if (!validInput(str1, str2)) {
        m_invalid_input = true;
        std::cout << "Invalid input, please try again" << std::endl;
        continue;
      }

      std::transform(str1.begin(), str1.end(), str1.begin(), ::tolower);
      if (str1 == "castle") {
        const bool castling_success = m_board.tryCastling(color_move, str2);
        if (castling_success) {
          ++m_turn_count;
        } else {
          m_invalid_input = true;
          std::cout << "Castling invalid:\n- Valid command example: castle "
                       "a4\n- Ensure King is not in check or moving through or "
                       "to check\n- Ensure King and rook have not moved\n";
        }
      } else {
        const bool success = m_board.movePiece(color_move, str1, str2);
        if (success) {
          m_turn_count++;
        } else {
          m_invalid_input = true;
          std::cout << "Invalid move, please try again" << std::endl;
        }
      }
    } else {
      m_invalid_input = true;
      std::cout << "Please enter exactly 2 inputs (start, end). Ex. a2 a4"
                << std::endl;
    }

    if (m_is_game_over) {
      std::cout << "GAME OVER!!!!!!\n";
      break;
    }
  }
}

bool Game::validInput(const std::string &s1, const std::string &s2) {
  return (s1.size() == 2 || s1.size() == 6) && s2.size() == 2 &&
         (std::regex_match(s1, PATTERN) ||
          std::regex_match(s1, CASTLE_PATTERN)) &&
         std::regex_match(s2, PATTERN);
}
