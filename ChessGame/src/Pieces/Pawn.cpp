#include "Pawn.h"

Pawn::Pawn(PieceColor color) : m_color{color}, m_first_move_made{false} {}

bool Pawn::isMoveValid(
    const std::string &from_pos, const std::string &to_pos,
    const std::map<std::string, std::unique_ptr<IPiece>> &board_map) {
  const auto &board_positions =
      PieceUtilities::convertBoardPosition(from_pos, to_pos);

  if (board_positions.size() < 2) {
    return false;
  }

  // Check if move is vertical or diagonal
  if (board_positions[0].first == board_positions[1].first) {
    // Move is vertical
    int col = board_positions[0].first;
    int row = board_positions[0].second;

    // Make sure nothing blocking move
    if (m_color == IPiece::PieceColor::WHITE) {
      while (row < board_positions[1].second) {
        ++row;
        std::string pos = PieceUtilities::getColLetter(col) + std::to_string(row);
        if (board_map.at(pos) != nullptr) {
          return false;
        }
      }
    } else {
      while (row > board_positions[1].second) {
        --row;
        std::string pos = PieceUtilities::getColLetter(col) + std::to_string(row);
        if (board_map.at(pos) != nullptr) {
          return false;
        }
      }
    }
  } else {
    // Move is diagonal
    if (board_map.at(to_pos) == nullptr) {
      return false; // Opponent doesn't exist, not valid
    }

    // Make sure move is not backwards
    if (m_color == IPiece::PieceColor::WHITE) {
      if (board_positions[0].second > board_positions[1].second) {
        return false;
      }
    } else {
      if (board_positions[0].second < board_positions[1].second) {
        return false;
      }
    }

    // Make sure opponent is valid and move is only 1 square diagonally
    const bool diff_color = board_map.at(to_pos)->getColor() != getColor();
    if (!diff_color ||
        std::abs(board_positions[0].first - board_positions[1].first > 1)) {
      return false;
    }
  }

  m_first_move_made = true;
  return true;
}

char Pawn::getSymbol() const { return 'P'; }

IPiece::PieceColor Pawn::getColor() const {
  return m_color;
}

std::string Pawn::getColorStr() const {
  return PieceUtilities::convertPieceColorToStr(m_color);
}
