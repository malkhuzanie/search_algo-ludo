#include <board.h>
#include <drawer.h>
#include <iomanip>
#include <utils.h>

namespace drawer {
const int MAX_NAME_WIDTH = 6;

string get_colour(PlayerColour colour) {
  switch (colour) {
  case PlayerColour::RED:
    return RED;
  case PlayerColour::GREEN:
    return GREEN;
  case PlayerColour::YELLOW:
    return YELLOW;
  case PlayerColour::BLUE:
    return BLUE;
  default:
    return RESET;
  }
}

string get_name(PlayerColour colour) {
  switch (colour) {
  case PlayerColour::RED:
    return "Red";
  case PlayerColour::GREEN:
    return "Green";
  case PlayerColour::YELLOW:
    return "Yellow";
  case PlayerColour::BLUE:
    return "Blue";
  default:
    return "";
  }
}
// Helper function to get the appropriate color for a cell
string get_colour(char c) {
  switch (c) {
  case 'R':
  case 'r':
    return RED;
  case 'G':
  case 'g':
    return GREEN;
  case 'Y':
  case 'y':
    return YELLOW;
  case 'B':
  case 'b':
    return BLUE;
  default:
    return RESET;
  }
}

// Helper function to get background color for borders
string get_border_color(char c) {
  switch (c) {
  case 'R':
  case '0':
    return RED_BG;
  case 'B':
  case '1':
    return BLUE_BG;
  case 'G':
  case '2':
    return GREEN_BG;
  case 'Y':
  case '3':
    return YELLOW_BG;
  default:
    return RESET;
  }
}

bool is_border(char c) { return c == '0' || c == '1' || c == '2' || c == '3'; }

// Helper function to determine if a character needs colored borders
bool needs_colored_border(char c) {
  return c == 'R' || c == 'G' || c == 'Y' || c == 'B';
}

// Helper function to determine if a character is a token
bool is_token(char c) { return c == 'r' || c == 'g' || c == 'y' || c == 'b'; }

template <std::size_t N> void print(const std::array<string, N> &grid, bool f) {
  for (int i = 0; i < grid.size(); i++) {
    cout << " " << i << (i < 10 ? "  " : " ");
  }
  cout << "\n";
  for (int i = 0; i < 15; i++) {
    // Print top border of each cell
    for (int j = 0; j < grid[i].size(); j++) {
      string borderColor;
      if (is_border(grid[i][j])) {
        // Use numeric border color
        borderColor = get_border_color(grid[i][j]);
      } else if (needs_colored_border(grid[i][j])) {
        // Use letter-based border color
        borderColor = get_border_color(grid[i][j]);
      } else {
        borderColor = RESET;
      }
      cout << borderColor << "┌──┐" << RESET;
    }
    cout << " " << i << "\n";

    // Print cell content
    for (int j = 0; j < grid[i].size(); j++) {
      string borderColor;
      if (is_border(grid[i][j])) {
        // Use numeric border color for filled borders
        borderColor = get_border_color(grid[i][j]);
        cout << borderColor << "│  │" << RESET;
      } else if (needs_colored_border(grid[i][j])) {
        // Use letter-based border color
        borderColor = get_border_color(grid[i][j]);
        cout << borderColor << "│" << RESET << "  " << borderColor << "│"
             << RESET;
      } else if (is_token(grid[i][j])) {
        // Print colored token
        string tokenColor = get_colour(grid[i][j]);
        cout << RESET << "│" << tokenColor << " " << grid[i][j] << RESET << "│";
      } else if (grid[i][j] == '.') {
        // Empty cell
        cout << RESET << "│  │";
      } else {
        // Other cells (like '#')
        cout << RESET << "│" << " " << grid[i][j] << "│";
      }
    }
    cout << "\n";

    // Print bottom border of each cell
    for (int j = 0; j < grid[i].size(); j++) {
      string borderColor;
      if (is_border(grid[i][j])) {
        borderColor = get_border_color(grid[i][j]);
      } else if (needs_colored_border(grid[i][j])) {
        borderColor = get_border_color(grid[i][j]);
      } else {
        borderColor = RESET;
      }
      cout << borderColor << "└──┘" << RESET;
    }
    cout << "\n";
  }
}

void print_path(vector<ai::Move> &path) {
  for (auto &move : path) {
    auto colour = Board::get_pawn_colour(move.pawn_id);
    auto name = get_name(colour);
    cout << get_colour(colour) << setw(6) << "-> " << name << " "
         << "moved pawn" << get_sep('.', MAX_NAME_WIDTH - name.size() + 2)
         << pawn_symbol(move.pawn_id) << ", " << move.steps
         << " steps forward, gaining a score = " << move.score << '\n'
         << RESET;
    // << format(" moved pawn: {}, {} steps forward, gaining a score =
    // {}\n",
    //           pawn_symbol(move.pawn_id), move.steps, move.score)
  }
}

char pawn_symbol(const int &id) {
  if (id >= 0 && id <= 9) {
    return '0' + id;
  } else if (id >= 10 && id <= 35) {
    return 'H' + (id - 10);
  }
  return '?';
}

} // namespace drawer
