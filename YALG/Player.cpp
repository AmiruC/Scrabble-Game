#include "Player.h"

Player::Player() {
    name = "undefinedName";
    score = 0;
    hand = std::make_shared<Hand>();
    pass_tracker.push_back(0);
    pass_tracker.push_back(0);
}

Player::Player(Player& other) {
    name = other.name;
    score = other.score;
    hand = std::make_shared<Hand>(*other.hand);
}

Player::~Player() {
}

void Player::setName(std::string name) {
    this->name = name;
}

void Player::setScore(int score) {
    this->score = score;
}
void Player::incScore(int score) {
    this->score += score;
}


std::string Player::getName() {
    return name;
}

int Player::getScore() {
    return score;
}

std::shared_ptr<Hand> Player::getHand() {
    return hand;
}

void Player::track_last_action(bool action) {
    pass_tracker.push_back(action);
    pass_tracker.erase(pass_tracker.begin());
}

bool Player::check_consec_passes() {
    return pass_tracker[0] && pass_tracker[1];
}