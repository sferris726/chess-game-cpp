#include "interfaces/IPiece.h"
#include <vector>

#pragma once

namespace PieceUtilities {
std::string convertPieceColorToStr(IPiece::PieceColor color_type);

/**
 * @brief Convert user input to a vector (from, to) of pairs (x, y)
 *
 * @param from_pos
 * @param to_pos
 * @returns vector of pairs
 */
std::vector<std::pair<int, int>>
convertBoardPosition(const std::string &from_pos, const std::string &to_pos);

int getColNum(const char col);
} // namespace PieceUtilities
