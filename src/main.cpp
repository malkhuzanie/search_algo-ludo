#include <chrono>
#include <drawer.h>
#include <fstream>
#include <game.h>
#include <intro.h>
#include <iomanip>
#include <pawn.h>
#include <thread>
#include <utils.h>

using namespace std;

const int WIDTH = 140;

inline string player_name(const Game &game) {
  return game.current_player_name();
}

int main() {
  // game intro
  print_intro();

  cout << center("Choose the number of players [1..4]\n");
  int players_count = 2;
  cin >> players_count;

  auto game = Game(players_count);
  cout << center(string(terminal_width() >> 1, '-')) << '\n';
  while (!game.ended()) {
    int steps = game.roll_dice();

    cout << drawer::get_colour(game.current_player_colour())
         << center(format("{} {} player turn.", dot, player_name(game)))
         << '\n';
    cout << center(format("{} Dice face value: {}", dot, steps)) << '\n';

    game.move(steps);

    cout << center(string(terminal_width() >> 1, '-')) << drawer::RESET << '\n';

    game.print();
  }

  cout << drawer::RESET;

  auto winner = game.winner();
  cout << drawer::get_colour(winner.colour);
  cout << center(string(terminal_width(), '-')) << '\n';
  cout << center(format("-----> {} player is the winner. <-----",
                        drawer::get_name(winner.colour)))
       << '\n';
  cout << center(string(terminal_width(), '-')) << drawer::RESET << '\n';

  return 0;
}
