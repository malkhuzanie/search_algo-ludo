#include <math.h>
#include <pawn.h>
#include <player_path.h>

using std::min;

const int Pawn::HOME = -1;
const int Pawn::DEST = 56;

Pawn::Pawn(PlayerColour colour, int id) : colour(colour), id(id), pos(-1) {}

void Pawn::move(const int &steps) { pos = min(pos + steps, DEST); }

void Pawn::go_back_home() { pos = HOME; }

bool Pawn::is_at_home() { return pos == HOME; }

bool Pawn::has_reached_destination() { return pos == DEST; }

pair<int, int> Pawn::get_coordinates() {
  return player_path::get_path(colour)[pos];
}
