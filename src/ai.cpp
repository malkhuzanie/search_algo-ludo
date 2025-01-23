#include <ai.h>
#include <game.h>
#include <limits>
#include <memory>
#include <player_path.h>
#include <utils.h>
#include <vector>

using namespace std;

namespace ai {

constexpr int MAX_DEPTH = 3;
constexpr double WINNING_SCORE = 1000.0;
constexpr double LOSING_SCORE = -1000.0;
constexpr double BLOCKING_BONUS = 40;
constexpr double PROBABILITY = 1.0 / 6.0;

struct LudoAI {
  LudoAI(int players_count) : players_count(players_count) {
    scores = vector<double>(players_count);
    nodes_visited = 0;
  }

  enum class NodeType { MAX, CHANCE, MIN, CHANCE };

  // returns an extra score if the player move blocks one or more
  // of opponent pawns
  double evaluate_blocking(const Game &game, const Pawn &pawn) {
    double blocking_score = 0.0;
    auto [x, y] = game.board.get_pawn_coordinates(pawn.colour, pawn.pos);

    for (const auto &player : game.board.players) {
      if (player.colour == game.current_player_colour()) {
        continue;
      }
      for (const auto &op_pawn : player.pawns) {
        if (op_pawn.is_at_home() || op_pawn.has_reached_destination()) {
          continue;
        }
        auto op_cor =
            game.board.get_pawn_coordinates(op_pawn.colour, op_pawn.pos);

        auto path = player_path::get_path(pawn.colour);
        auto it = find(path.begin(), path.end(), op_cor);
        if (it != path.end()) {
          if (pawn.pos > it - path.begin()) {
            blocking_score += BLOCKING_BONUS;
          }
        }
      }
    }
    return blocking_score;
  }

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
      if (!pawn.is_at_home()) {
        score += pawn.pos * 2.0;
      }
      score += pawn.has_reached_destination() * 100;
      score += pawn.is_protected() * 10;
      score += evaluate_blocking(game, pawn);
    }

    return score;
  }

  // take out all possible player moves.
  vector<Move> take_moves(Game &game, int depth, int type, const int &steps) {
    vector<Move> issued_moves;
    for (int pawn_id : game.get_all_pawns(steps)) {
      auto next_state = game.clone();
      next_state->move_current_player(pawn_id, steps);
      type = (type + 1) % 4;
      double alpha = expectiminmax(*next_state, depth - 1, type, steps);
      issued_moves.emplace_back(pawn_id, steps, alpha);
    }
    return issued_moves;
  }

  double expectiminmax_player(Game &game, int depth, int type,
                              const int &steps) {
    double alpha = numeric_limits<double>::max();
    auto issued_moves = take_moves(game, depth, type, steps);
    Move next_move;
    for (auto &[pawn_id, steps, score] : issued_moves) {
      alpha = min(alpha, score);
      next_move.update(pawn_id, steps, score);
    }
    path[MAX_DEPTH - depth] = next_move;
    return alpha;
  }

  double expectiminmax_ai(Game &game, int depth, int type, const int &steps) {
    double alpha = numeric_limits<double>::min();
    auto issued_moves = take_moves(game, depth, type, steps);
    Move next_move;
    for (auto &[pawn_id, steps, score] : issued_moves) {
      alpha = max(alpha, score);
      next_move.update(pawn_id, steps, score);
    }
    path[MAX_DEPTH - depth] = next_move;
    return alpha;
  }

  double expectiminmax(Game &game, int depth, int type, const int &steps) {
    nodes_visited += 1;
    if (depth == 0 || game.ended()) {
      return evaluate(game);
    }

    switch (NodeType(type)) {
    case NodeType::MAX:
      return expectiminmax_ai(game, depth, type, steps);
    case NodeType::MIN:
      return expectiminmax_player(game, depth, type, steps);
    default:
      double alpha = 0;
      int next_idx = (type + 1) % 4;
      for (int steps = 1; steps <= 6; ++steps) {
        alpha += PROBABILITY * expectiminmax(game, depth - 1, next_idx, steps);
      }
      return alpha;
    }
  }

  vector<Move> path = vector<Move>(MAX_DEPTH);
  vector<double> scores;

  int players_count;
  int nodes_visited;
};

int choose_pawn(Game &game, const int &steps, vector<Move> &path) {
  static LudoAI ai(game.players_count());
  ai.expectiminmax(game, MAX_DEPTH, 0, steps);
  path = ai.path;
  reverse(path.begin(), path.end());
  cout << center(dotted_line(format(
              "The number of nodes visited during Expectiminmax traversal = {}",
              ai.nodes_visited)))
       << '\n';
  return path.front().pawn_id;
}

} // namespace ai
