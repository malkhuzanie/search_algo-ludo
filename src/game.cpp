#include <ai.h>
#include <board_printer.h>
#include <drawer.h>
#include <expectiminmax.h>
#include <format>
#include <game.h>
#include <utils.h>

void Game::print() {
  LudoBoardPrinter<15>::print(board.current_state());
  // cout << board.to_string() << '\n';
  // drawer::print(board.current_state(), false);
}

bool Game::ended() const { return board.ended(); }

const Player &Game::winner() const { return board.winner(); }

PlayerColour Game::current_player_colour() const {
  return board.current_player().colour;
}

string Game::current_player_name() const {
  switch (board.current_player().colour) {
  case PlayerColour::RED:
    return "Red";
  case PlayerColour::GREEN:
    return "Green";
  case PlayerColour::YELLOW:
    return "Yellow";
  case PlayerColour::BLUE:
    return "Blue";
  }
  return "";
}

vector<int> Game::get_all_pawns(const int &steps) {
  return board.get_all_pawns(steps);
}

string Game::get_choices(const vector<int> &pawns) {
  string choices = "[";
  for (int i = 0; i < pawns.size(); ++i) {
    if (i < pawns.size() - 1) {
      choices += to_string(pawns[i]);
      choices += ", ";
      continue;
    }
    choices += to_string(pawns[i]);
  }
  return choices += "]";
}

bool pawn_exists(vector<int> &indices, const int &id) {
  for (auto &idx : indices) {
    if (idx == id)
      return true;
  }
  return false;
}

void Game::move(int steps) {
  bool clashed = false;
  int id = -1;
  auto pawns = board.get_all_pawns(steps);
  if (pawns.empty()) {
    cout << center(dotted_line("Skipping, None of the pawns can be moved"))
         << '\n';
    return;
  }
  switch (board._current_player) {
  case 0: {
    cout << center(
        dotted_line(format("Choose a piece to move {}:", get_choices(pawns))));
    cout << string(get_padding(""), ' ');
    cin >> id;
    cout << '\n';
    while (!pawn_exists(pawns, id)) {
      cout << center(dotted_line("Invalid pawn number, Please try again."))
           << '\n';
      cin >> id;
    }
    break;
  }
  default:
    vector<ai::Move> path;
    cout << center("Thinking...") << '\n';
    id = ai::choose_pawn(*this, steps, path);
    // id = algo::choose_pawn(*this, steps, path);
    cout << center(dotted_line(
                format("Chosen pawn id: {}", drawer::pawn_symbol(id))))
         << '\n';
    cout << center(dotted_line("Expectiminmax path is:")) << '\n';
    drawer::print_path(path);
  }
  board.move_current_player(id, steps);
}

void Game::move_current_player(const unsigned int &id, int steps) {
  board.move_current_player(id, steps);
}

int Game::roll_dice() { return rand(1, 6); }

unique_ptr<Game> Game::clone() { return make_unique<Game>(*this); }

unsigned int Game::players_count() { return board.players_count; }

unsigned int Game::next_player_id() {
  return (board._current_player + 1) % players_count();
}

unsigned int Game::current_player_idx() { return board._current_player; }

unsigned int Game::next_player_id(const int &current) {
  return (current + 1) % players_count();
}
