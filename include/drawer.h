#ifndef DRAWER_H
#define DRAWER_H

#include <ai.h>
#include <board.h>
#include <iostream>
#include <player_colour.h>
#include <vector>

const string dot = "⚫";

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

string get_colour(PlayerColour colour);

string get_name(PlayerColour colour);

string get_colour(char c);

string get_border_color(char c);

char pawn_symbol(const int &id);

bool is_border(char c);

bool needs_colored_border(char c);

bool is_token(char c);

template <std::size_t N> void print(const std::array<string, N> &grid, bool f);
void print_path(vector<ai::Move> &path);

} // namespace drawer
#endif
