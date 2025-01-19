#include <chrono>
#include <drawer.h>
#include <fstream>
#include <iomanip>
#include <thread>
#include <utils.h>

using namespace std;
const vector<string> RULES = {"+------------------------------- GAME RULES "
                              "-------------------------------+",
                              "|                                               "
                              "                           |",
                              "|          * Roll a 6 to release pawns from "
                              "your home                      |",
                              "|          * Move your pawns clockwise around "
                              "the board                    |",
                              "|          * Capture opponent's pawns by "
                              "landing on their space            |",
                              "|          * Get all four pawns to the center "
                              "to win                       |",
                              "|                                               "
                              "                           |",
                              "+-----------------------------------------------"
                              "---------------------------+"};

void print_intro() {
// Clear screen in a cross-platform way
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif

  auto type_text = [](const string &text, int delay_ms = 20) {
    cout << string(get_padding(text), ' ');
    for (char c : text) {
      cout << c << flush;
      this_thread::sleep_for(chrono::milliseconds(delay_ms));
    }
    cout << "\n";
  };

  cout << "\n\n+" << string(terminal_width() - 2, '-') << "+\n";

  fstream f{"../intro"};
  string line;
  while (getline(f, line)) {
    cout << drawer::BLUE << center(line) << drawer::RESET << '\n';
  }

  cout << string(terminal_width(), '=') << "\n\n";

  type_text("Welcome to the Classic Board Game of Strategy");

  cout << "\n";

  constexpr auto rules =
      R"(
    +------------------------------- GAME RULES -------------------------------+
    |                                                                          |
    |          * Roll a 6 to release pawns from your home                      |
    |          * Move your pawns clockwise around the board                    |
    |          * Capture opponent's pawns by landing on their space            |
    |          * Get all four pawns to the center to win                       |
    |                                                                          |
    +--------------------------------------------------------------------------+
)";
  // cout << center(rules.data()) << '\n';
  for (auto &r : RULES) {
    cout << center(r) << '\n';
  }

  cout << center("Press Enter to Begin Your Journey...") << '\n';
  cout << center(">>>> Press Enter to Begin <<<<") << '\n';

  cin.get();
}
