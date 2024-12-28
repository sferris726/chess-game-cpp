#include "Game.h"

Game::Game(IBoard &board) : m_board{board} {}

void Game::start() {}

void Game::playMove(int start_x, int start_y, int end_x, int end_y) {}

bool Game::isGameOver() { return false; }