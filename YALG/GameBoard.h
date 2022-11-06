#ifndef ASSIGN_2_GAMEBOARD_H
#define ASSIGN_2_GAMEBOARD_H

#include <vector>
#include <string>
#include <memory>
#include <iostream>
#include "Tile.h"
#include "TileBag.h"
#include "Player.h"

#define MAX_BOARD_SIZE 15
#define ADJACENT_SQUARES 4

class GameBoard {
public:
    std::vector<std::shared_ptr<Player>> players;
    std::shared_ptr<TileBag> bag;

    GameBoard(std::shared_ptr<Player> player1, std::shared_ptr<Player> player2, std::shared_ptr<TileBag> bag);
    GameBoard(std::shared_ptr<Player> player1, std::shared_ptr<Player> player2, std::shared_ptr<TileBag> bag, std::vector<std::vector<std::shared_ptr<Tile>>> loadedBoard, bool turn_tracker);
    ~GameBoard();

    std::string save_game();

    void print_board();
    bool place_tile(std::shared_ptr<Tile> tile, std::string location);

    bool get_turn();
    void change_turn();

private:
    std::vector<std::vector<std::shared_ptr<Tile>>> board;
    bool turn_tracker;

    bool check_valid_place(int row, int col);
    bool is_adjacent_tile(int row, int col);
    bool is_board_empty();
    void increment_current_player_score(int value);
    std::string board_state_to_string();
};

#endif