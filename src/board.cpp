#include <board.h>
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
        count += 2 * (pawn.is_protected());
      }
      if (count >= 2) {
        return pos + 1;
      }
    }
  }
  return nullopt;
}

bool Board::move_current(const unsigned int &id, int steps) {
  auto pawn = current_player().get_pawn(id);
  if (steps == 6 && pawn.is_at_home()) {
    steps = 1;
  }
  auto wall = next_wall(current_player().colour, pawn.pos, steps);
  if (wall.has_value()) {
    steps = 0;
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

vector<int> Board::get_all_pawns(const int &steps) {
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

pair<int, int> Board::get_pawn_home(PlayerColour colour) {
  auto &home = tokens[colour];
  auto home_pos = home.front();
  rotate(home.begin(), home.begin() + 1, home.end());
  return home_pos;
}

array<string, rows> Board::current_state() {
  auto state = grid;
  for (auto &player : players) {
    for (auto &pawn : player.pawns) {
      if (pawn.has_reached_destination()) {
        continue;
      }
      if (pawn.is_at_home()) {
        auto [x, y] = get_pawn_home(pawn.colour);
        state[x][y] = pawn.to_char();
      } else {
        auto [x, y] = get_pawn_coordinates(pawn.colour, pawn.pos);
        state[x][y] = pawn.to_char();
      }
    }
  }
  return state;
}

PlayerColour Board::get_pawn_colour(const int &id) {
  return static_cast<PlayerColour>(id / TOKEN_COUNT);
}
