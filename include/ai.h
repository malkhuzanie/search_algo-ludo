#ifndef AI_H
#define AI_H

#include <game.h>

struct Game;

namespace ai {
int choose_pawn(Game &game, const int &steps);
} // namespace ai

#endif
