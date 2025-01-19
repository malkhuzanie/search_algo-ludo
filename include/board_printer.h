#include <array>
#include <iomanip>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utils.h>

template <size_t Rows> class LudoBoardPrinter {
private:
  static constexpr const char *RESET = "\033[0m";
  static const std::unordered_map<char, const char *> colorMap;

  static void printHorizontalBorder(size_t width) {
    std::cout << string((terminal_width() - 35) >> 1, ' ');
    std::cout << "   +" << std::string(width * 2 - 1, '-') << "+\n";
  }

public:
  static bool is_home(char c) {
    return c == 'R' || c == 'G' || c == 'Y' || c == 'B';
  }

  static bool is_border(char c) {
    return c == 'r' || c == 'g' || c == 'y' || c == 'b';
  }

  static void print(const std::array<std::string, Rows> &grid) {
    if (grid.empty())
      return;

    const size_t width = grid[0].length();

    // Print column numbers with spacing
    std::cout << string((terminal_width() - 35) >> 1, ' ');
    std::cout << "    ";
    for (size_t i = 0; i < width; ++i) {
      std::cout << i % 10 << " "; // Add space after each number
    }
    std::cout << "\n";

    printHorizontalBorder(width);

    // Print rows with row numbers
    for (size_t i = 0; i < grid.size(); ++i) {
      std::cout << string((terminal_width() - 35) >> 1, ' ');
      std::cout << std::setw(2) << i << " |";

      // Print each cell with appropriate color and spacing
      for (char c : grid[i]) {
        auto colorIt = colorMap.find(c);
        if (colorIt != colorMap.end()) {
          std::cout << colorIt->second;
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
          std::cout << c << " ";
        }
      }

      std::cout << "\b|" << "\n"; // Remove the last space before closing border
    }

    printHorizontalBorder(width);
  }
};

template <size_t Rows>
const std::unordered_map<char, const char *> LudoBoardPrinter<Rows>::colorMap =
    {
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
