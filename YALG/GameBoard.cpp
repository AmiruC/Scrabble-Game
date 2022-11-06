#include "GameBoard.h"

GameBoard::GameBoard(std::shared_ptr<Player> player1, std::shared_ptr<Player> player2, std::shared_ptr<TileBag> bag) {
    // new game constructor

    this->players.push_back(player1);
    this->players.push_back(player2);
    this->bag = bag;
    this->board.resize(MAX_BOARD_SIZE, std::vector<std::shared_ptr<Tile>>(MAX_BOARD_SIZE));
    for (int row = 0; row < MAX_BOARD_SIZE; ++row) {
        for (int col = 0; col < MAX_BOARD_SIZE; ++col) {
            this->board[row][col] = nullptr;
        }
    }
    this->turn_tracker = 0;

    // fill player's hands with tiles
    for (int tile = 1; tile <= MAX_HAND_SIZE; ++tile) {
        this->players[0]->getHand()->add(bag->drawTile());
    }

    for (int tile = 1; tile <= MAX_HAND_SIZE; ++tile) {
        this->players[1]->getHand()->add(bag->drawTile());
    }
}


GameBoard::GameBoard(std::shared_ptr<Player> player1, std::shared_ptr<Player> player2, std::shared_ptr<TileBag> bag, std::vector<std::vector<std::shared_ptr<Tile>>> saved_board, bool turn_tracker) {

    this->players.push_back(player1);
    this->players.push_back(player2);
    this->bag = bag;
    this->board = saved_board;
    this->turn_tracker = turn_tracker;

}

GameBoard::~GameBoard() {}

std::string GameBoard::save_game() {
    return board_state_to_string();
}

void GameBoard::print_board() {
    std::cout << board_state_to_string() << std::endl;
}

bool GameBoard::place_tile(std::shared_ptr<Tile> tile, std::string location) {
    // need to implement check for all tiles in same row (per turn) somewhere in main
    int row = int(location[0]) - 'A';
    int col = std::stoi(location.substr(1));

    bool valid = check_valid_place(row, col);
    if (valid) {
        // only place tile and increment score if tile placement is valid (adjacent to existing tiles)
        this->board[row][col] = tile;
        this->increment_current_player_score(tile->value);
    }
    return valid;
}

bool GameBoard::get_turn() {
    return this->turn_tracker;
}

void GameBoard::change_turn() {
    this->turn_tracker = !(this->turn_tracker);
}

bool GameBoard::check_valid_place(int row, int col) {
    // checks if tile placement at current row and col is valid based on existence of adjacent tiles

    bool valid = false;
    if ((row >= 0 && row < MAX_BOARD_SIZE) && (col >=0 && col < MAX_BOARD_SIZE)) {
        if (is_board_empty()) {
            valid = true;
        }
        else {
            if (this->board[row][col] == nullptr) {
                valid = is_adjacent_tile(row, col);
            }
        }
    }
    return valid;
}

bool GameBoard::is_adjacent_tile(int row, int col) {
    // checks if tile placement is adjacent to already placed tile as per scrabble rules

    bool adjacent = false;
    int adjacent_check[4][2] = {{-1,0},{1,0},{0,-1},{0,1}};
    for (int square = 0; square < ADJACENT_SQUARES && adjacent == false; square++) {
        int check_row = row + adjacent_check[square][0];
        int check_col = col + adjacent_check[square][1];
        if (check_row >= 0 && check_row < MAX_BOARD_SIZE && check_col >= 0 && check_col < MAX_BOARD_SIZE) {
            if (this->board[check_row][check_col] != nullptr) {
                adjacent = true;
            }
        }
    }
    return adjacent;
}

bool GameBoard::is_board_empty() {
    // checks if board is empty i.e. first tile placement of the game

    bool empty = true;
    for (int row = 0; row < MAX_BOARD_SIZE && empty == true; ++row) {
        for (int col = 0; col < MAX_BOARD_SIZE && empty == true; ++col) {
            if (this->board[row][col] != nullptr) {
                empty = false;
            }
        }
    }
    return empty;
}

void GameBoard::increment_current_player_score(int value) {
    // increments current player's score by given value
    this->players[turn_tracker]->incScore(value);
}

std::string GameBoard::board_state_to_string() {
    // saves board state to string (to the extent of the furthest tile placed) for use in printing and saving

    std::string board_state;

    // find the minimum dimensions to represent all tiles
    int max_cols = 2;
    int max_rows = 2;
    for (int row = 0; row < MAX_BOARD_SIZE; ++row) {
        for (int col = 0; col < MAX_BOARD_SIZE; ++col) {
            if (this->board[row][col] != nullptr) {
                if (row > max_rows) {
                    max_rows = row;
                }
                if (col > max_cols) {
                    max_cols = col;
                }
            }
        }
    }

    // saves the column numbers across top of board
    board_state += "    ";
    for (int col = 0; col <= max_cols; ++col) {
        board_state += std::to_string(col) + "  ";
        if (col < 10) {
            board_state += " ";
        }
    }
    board_state += "\n";

    // saves the horizontal line across top of board
    board_state += "  -";
    for (int col = 0; col <= max_cols; ++col) {
        board_state += "----";
    }
    board_state += "\n";

    // saves each row of board
    for (int row = 0; row <= max_rows; ++row) {
        board_state += char(row + 'A') + std::string(" |");
        for (int col = 0; col <= max_cols; ++col) {
            if (this->board[row][col] != nullptr) {
                board_state += " ";
                board_state += this->board[row][col]->letter;
                board_state += " |";
            } else {
            board_state += "   |";
            }
        }
        board_state += "\n";
    }

    return board_state;
}