#include "interfaces/ICheckMateTracker.h"
#include "interfaces/IPiece.h"
#include <vector>

#pragma once

namespace PieceUtilities {
static const std::set<IPiece::Direction> DIRECTIONS = {
    IPiece::Direction::NORTH, IPiece::Direction::NORTH_EAST,
    IPiece::Direction::EAST,  IPiece::Direction::SOUTH_EAST,
    IPiece::Direction::SOUTH, IPiece::Direction::SOUTH_WEST,
    IPiece::Direction::WEST,  IPiece::Direction::NORTH_WEST};

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

bool doesMovePutKingInCheck(
    const IPiece::PieceColor color, const std::string &king_pos,
    const std::string &moving_from, const std::string &moving_to,
    const std::map<std::string, std::unique_ptr<IPiece>> &board_map);

bool canAttackPatternThreaten(IPiece::Direction direction,
                              IPiece::AttackPattern attack,
                              bool is_one_rank_from);

bool canPieceBeAttacked(const std::string &piece_pos,
                        const std::string &attack_pos,
                        const IPiece &attack_piece);

bool isOneAwayFromKing(IPiece::Direction direction_to_king, int king_col,
                       int king_row, const std::string &piece_pos);

IPiece::Direction getOppositeDirection(IPiece::Direction direction);

void moveDirection(IPiece::Direction direction, int &col, int &row);
} // namespace PieceUtilities
