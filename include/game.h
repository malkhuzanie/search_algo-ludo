#ifndef GAME_H
#define GAME_H

#include <board.h>
#include <memory>
#include <player.h>
#include <random>
#include <vector>

using namespace std;

struct Game {
  static const int TOKEN_COUNT = 4;

  Game(int players_count) { board = Board(players_count); }

  void print();

  bool ended() const;

  const Player &winner() const;

  PlayerColour current_player_colour() const;

  string current_player_name() const;

  vector<int> get_all_pawns(const int &steps);

  string get_choices(const vector<int> &pawns);

  void move(int steps);

  void move_current_player(const unsigned int &id, int steps);

  int roll_dice();

  unsigned int players_count();

  unsigned int next_player_id();

  // returns the next player id if the current player
  // id = current
  unsigned int next_player_id(const int &current);

  unique_ptr<Game> clone();

  static int rand(const int &low, const int &high) {
    random_device r;
    static default_random_engine eng{r()};
    return uniform_int_distribution<>{low, high}(eng);
  }

  Board board;
  unsigned int extra = 0;
};

#endif
