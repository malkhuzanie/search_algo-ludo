#include <ai.h>
#include <game.h>

void Game::print() {
  LudoBoardPrinter<15>::print(board.current_state());
  // cout << board.to_string() << '\n';
  // drawer::print(board.current_state(), false);
}

bool Game::ended() { return board.ended(); }

Player &Game::winner() { return board.winner(); }

PlayerColour Game::current_player_colour() {
  return board.current_player().colour;
}

string Game::current_player_name() {
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
}

void Game::print_movable_pieces() {
  auto pawns = board.current_player().pawns;
  for (int i = 0; i < pawns.size(); ++i) {
    cout << format("{:>4} \n", pawns[i].id);
  }
}

void Game::print_choices(const vector<int> &pawns) {
  for (int i = 0; i < pawns.size(); ++i) {
    cout << format("{:>4} \n", pawns[i]);
  }
}

void Game::move(int steps) {
  bool clashed = false;
  switch (steps) {
  case 6:
    int id;
    switch (board._current_player) {
    case 0: {
      cout << "Choose a piece to move\n";
      print_choices(board.get_all_pawns(steps));
      cin >> id;
      break;
    }
    default:
      id = ai::choose_pawn(*this, steps);
      cout << "chosen id: " << id << '\n';
    }
    clashed = board.move_current(id, steps);
    break;
  default:
    auto pawns = board.get_all_pawns(steps);
    if (pawns.empty()) {
      cout << "no movable pieces \n";
      break;
    }
    switch (board._current_player) {
    case 0: {
      cout << "Choose a piece to move\n";
      print_choices(pawns);
      cin >> id;
    }
    default:
      id = ai::choose_pawn(*this, steps);
    }
    clashed = board.move_current(id, steps);
    break;
  }
  if (steps != 6 || clashed || extra == 3) {
    board.set_next_player();
    extra == 0;
  }
  ++extra;
}

int Game::roll_dice() { return rand(1, 6); }
