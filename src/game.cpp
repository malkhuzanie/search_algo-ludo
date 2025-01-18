#include <ai.h>
#include <board_printer.h>
#include <drawer.h>
#include <format>
#include <game.h>

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
    return "Yello";
  case PlayerColour::BLUE:
    return "Blue";
  }
  return "";
}

vector<int> Game::get_all_pawns(const int &steps) {
  return board.get_all_pawns(steps);
}

void Game::print_choices(const vector<int> &pawns) {
  for (int i = 0; i < pawns.size(); ++i) {
    if (i < pawns.size() - 1) {
      cout << format("{}, ", pawns[i]);
      continue;
    }
    cout << format("{}", pawns[i]);
  }
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
    cout << dot << "None of the pawns can be moved, Sorry\n";
    return;
  }
  switch (board._current_player) {
  case 0: {
    cout << dot << "Choose a piece to move [";
    print_choices(pawns);
    cout << "]: ";
    cin >> id;
    while (!pawn_exists(pawns, id)) {
      cout << dot << "Invalid pawn number, Please try again.\n";
      cin >> id;
    }
    break;
  }
  default:
    vector<ai::Move> path;
    id = ai::choose_pawn(*this, steps, path);
    cout << dot << "Chosen pawn id: " << drawer::pawn_symbol(id) << '\n';
    cout << dot << "Expectiminmax path is:\n";
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

unsigned int Game::next_player_id(const int &current) {
  return (current + 1) % players_count();
}
