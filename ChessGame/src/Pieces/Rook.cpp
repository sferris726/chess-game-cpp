#include "Rook.h"

Rook::Rook(PieceColor color) : m_color{color} {}

bool Rook::isMoveValid(
    const std::string &from_pos, const std::string &to_pos,
    const std::map<std::string, std::unique_ptr<IPiece>> &board_map) {
  const auto &board_positions =
      PieceUtilities::convertBoardPosition(from_pos, to_pos);

  if (board_positions.size() < 2) {
    return false;
  }

  if (board_map.at(to_pos) && board_map.at(to_pos)->getColor() == getColor()) {
    return false;
  }

  int col = board_positions[0].first;
  int row = board_positions[0].second;

  if (col == board_positions[1].first) {
    while (true) {
      if (row < board_positions[1].second) {
        ++row;
      } else {
        --row;
      }

      std::string pos = PieceUtilities::getColLetter(col) + std::to_string(row);
      if (board_map.at(pos) != nullptr) {
        return false;
      }

      if (row == board_positions[1].second) {
        break;
      }
    }
  } else if (row == board_positions[1].second) {
    while (true) {
      if (col < board_positions[1].second) {
        ++col;
      } else {
        --col;
      }

      std::string pos = PieceUtilities::getColLetter(col) + std::to_string(row);
      if (board_map.at(pos) != nullptr) {
        return false;
      }

      if (col == board_positions[1].first) {
        break;
      }
    }
  } else {
    return false;
  }

  return true;
}

void Rook::setOrigin(const int col, const int row) {
  m_origin.first = col;
  m_origin.second = row;
}

char Rook::getSymbol() const { return 'R'; }

IPiece::PieceColor Rook::getColor() const { return m_color; }

std::string Rook::getColorStr() const {
  return PieceUtilities::convertPieceColorToStr(m_color);
}
