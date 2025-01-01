#include "Pawn.h"

static const int VALID_NUM_SPOTS_FROM_ORIGIN = 3;
static const int VALID_OPPONENT_MOVES_FROM_ORIGIN = 2;
static const int ENABLE_PAWN_EXCHANGE_WHITE = 8;
static const int ENABLE_PAWN_EXCHANGE_BLACK = 1;

Pawn::Pawn(PieceColor color)
    : m_color{color}, m_first_move_made{false}, m_last_move{
                                                    std::pair("-", "-")} {}

IPiece::MoveInfo Pawn::getMoveInfo(
    const std::string &from_pos, const std::string &to_pos,
    const std::map<std::string, std::unique_ptr<IPiece>> &board_map) {
  MoveInfo move_info = MoveInfo{};
  const auto &board_positions =
      PieceUtilities::convertBoardPosition(from_pos, to_pos);

  if (board_positions.size() < 2) {
    return move_info;
  }

  // Check if move is vertical or diagonal
  if (board_positions[0].first == board_positions[1].first) {
    // Move is vertical
    int col = board_positions[0].first;
    int row = board_positions[0].second;
    int moves_made = 0;
    int allowable_moves = m_first_move_made ? 1 : 2;

    // Make sure nothing blocking move
    while (true) {
      if (m_color == PieceColor::WHITE) {
        ++row;
      } else {
        --row;
      }

      const std::string pos =
          PieceUtilities::getColLetter(col) + std::to_string(row);
      if (board_map.at(pos) != nullptr) {
        return move_info;
      }

      ++moves_made;
      if (moves_made == allowable_moves || row == board_positions[1].second) {
        break;
      }
    }

    // Check we've reached the target
    if (row != board_positions[1].second) {
      return move_info;
    }
  } else {
    // Move is diagonal
    if (board_map.at(to_pos) == nullptr) {
      // Check that we are 3 ranks from the origin
      const bool legal_en_passant =
          std::abs(m_origin.second - board_positions[0].second) ==
              VALID_NUM_SPOTS_FROM_ORIGIN &&
          m_origin.first == board_positions[0].first;

      if (legal_en_passant) {
        auto en_passant =
            checkEnPassant(board_map, board_positions[0], board_positions[1]);
        move_info.is_valid = en_passant.first;
        move_info.en_passant_valid = en_passant.first;
        move_info.en_passant_opponent = en_passant.second;
        return move_info;
      }

      return move_info; // Opponent doesn't exist, not valid
    }

    // Make sure move is not backwards
    if (m_color == IPiece::PieceColor::WHITE) {
      if (board_positions[0].second > board_positions[1].second) {
        return move_info;
      }
    } else {
      if (board_positions[0].second < board_positions[1].second) {
        return move_info;
      }
    }

    // Make sure opponent is valid and move is only 1 square diagonally
    const bool diff_color = board_map.at(to_pos)->getColor() != getColor();
    if (!diff_color ||
        std::abs(board_positions[0].first - board_positions[1].first > 1)) {
      return move_info;
    }
  }

  if (!m_first_move_made) {
    m_first_move_made = true;
  }

  m_last_move.first = from_pos;
  m_last_move.second = to_pos;
  move_info.is_valid = true;
  move_info.can_pawn_promote = enablePawnPromotion(board_positions[1].second);
  return move_info;
}

void Pawn::setOrigin(const int col, const int row) {
  m_origin.first = col;
  m_origin.second = row;
}

char Pawn::getSymbol() const { return 'P'; }

IPiece::PieceColor Pawn::getColor() const { return m_color; }

std::string Pawn::getColorStr() const {
  return PieceUtilities::convertPieceColorToStr(m_color);
}

std::pair<std::string, std::string> Pawn::getLastMove() const {
  return m_last_move;
}

std::set<IPiece::AttackPattern> Pawn::getAttackPatterns() const {
  std::set<AttackPattern> ret;
  ret.insert(AttackPattern::DIAGONAL_ONE);
  return ret;
}

std::pair<bool, std::string> Pawn::checkEnPassant(
    const std::map<std::string, std::unique_ptr<IPiece>> &board_map,
    const std::pair<int, int> &start, const std::pair<int, int> &end) {
  const std::string target_pos =
      PieceUtilities::getColLetter(end.first) + std::to_string(end.second);
  if (board_map.at(target_pos) != nullptr) {
    return std::pair(false, "");
  }

  int target_col = end.first;
  int target_row = 0;

  if (m_color == IPiece::PieceColor::WHITE) {
    // Going up the board
    target_row = end.second - 1;
  } else {
    // Going down the board
    target_row = end.second + 1;
  }

  std::string opp_pos =
      PieceUtilities::getColLetter(target_col) + std::to_string(target_row);
  if (board_map.at(opp_pos) == nullptr) {
    return std::pair(false, "");
  }

  if (board_map.at(opp_pos)->getColor() == getColor()) {
    return std::pair(false, "");
  }

  // make sure opponent move is 2 from their origin
  int opponent_last_row_move =
      std::atoi(&board_map.at(opp_pos)->getLastMove().first[1]);
  const bool valid_opponent_move =
      std::abs(opponent_last_row_move - target_row) ==
      VALID_OPPONENT_MOVES_FROM_ORIGIN;

  return std::pair(valid_opponent_move, opp_pos);
}

bool Pawn::enablePawnPromotion(int row) {
  if (m_color == PieceColor::WHITE) {
    return row == ENABLE_PAWN_EXCHANGE_WHITE;
  } else {
    return row == ENABLE_PAWN_EXCHANGE_BLACK;
  }
}
