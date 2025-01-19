#include <array>
#include <iomanip>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utils.h>

using namespace std;

template <size_t Rows> class LudoBoardPrinter {
private:
  static constexpr const char *RESET = "\033[0m";
  static const unordered_map<char, const char *> colour_map;

  static void print_horizontal_border(size_t width) {
    cout << string((terminal_width() - 35) >> 1, ' ');
    cout << "   +" << string(width * 2 - 1, '-') << "+\n";
  }

public:
  static bool is_home(char c) {
    return c == 'R' || c == 'G' || c == 'Y' || c == 'B';
  }

  static bool is_border(char c) {
    return c == 'r' || c == 'g' || c == 'y' || c == 'b';
  }

  static void print(const array<string, Rows> &grid) {
    if (grid.empty())
      return;

    const size_t width = grid[0].length();

    cout << string((terminal_width() - 35) >> 1, ' ');
    cout << "    ";
    for (size_t i = 0; i < width; ++i) {
      cout << i % 10 << " ";
    }
    cout << "\n";

    print_horizontal_border(width);

    for (size_t i = 0; i < grid.size(); ++i) {
      cout << string((terminal_width() - 35) >> 1, ' ');
      cout << setw(2) << i << " |";

      for (char c : grid[i]) {
        auto colour_it = colour_map.find(c);
        if (colour_it != colour_map.end()) {
          cout << colour_it->second;
          char to_print;
          if (is_border(c)) {
            to_print = '-';
          } else if (is_home(c)) {
            to_print = '*';
          } else {
            to_print = c;
          }
          cout << to_print << RESET << " ";
        } else {
          cout << c << " ";
        }
      }

      cout << "\b|" << "\n";
    }

    print_horizontal_border(width);
  }
};

template <size_t Rows>
const unordered_map<char, const char *> LudoBoardPrinter<Rows>::colour_map = {
    {'R', "\033[31m"}, // Red
    {'r', "\033[31m"}, // Red
    {'G', "\033[32m"}, // Green
    {'g', "\033[32m"}, // Green
    {'B', "\033[34m"}, // Blue
    {'b', "\033[34m"}, // Blue
    {'Y', "\033[33m"}, // Yellow
    {'y', "\033[33m"}, // Yellow
    {'0', "\033[31m"}, // Red home
    {'1', "\033[31m"}, // Red home
    {'2', "\033[31m"}, // Red home
    {'3', "\033[31m"}, // Red home
    {'4', "\033[33m"}, // Yellow home
    {'5', "\033[33m"}, // Yellow home
    {'6', "\033[33m"}, // Yellow home
    {'7', "\033[33m"}, // Yellow home
    {'8', "\033[34m"}, // Blue home
    {'9', "\033[34m"}, // Blue home
    {'H', "\033[34m"}, // Blue home
    {'I', "\033[34m"}, // Blue home
    {'J', "\033[32m"}, // Green home
    {'K', "\033[32m"}, // Green home
    {'L', "\033[32m"}, // Green home
    {'M', "\033[32m"}, // Green home
};
