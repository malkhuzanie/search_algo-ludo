#ifndef BOARD_H
#define BOARD_H

#include <constants.h>
#include <grid.h>
#include <iostream>
#include <map>
#include <optional>
#include <pawn.h>
#include <player.h>
#include <player_colour.h>
#include <player_path.h>
#include <set>
#include <vector>

using namespace std;

struct Board {
  Board() {}

  Board(int players_count) : players_count(players_count) {
    for (int i = 0; i < players_count; ++i) {
      players.emplace_back(static_cast<PlayerColour>(i), i * TOKEN_COUNT);
    }
  }

  pair<int, int> get_pawn_coordinates(const PlayerColour &colour,
                                      const unsigned int &pos) {
    return player_path::get_path(colour)[pos];
  }

  optional<Pawn *> get_pawn_at_coordinates(Player &player, pair<int, int> cor) {
    for (auto &pawn : player.pawns) {
      if (pawn.is_at_home()) {
        continue;
      }
      if (get_pawn_coordinates(pawn.colour, pawn.pos) == cor)
        return &pawn;
    }
    return nullopt;
  }

  bool check_clash(const Pawn &pawn) {
    auto cor = get_pawn_coordinates(pawn.colour, pawn.pos);
    for (auto &player : players) {
      if (player.colour == pawn.colour) {
        continue;
      }
      auto top_pawn = get_pawn_at_coordinates(player, cor);
      if (top_pawn.has_value()) {
        (*top_pawn)->go_back_home();
        return true;
      }
    }
    return false;
  }

  void set_next_player() {
    _current_player = (_current_player + 1) % players_count;
  }

  Player &current_player() { return players[_current_player]; }

  optional<int> next_wall(PlayerColour colour, const int &pos,
                          const int &steps) {
    pair<int, int> cor = player_path::get_path(colour)[pos];
    for (int i = pos + 1; i <= pos + steps; ++i) {
      for (auto &player : players) {
        if (player.colour == colour) {
          continue;
        }
        int count = 0;
        for (auto &pawn : player.pawns) {
          if (pawn.is_at_home()) {
            continue;
          }
          count += (pawn.get_coordinates() == cor);
        }
        if (count >= 2) {
          return pos + 1;
        }
      }
    }
    return nullopt;
  }

  bool move_current(const unsigned int &id, int &steps) {
    auto pawn = current_player().get_pawn(id);
    if (steps == 6 && pawn.is_at_home()) {
      steps = 1;
    }
    auto wall = next_wall(current_player().colour, pawn.pos, steps);
    if (wall.has_value()) {
      steps = *wall - pawn.pos - 1;
    }
    current_player().move(id, steps);
    return check_clash(current_player().get_pawn(id));
  }

  vector<int> get_all_pawns(const int &steps) {
    vector<int> pawns;
    for (auto &pawn : current_player().pawns) {
      if (steps != 6 && pawn.is_at_home()) {
        continue;
      }
      if (!pawn.has_reached_destination()) {
        pawns.emplace_back(pawn.id);
      }
    }
    return pawns;
  }

  bool ended() {
    for (auto &player : players) {
      if (player.is_winner())
        return true;
    }
    return false;
  }

  Player &winner() {
    return *find_if(players.begin(), players.end(),
                    [&](auto &p) { return p.is_winner(); });
  }

  string to_string() {
    string res = "";
    for (auto &x : current_state()) {
      for (auto &y : x) {
        res += y;
      }
      res += '\n';
    }
    return res;
  }

  pair<int, int> get_pawn_home(PlayerColour colour) {
    auto &home = tokens[colour];
    auto home_pos = home.front();
    rotate(home.begin(), home.begin() + 1, home.end());
    return home_pos;
  }

  array<string, rows> current_state() {
    auto state = grid;
    for (auto &player : players) {
      for (auto &pawn : player.pawns) {
        if (pawn.has_reached_destination()) {
          continue;
        }
        if (pawn.is_at_home()) {
          auto [x, y] = get_pawn_home(pawn.colour);
          state[x][y] = '0' + pawn.id;
        } else {
          auto [x, y] = get_pawn_coordinates(pawn.colour, pawn.pos);
          state[x][y] = '0' + (char)pawn.id;
        }
      }
    }
    return state;
  }

  array<string, rows> grid = {
      "gggggg...yyyyyy", //
      "g....g.YYy....y", //
      "g....g.Y.y....y", //
      "g....g.Y.y....y", //
      "g....g.Y.y....y", //
      "gggggg.Y.yyyyyy", //
      ".G....#Y#......", //
      ".GGGGGG#BBBBBB.", //
      "......#R#....B.", //
      "rrrrrr.R.bbbbbb", //
      "r....r.R.b....b", //
      "r....r.R.b....b", //
      "r....r.R.b....b", //
      "r....rRR.b....b", //
      "rrrrrr...bbbbbb", //
  };

  // represts tokens starting position "player's home"
  map<PlayerColour, vector<pair<int, int>>> tokens = {
      {PlayerColour::RED, {{11, 2}, {12, 2}, {11, 3}, {12, 3}}},
      {PlayerColour::BLUE, {{11, 11}, {12, 11}, {11, 11}, {12, 12}}},
      {PlayerColour::GREEN, {{2, 2}, {3, 2}, {2, 3}, {3, 3}}},
      {PlayerColour::YELLOW, {{2, 11}, {3, 11}, {2, 12}, {3, 12}}}};

  unsigned int players_count;
  unsigned int _current_player = 0;
  vector<Player> players;
};

#endif
