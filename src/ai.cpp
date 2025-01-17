
#include <ai.h>
#include <game.h>

namespace ai {
int choose_pawn(Game &game, const int &steps) {
  vector<int> pawns = game.board.get_all_pawns(steps);
  return pawns[Game::rand(0, pawns.size() - 1)];
}
} // namespace ai
