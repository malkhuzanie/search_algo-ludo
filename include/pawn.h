#ifndef PAWN_H
#define PAWN_H

enum class PlayerColour;

struct Pawn {
  static const int HOME;
  static const int DEST;

  Pawn() {}

  Pawn(PlayerColour colour, int id);

  void move(const int &steps);

  void go_back_home();

  bool is_at_home() const;

  bool has_reached_destination() const;

  bool is_protected() const;

  char to_char();

  // returns the pawn coordinates in the grid
  std::pair<int, int> get_coordinates();

  int id; // current position ranging from [-1, 57[
  PlayerColour colour;
  int pos;
};

#endif
