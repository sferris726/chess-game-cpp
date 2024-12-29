#include "Pawn.h"

Pawn::Pawn(PieceColor color) : m_color{color}, m_first_move_made{false} {}

bool Pawn::isMoveValid(const std::string &from_pos, const std::string &to_pos,
                       std::unique_ptr<IPiece> to_pos_piece) {
  const auto &board_positions =
      PieceUtilities::convertBoardPosition(from_pos, to_pos);

  if (board_positions.size() < 2) {
    return false; // Conversion problem
  }

  const int allowable_squares = m_first_move_made ? 1 : 2;
  const bool to_pos_occupied = to_pos_piece != nullptr;
  const bool diff_color = to_pos_piece->getColor() != getColor();

  // Check if move is vertical or diagonal
  if (board_positions[0].first == board_positions[1].first) {
    // Move is vertical
    if (!diff_color ||
        std::abs(board_positions[0].second - board_positions[1].second) >
            allowable_squares) {
      return false;
    }
  } else {
    // Move is diagonal
    if (!to_pos_occupied) {
      return false;
    }

    if (!diff_color ||
        std::abs(board_positions[0].first - board_positions[1].first > 1)) {
      return false;
    }
  }

  m_first_move_made = true;
  return true;
}

char Pawn::getSymbol() const { return 'P'; }

std::string Pawn::getColor() const {
  return PieceUtilities::convertPieceColorToStr(m_color);
}
