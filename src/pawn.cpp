#include <math.h>
#include <pawn.h>
#include <player_path.h>

using std::min;

const int Pawn::HOME = -1;
const int Pawn::DEST = 56;

Pawn::Pawn(PlayerColour colour, int id) : colour(colour), id(id), pos(-1) {}

void Pawn::move(const int &steps) { pos = min(pos + steps, DEST); }

void Pawn::go_back_home() { pos = HOME; }

bool Pawn::is_at_home() const { return pos == HOME; }

bool Pawn::has_reached_destination() const { return pos == DEST; }

bool Pawn::is_protected() const { return pos == 0 || pos >= DEST - 5; }

pair<int, int> Pawn::get_coordinates() {
  return player_path::get_path(colour)[pos];
}

char Pawn::to_char() {
  if (id >= 0 && id <= 9) {
    return '0' + id;
  } else if (id >= 10 && id <= 35) {
    return 'H' + (id - 10);
  }
  return '?';
}
