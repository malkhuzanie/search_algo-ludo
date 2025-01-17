#ifndef DRAWER_H
#define DRAWER_H

#include <board.h>
#include <iostream>
#include <player_colour.h>
#include <vector>

namespace drawer {
// ANSI color codes
const string RESET = "\033[0m";
const string RED = "\033[31m";
const string GREEN = "\033[32m";
const string YELLOW = "\033[33m";
const string BLUE = "\033[34m";
const string RED_BG = "\033[41m";
const string GREEN_BG = "\033[42m";
const string YELLOW_BG = "\033[43m";
const string BLUE_BG = "\033[44m";

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
// void print(const std::vector<std::vector<char>> &grid) {
//   for (size_t i = 0; i < grid.size(); ++i) {
//     for (size_t j = 0; j < grid[i].size(); ++j) {
//       char cell = grid[i][j];
//       switch (cell) {
//       case 'G':
//         std::cout << "\033[42m  \033[0m"; // Green zone
//         break;
//       case 'Y':
//         std::cout << "\033[43m  \033[0m"; // Yellow zone
//         break;
//       case 'R':
//         std::cout << "\033[41m  \033[0m"; // Red zone
//         break;
//       case 'B':
//         if (i > 0 && grid[i - 1][j] == '.') {
//           std::cout << "\033[32m|\033[0m"; // Top border
//         } else if (j > 0 && grid[i][j - 1] == '.') {
//           std::cout << "\033[32m-\033[0m"; // Left border
//         } else {
//           std::cout << "  ";
//         }
//         break;
//       case 'g':
//         std::cout << "\033[32mg\033[0m "; // Green token
//         break;
//       case 'y':
//         std::cout << "\033[33my\033[0m "; // Yellow token
//         break;
//       case 'r':
//         std::cout << "\033[31mr\033[0m "; // Red token
//         break;
//       case 'b':
//         std::cout << "\033[34mb\033[0m "; // Blue token
//         break;
//       case '#':
//         std::cout << "\033[46m# \033[0m"; // Central square
//         break;
//       case '.':
//         std::cout << "  "; // Empty space
//         break;
//       default:
//         std::cout << cell << ' '; // Default case
//       }
//     }
//     std::cout << '\n';
//   }
// }
} // namespace drawer

// void print(const std::vector<std::vector<char>> &grid) {
//   for (int i = 0; i < grid.size(); i++) {
//     cout << i << "  " << RESET;
//   }
//   cout << "\n\n\n";
//   for (int i = 0; i < grid.size(); i++) {
//     // Print top border of each cell
//     for (int j = 0; j < grid[i].size(); j++) {
//       string borderColor = needs_colored_border(grid[i][j])
//                                ? get_border_color(grid[i][j])
//                                : RESET;
//       cout << borderColor << "┌──┐" << RESET;
//     }
//     cout << "\n";
//
//     // Print cell content
//     for (int j = 0; j < grid[i].size(); j++) {
//       string borderColor = needs_colored_border(grid[i][j])
//                                ? get_border_color(grid[i][j])
//                                : RESET;
//       string contentColor = get_colour(grid[i][j]);
//
//       cout << borderColor << "│" << RESET;
//
//       if (is_token(grid[i][j])) {
//         // Color only the token character
//         cout << contentColor << " " << grid[i][j] << RESET;
//       } else if (grid[i][j] == '.') {
//         // Empty cell
//         cout << "  ";
//       } else {
//         // Other cells (including capital letters)
//         cout << (needs_colored_border(grid[i][j]) ? "  " : "  ");
//       }
//
//       cout << borderColor << "│" << RESET;
//     }
//     cout << "\n";
//
//     // Print bottom border of each cell
//     for (int j = 0; j < grid[i].size(); j++) {
//       string borderColor = needs_colored_border(grid[i][j])
//                                ? get_border_color(grid[i][j])
//                                : RESET;
//       cout << borderColor << "└──┘" << RESET;
//     }
//     cout << "\n";
//   }
// }
#endif
