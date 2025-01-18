#include <ai.h>
#include <game.h>
#include <limits>
#include <memory>
#include <vector>

using namespace std;

namespace ai {

constexpr int MAX_DEPTH = 3;
constexpr double WINNING_SCORE = 1000.0;
constexpr double LOSING_SCORE = -1000.0;
constexpr double PROBABILITY = 1.0 / 6.0; // dice face probability

struct LudoAI {
  double evaluate(const Game &game) {
    if (game.ended()) {
      if (game.winner().colour == game.current_player_colour()) {
        return WINNING_SCORE;
      }
      return LOSING_SCORE;
    }

    double score = 0.0;
    const auto &current_player = game.board.current_player();

    for (const auto &pawn : current_player.pawns) {
      if (pawn.has_reached_destination()) {
        score += 100.0;
      } else if (!pawn.is_at_home()) {
        score += pawn.pos * 2.0;
      }
      score += pawn.is_protected() * 10;
    }

    return score;
  }

  double expectiminmax(Game &game, int depth, bool is_max, int current_player) {
    if (depth == 0 || game.ended()) {
      return evaluate(game);
    }

    Move next_move;
    if (is_max) {
      double best_score = numeric_limits<double>::lowest();
      for (int steps = 1; steps <= 6; ++steps) {
        auto pawns = game.get_all_pawns(steps);

        for (int pawn_id : pawns) {
          auto next_state = game.clone();
          next_state->move_current_player(pawn_id, steps);

          double score = expectiminmax(*next_state, depth - 1, false,
                                       game.next_player_id(current_player));

          best_score = max(best_score, score * PROBABILITY);
          next_move.update(pawn_id, steps, score);
        }
      }
      _path[MAX_DEPTH - depth] = next_move;
      return best_score;
    } else {
      // Chance node (dice roll)
      double expected_score = 0.0;
      for (int steps = 1; steps <= 6; ++steps) {
        auto pawns = game.get_all_pawns(steps);

        if (pawns.empty()) {
          continue;
        }

        double best_op_score = numeric_limits<double>::lowest();
        for (int pawn_id : pawns) {
          auto next_state = game.clone();
          next_state->move_current_player(pawn_id, steps);

          double score =
              expectiminmax(*next_state, depth - 1, true, current_player);
          best_op_score = max(best_op_score, score);
          next_move.update(pawn_id, steps, score);
        }
        expected_score += PROBABILITY * best_op_score;
      }
      _path[MAX_DEPTH - depth] = next_move;
      return expected_score;
    }
  }

  int choose_pawn(Game &game, const int &steps) {
    vector<Move> possible_moves;
    auto pawns = game.get_all_pawns(steps);

    if (pawns.empty()) {
      return -1;
    }

    if (pawns.size() == 1) {
      return pawns[0];
    }

    double best_score = numeric_limits<double>::lowest();
    int best_pawn_id = pawns[0];

    for (int pawn_id : pawns) {
      auto next_state = game.clone();
      next_state->move_current_player(pawn_id, steps);

      double score =
          expectiminmax(*next_state, MAX_DEPTH, false, game.next_player_id());

      if (score > best_score) {
        path = _path;
        best_score = score;
        best_pawn_id = pawn_id;
      }
    }

    path.emplace_back(best_pawn_id, steps, best_score);
    return best_pawn_id;
  }

  vector<Move> path;
  vector<Move> _path = vector<Move>(MAX_DEPTH);
};

int choose_pawn(Game &game, const int &steps, vector<Move> &path) {
  static LudoAI ai;
  int pawn_id = ai.choose_pawn(game, steps);
  path = ai.path;
  reverse(path.begin(), path.end());
  return pawn_id;
}

} // namespace ai
