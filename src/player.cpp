#include <board.h>
#include <constants.h>
#include <player.h>

Player::Player(PlayerColour colour, int starting_idx) : colour(colour) {
  pawns.resize(TOKEN_COUNT);
  for (int i = starting_idx; i < starting_idx + 4; ++i) {
    pawns[i % 4] = Pawn(colour, i);
  }
}

Pawn &Player::get_pawn(const unsigned int &id) {
  return pawns[get_pawn_idx(id)];
}

unsigned int inline Player::get_pawn_idx(const int &id) {
  return id % TOKEN_COUNT;
}

void Player::move(const int &id, const int &steps) {
  pawns[get_pawn_idx(id)].move(steps);
}

void Player::go_back_home(const int &id) {
  pawns[get_pawn_idx(id)].go_back_home();
}

bool Player::is_winner() {
  for (auto &pawn : pawns) {
    if (!pawn.has_reached_destination()) {
      return false;
    }
  }
  return true;
}
