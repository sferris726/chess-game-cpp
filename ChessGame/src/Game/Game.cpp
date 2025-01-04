#include "Game.h"

static const std::regex PATTERN(R"(^[a-h][1-8]$)");

Game::Game(IBoard &board)
    : m_board{board}, m_is_game_over{false}, m_turn_count{0}, m_display_board{
                                                                  true} {
  m_board.onGameOver([this]() { m_is_game_over = true; });
}

void Game::start() {
  std::cout << "Welcome to cmd line chess.\nTo play, enter the locations you "
               "want to move (e.g. a2 a4). In the example a2 is the start "
               "position and a4 is the end position\n\n";

  while (true) {
    if (m_display_board) {
      m_board.displayBoard();
    }

    m_display_board = false; // Reset on every loop
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
      std::transform(str1.begin(), str1.end(), str1.begin(), ::tolower);

      if (!validInput(str1, str2)) {
        std::cout << "Invalid input, please try again" << std::endl;
        continue;
      }

      if (str1 == "help" || str1 == "h") {
        getHelpMessage();
      } else if (str1 == "r" || str1 == "resign") {
        std::cout << (m_turn_count % 2 == 0 ? "White" : "Black")
                  << " resigned, "
                  << (m_turn_count % 2 == 0 ? "Black" : "White") << " wins!\n";
        break;
      } else if (str1 == "castle") {
        const bool castling_success = m_board.tryCastling(color_move, str2);
        if (castling_success) {
          m_turn_count++;
          m_display_board = true;
        } else {
          std::cout << "Castling invalid:\n- Valid command example: castle "
                       "a4\n- Ensure King is not in check or moving through or "
                       "to check\n- Ensure King and rook have not moved\n";
        }
      } else {
        const bool success = m_board.movePiece(color_move, str1, str2);
        if (success) {
          m_turn_count++;
          m_display_board = true;
        } else {
          std::cout << "Invalid move, please try again" << std::endl;
        }
      }
    } else {
      std::cout << "Invalid input, enter help or h for rules and instructions"
                << std::endl;
    }

    if (m_is_game_over) {
      m_board.displayGameOver();
      break;
    }
  }
}

bool Game::validInput(const std::string &s1, const std::string &s2) {
  if ((s1 == "resign" || s1 == "r" || s1 == "help" || s1 == "h") &&
      s2.empty()) {
    return true;
  }

  if (s1 == "castle") {
    return s2.size() == 2 && std::regex_match(s2, PATTERN);
  }

  return s1.size() == 2 && s2.size() == 2 && std::regex_match(s1, PATTERN) &&
         std::regex_match(s2, PATTERN);
}

void Game::getHelpMessage() {
  std::cout << "CMD Line Chess Rules:\n";
  std::cout << "- To move a piece, enter the position on the board you want to "
               "move to (e.g. a2 a4)\n";
  std::cout << "- To perform a castling move, enter 'castle' with the position "
               "to move the King (e.g. castle g1)\n";
  std::cout << "- To promote a pawn, enter the piece letter to promote to "
               "(e.g. Q for Queen)\n";
  std::cout << "- En Passant is a legal move in this game\n";
  std::cout << "- To resign the game, enter 'r' or 'resign'\n";
}
