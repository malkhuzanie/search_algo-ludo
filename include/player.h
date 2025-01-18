#ifndef PLAYER_H
#define PLAYER_H

#include <pawn.h>
#include <player_colour.h>
#include <vector>

using namespace std;

struct Player {
  Player(PlayerColour colour, int starting_idx);

  // returns the pawn index in the player path
  // specified in player_path struct
  unsigned int inline get_pawn_idx(const int &id);

  // get the pawn given its id
  Pawn &get_pawn(const unsigned int &id);

  // move the pawn with id, steps = steps forward
  void move(const int &id, const int &steps);

  // returns the pawn back to the player home position
  void go_back_home(const int &id);

  bool is_winner() const;

  PlayerColour colour;
  vector<Pawn> pawns;
};

#endif
