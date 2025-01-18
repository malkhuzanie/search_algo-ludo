#include <chrono>
#include <drawer.h>
#include <fstream>
#include <game.h>
#include <iomanip>
#include <pawn.h>
#include <thread>
#include <utils.h>

using namespace std;

const int WIDTH = 140;

void print_intro() {
// Clear screen in a cross-platform way
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif

  // Function for creating typed text effect
  auto typeText = [](const string &text, int delay_ms = 20) {
    for (char c : text) {
      cout << c << flush;
      this_thread::sleep_for(chrono::milliseconds(delay_ms));
    }
    cout << "\n";
  };

  // Using standard ASCII characters for borders
  cout << "\n\n+" << string(WIDTH - 2, '-') << "+\n";

  fstream f{"../intro"};
  if (f.is_open()) {
    cout << f.rdbuf();
  }

  cout << string(WIDTH, '=') << "\n\n";

  // Welcome message with typing effect
  typeText("                           Welcome to the Classic Board Game of "
           "Strategy");
  cout << "\n";

  // Player information box using standard ASCII
  cout << R"(
    +-------------------------------- PLAYERS ----------------------------------+
    |                                                                           |
    |     [R]ed        [B]lue        [G]reen        [Y]ellow                    |
    |     Home: *      Home: *      Home: *       Home: *                       |
    |                                                                           |
    +---------------------------------------------------------------------------+
)" << endl;

  // Game rules box using standard ASCII
  cout << R"(
    +------------------------------- GAME RULES -------------------------------+
    |                                                                          |
    |          * Roll a 6 to release pawns from your home                      |
    |          * Move your pawns clockwise around the board                    |
    |          * Capture opponent's pawns by landing on their space            |
    |          * Get all four pawns to the center to win                       |
    |                                                                          |
    +--------------------------------------------------------------------------+
)" << endl;

  // Press Enter prompt with simple animation
  for (int i = 0; i < 3; i++) {
    cout << "\r                        Press Enter to Begin Your Journey..."
         << flush;
    this_thread::sleep_for(chrono::milliseconds(500));
    cout << "\r                        >>>> Press Enter to Begin <<<<    "
         << flush;
    this_thread::sleep_for(chrono::milliseconds(500));
  }
  cout << '\n';

  cin.get();
}

int main() {
  map<char, string> colour = {
      {'R', "Red"}, {'B', "Blue"}, {'G', "Green"}, {'Y', "Yellow"}};

  print_intro();
  cout << "Choose the number of players [1..4]\n";
  int players_count = 2;
  cin >> players_count;
  cout << "----------------------> Start <-------------------\n";
  auto game = Game(players_count);
  while (!game.ended()) {
    int steps = game.roll_dice();
    cout << drawer::get_colour(game.current_player_colour()) << dot
         << format("{} player turn.\n", game.current_player_name());

    // cout << game.current_player_name() << " player turn. \n";
    cout << dot << format("Dice face value: {}\n", steps);
    game.move(steps);
    print_sep('-', 70);
    cout << drawer::RESET;
    game.print();
  }
  return 0;
}
