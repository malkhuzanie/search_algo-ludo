#ifndef PLAYER_H
#define PLAYER_H

#include <cassert>
#include <constants.h>
#include <iostream>
#include <pawn.h>
#include <player_colour.h>
#include <player_path.h>
#include <vector>

using namespace std;

struct Player {
  Player(PlayerColour colour, int starting_idx);

  unsigned int inline get_pawn_idx(const int &id);

  Pawn &get_pawn(const unsigned int &id);

  void move(const int &id, const int &steps);

  void go_back_home(const int &id);

  bool is_winner();

  PlayerColour colour;
  vector<Pawn> pawns;
};

#endif
