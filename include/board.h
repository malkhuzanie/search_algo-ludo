#ifndef BOARD_H
#define BOARD_H

#include <grid.h>
#include <iostream>
#include <map>
#include <optional>
#include <pawn.h>
#include <player.h>
#include <player_colour.h>
#include <set>
#include <vector>

using namespace std;

struct Board {
  Board() {}

  Board(int players_count);

  // returns the pawn coordinates in the grid based on the player path specified
  // int player_path struct
  pair<int, int> get_pawn_coordinates(const PlayerColour &colour,
                                      const unsigned int &pos) const;

  optional<Pawn *> get_pawn_at_coordinates(Player &player, pair<int, int> cor);

  // check for clash whenever a player moves a pawn
  // returns true if there is a clash and false otherwise
  bool check_clash(const Pawn &pawn);

  // switch players turn
  void set_next_player();

  Player &current_player();

  const Player &current_player() const;

  // search for a wall in range [pos + 1, pos + steps], return nullopt
  // if no wall is found
  // a wall exists if 2 pawns are placed on the same cell, or if a cell
  // contains a pawn in the player's protected area.
  optional<int> next_wall(PlayerColour colour, const int &pos,
                          const int &steps);

  bool will_land_on_block(const Pawn &pawn, int steps);

  bool move_current(const unsigned int &id, int steps);

  // move the current player steps = steps forward
  void move_current_player(const unsigned int &id, int steps);

  vector<int> get_all_pawns();

  // get current player pawns the can be moved with the specified steps
  // where steps represents the face value of the rolled dice
  vector<int> get_all_pawns(const int &steps);

  // return true if the game has ended "there is a winner"
  bool ended() const;

  const Player &winner() const;

  string to_string();

  // returns a string representation of the current game state
  array<string, rows> current_state();

  static PlayerColour get_pawn_colour(const int &id);

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
      {PlayerColour::BLUE, {{11, 11}, {12, 11}, {11, 12}, {12, 12}}},
      {PlayerColour::GREEN, {{2, 2}, {3, 2}, {2, 3}, {3, 3}}},
      {PlayerColour::YELLOW, {{2, 11}, {3, 11}, {2, 12}, {3, 12}}}};

  unsigned int extra = 0;
  unsigned int players_count;
  unsigned int _current_player = 0;
  vector<Player> players;
};

#endif
