#include <expectiminmax.h>
#include <game.h>
#include <limits>
#include <memory>
#include <player_path.h>
#include <utils.h>
#include <vector>

using namespace std;

using ai::Move;

namespace algo {

constexpr int MAX_DEPTH = 6;
constexpr double WINNING_SCORE = 1000.0;
constexpr double LOSING_SCORE = -1000.0;
constexpr double BLOCKING_BONUS = 40;
constexpr double PROBABILITY = 1.0 / 6.0;

struct Expectiminmax {
  Expectiminmax(int players_count) : players_count(players_count) {
    scores = vector<double>(players_count);
    nodes_visited = 0;
  }

  enum class NodeType { MAX, CHANCE };

  // returns an extra score if the player move blocks one or more
  // of opponent pawns
  double evaluate_blocking(const Game &game, const Pawn &pawn,
                           const Player &current_player) {
    double blocking_score = 0.0;
    auto [x, y] = game.board.get_pawn_coordinates(pawn.colour, pawn.pos);

    for (const auto &player : game.board.players) {
      if (player.colour == current_player.colour) {
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

  vector<double> evaluate(const Game &game) {
    vector<double> scores(players_count, 0.0);

    if (game.ended()) {
      for (int i = 0; i < players_count; ++i) {
        auto cl = game.board.get_player(i).colour;
        scores[i] = (game.winner().colour == cl) ? WINNING_SCORE : LOSING_SCORE;
      }
      return scores;
    }

    for (int player_idx = 0; player_idx < players_count; ++player_idx) {
      const auto &current_player = game.board.get_player(player_idx);

      for (const auto &pawn : current_player.pawns) {
        scores[player_idx] += pawn.pos * 2.0;
        scores[player_idx] += pawn.has_reached_destination() * 100;
        scores[player_idx] += pawn.is_protected() * 10;
        scores[player_idx] += evaluate_blocking(game, pawn, current_player);
      }
    }

    return scores;
  }

  double maximise_player_move(Game &game, int depth, int type,
                              const int &steps) {
    double score = numeric_limits<double>::min();
    Move next_move;
    int next_type = (type + 1) % 2;
    for (auto pawn_id : game.get_all_pawns(steps)) {
      auto next_state = game.clone();
      next_state->move_current_player(pawn_id, steps);
      double score = expectiminmax(*next_state, depth - 1, next_type, steps);
      score = max(score, score);
      next_move.update(pawn_id, steps, score);
    }

    path[MAX_DEPTH - depth] = next_move;
    scores[MAX_DEPTH - depth] = score;
    return score;
  }

  double compute_chance_node(Game &game, int depth, int type,
                             const int &steps) {
    double alpha = 0;
    int next_idx = (type + 1) % 4;
    for (int steps = 1; steps <= 6; ++steps) {
      alpha += PROBABILITY * expectiminmax(game, depth - 1, next_idx, steps);
    }
    return alpha;
  }

  double expectiminmax(Game &game, int depth, int type, const int &steps) {
    nodes_visited += 1;
    if (depth == 0 || game.ended()) {
      return evaluate(game)[game.current_player_idx()];
    }

    switch (NodeType(type)) {
    case NodeType::MAX:
      return maximise_player_move(game, depth, type, steps);
    default:
      return compute_chance_node(game, depth, type, steps);
    }
  }

  vector<Move> path = vector<Move>(MAX_DEPTH);
  vector<double> scores;

  int players_count;
  int nodes_visited;
};

int choose_pawn(Game &game, const int &steps, vector<Move> &path) {
  static Expectiminmax ai(game.players_count());
  ai.expectiminmax(game, MAX_DEPTH, 0, steps);
  path = ai.path;
  // reverse(path.begin(), path.end());
  cout << center(dotted_line(format(
              "The number of nodes visited during Expectiminmax traversal = {}",
              ai.nodes_visited)))
       << '\n';
  return path.front().pawn_id;
}

} // namespace algo
