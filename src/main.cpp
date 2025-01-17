#include <drawer.h>
#include <game.h>
#include <pawn.h>

using namespace std;

int main() {
  map<char, string> colour = {
      {'R', "Red"}, {'B', "Blue"}, {'G', "Green"}, {'Y', "Yellow"}};

  cout << "----------------------> Start <-------------------\n";
  auto game = Game(2);
  while (!game.ended()) {
    int steps = game.roll_dice();
    cout << drawer::get_colour(game.current_player_colour()) << '\n';
    cout << game.current_player_name() << " player turn. \n";
    cout << format("Dice face value: {}\n", steps);
    game.move(steps);
    cout << "--------------------------------------------------\n";
    cout << drawer::RESET;
    game.print();
  }
  return 0;
}
