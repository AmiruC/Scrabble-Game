#ifndef ASSIGN_2_PLAYER_H
#define ASSIGN_2_PLAYER_H

#include <iostream>
#include <memory>
#include <vector>
#include "Hand.h"

class Tile;

class Player {
public:
    Player();
    Player(Player& other);
    ~Player();

    void setName(std::string name);
    void setScore(int score);
    void incScore(int score);
    std::string getName();
    int getScore();
    std::shared_ptr<Hand> getHand();

    void track_last_action(bool action);
    bool check_consec_passes();

private:
    std::string name;
    int score;
    std::shared_ptr<Hand> hand;
    std::vector<bool> pass_tracker;
};

#endif