#include "Game.h"

Game::Game(IBoard &board, IPieceManager &piece_manager)
    : m_board{board}, m_piece_manager{piece_manager} {}

void Game::start() {}

void Game::playMove(int start_x, int start_y, int end_x, int end_y) {}

bool Game::isGameOver() { return false; }
