#ifndef GAME_H
#define GAME_H

#include <board.h>
#include <board_printer.h>
#include <format>
#include <map>
#include <player.h>
#include <random>
#include <vector>

using namespace std;

struct Game {
  static const int TOKEN_COUNT = 4;

  Game(int players_count) : players_count(players_count) {
    board = Board(players_count);
  }

  void print();

  bool ended();

  Player &winner();

  PlayerColour current_player_colour();

  string current_player_name();

  void print_movable_pieces();

  void print_choices(const vector<int> &pawns);

  void move(int steps);

  int roll_dice();

  static int rand(const int &low, const int &high) {
    std::random_device r;
    static std::default_random_engine eng{r()};
    return std::uniform_int_distribution<>{low, high}(eng);
  }

  Board board;
  unsigned int players_count;
  unsigned int extra = 0;
};

#endif
