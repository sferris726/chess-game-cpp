#include "interfaces/ICheckMateTracker.h"
#include "interfaces/IPiece.h"
#include <vector>

#pragma once

namespace PieceUtilities {
/**
 * @brief Convert user input to a vector (from, to) of pairs (x, y)
 *
 * @param from_pos
 * @param to_pos
 * @returns vector of pairs
 */
std::vector<std::pair<int, int>>
convertBoardPosition(const std::string &from_pos, const std::string &to_pos);

std::string getColLetter(int col);

int getColNum(const char col);

IPiece::PieceType convertStrToPieceType(const std::string &str);

bool canAttackPatternThreaten(IPiece::Direction direction,
                              IPiece::AttackPattern attack,
                              bool is_one_rank_from);

bool canPieceBeAttacked(const std::string &piece_pos,
                        const std::string &attack_pos,
                        const IPiece &attack_piece);

IPiece::Direction getOppositeDirection(IPiece::Direction direction);
} // namespace PieceUtilities
