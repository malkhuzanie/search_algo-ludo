#include <board.h>
#include <cassert>
#include <constants.h>
#include <iostream>
#include <player_path.h>

using namespace std;

Board::Board(int players_count) : players_count(players_count) {
  for (int i = 0; i < players_count; ++i) {
    players.emplace_back(static_cast<PlayerColour>(i), i * TOKEN_COUNT);
  }
}

pair<int, int> Board::get_pawn_coordinates(const PlayerColour &colour,
                                           const unsigned int &pos) const {
  return player_path::get_path(colour)[min<int>(pos, Pawn::DEST)];
}

optional<Pawn *> Board::get_pawn_at_coordinates(Player &player,
                                                pair<int, int> cor) {
  for (auto &pawn : player.pawns) {
    if (pawn.is_at_home()) {
      continue;
    }
    if (get_pawn_coordinates(pawn.colour, pawn.pos) == cor)
      return &pawn;
  }
  return nullopt;
}

bool Board::check_clash(const Pawn &pawn) {
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

void Board::set_next_player() {
  _current_player = (_current_player + 1) % players_count;
}

Player &Board::current_player() { return players[_current_player]; }

const Player &Board::current_player() const { return players[_current_player]; }

optional<int> Board::next_wall(PlayerColour colour, const int &pos,
                               const int &steps) {
  auto destination_cor = player_path::get_path(colour)[pos + steps];
  for (int i = pos + 1; i <= min(pos + steps - 1, Pawn::DEST); ++i) {
    pair<int, int> cor = player_path::get_path(colour)[i];
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
        if (pawn.is_protected() && pawn.get_coordinates() == destination_cor) {
          count += 2;
        }
      }
      if (count >= 2) {
        return i;
      }
    }
  }
  return nullopt;
}

bool Board::will_land_on_block(const Pawn &pawn, int steps) {
  auto colour = pawn.colour;
  pair<int, int> cor = player_path::get_path(colour)[pawn.pos + steps];
  for (auto &player : players) {
    if (player.colour == colour) {
      continue;
    }
    int count = 0;
    for (auto &p : player.pawns) {
      count += (p.get_coordinates() == cor);
    }
    if (count >= 2) {
      return true;
    }
  }
  return false;
}

bool Board::move_current(const unsigned int &id, int steps) {
  auto pawn = current_player().get_pawn(id);
  if (steps == 6 && pawn.is_at_home()) {
    steps = 1;
  }
  if (will_land_on_block(pawn, steps)) {
    pawn.go_back_home();
    return false;
  }
  current_player().move(id, steps);
  return check_clash(current_player().get_pawn(id));
}

void Board::move_current_player(const unsigned int &id, int steps) {
  bool clashed = move_current(id, steps);
  if (extra == 3) {
    set_next_player();
    extra = 0;
    return;
  }
  if (steps == 6 || clashed) {
    ++extra;
    return;
  }
  set_next_player();
}

vector<int> Board::get_all_pawns() {
  vector<int> pawns;
  for (auto &pawn : current_player().pawns) {
    pawns.emplace_back(pawn.pos);
  }
  return pawns;
}

vector<int> Board::get_all_pawns(const int &steps) {
  vector<int> pawns;
  for (auto &pawn : current_player().pawns) {
    if (steps != 6 && pawn.is_at_home()) {
      continue;
    }
    if (pawn.pos + steps > Pawn::DEST) {
      continue;
    }
    // remove all pawns that cannot be moved because of a wall exists
    // between range [pawn.pos, steps]
    auto wall = next_wall(current_player().colour, pawn.pos, steps);
    if (wall.has_value()) {
      continue;
    }
    if (!pawn.has_reached_destination()) {
      pawns.emplace_back(pawn.id);
    }
  }
  return pawns;
}

bool Board::ended() const {
  for (auto &player : players) {
    if (player.is_winner())
      return true;
  }
  return false;
}

const Player &Board::winner() const {
  return *find_if(players.begin(), players.end(),
                  [&](auto &p) { return p.is_winner(); });
}

string Board::to_string() {
  string res = "";
  for (auto &x : current_state()) {
    for (auto &y : x) {
      res += y;
    }
    res += '\n';
  }
  return res;
}

array<string, rows> Board::current_state() {
  auto state = grid;
  for (auto &player : players) {
    unsigned int home_idx = 0;
    for (auto &pawn : player.pawns) {
      if (pawn.has_reached_destination()) {
        continue;
      }
      if (pawn.is_at_home()) {
        auto [x, y] = tokens[pawn.colour][home_idx++];
        state[x][y] = pawn.to_char();
      } else {
        auto [x, y] = get_pawn_coordinates(pawn.colour, pawn.pos);
        state[x][y] = pawn.to_char();
      }
    }
  }
  return state;
}

const Player &Board::get_player(const unsigned int &idx) const {
  assert(idx < players.size());
  return players[idx];
}

PlayerColour Board::get_pawn_colour(const int &id) {
  return static_cast<PlayerColour>(id / TOKEN_COUNT);
}
