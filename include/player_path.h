#ifndef PLAYER_PATH_H
#define PLAYER_PATH_H

#include <array>
#include <player_colour.h>
#include <stdexcept>
#include <vector>

using namespace std;

class player_path {
public:
  static const vector<pair<int, int>> &get_path(const PlayerColour &colour) {
    switch (colour) {
    case PlayerColour::RED:
      return red_path;
    case PlayerColour::BLUE:
      return blue_path;
    case PlayerColour::GREEN:
      return green_path;
    case PlayerColour::YELLOW:
      return yellow_path;
    default:
      throw std::invalid_argument("Invalid color");
    }
  }

private:
  static const vector<pair<int, int>> circular_path;
  static const vector<pair<int, int>> green_path;
  static const vector<pair<int, int>> yellow_path;
  static const vector<pair<int, int>> red_path;
  static const vector<pair<int, int>> blue_path;
};

#endif
