#ifndef AI_H
#define AI_H

#include <game.h>

struct Game;

namespace ai {
struct Move {
  int pawn_id;
  int steps;
  double score;

  Move() : pawn_id(0), steps(0), score(-1000) {}

  Move(int id, int s, double sc) : pawn_id(id), steps(s), score(sc) {}

  void update(int id, int s, double sc) {
    if (sc > score) {
      pawn_id = id;
      steps = s;
      score = sc;
    }
  }
};

int choose_pawn(Game &game, const int &steps, vector<Move> &path);
} // namespace ai

#endif
