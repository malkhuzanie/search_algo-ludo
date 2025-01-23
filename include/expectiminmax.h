#ifndef EXPECTIMINMAX_H
#define EXPECTIMINMAX_H

#include <ai.h>
#include <game.h>

struct Game;

namespace algo {
int choose_pawn(Game &game, const int &steps, vector<ai::Move> &path);
} // namespace algo

#endif
