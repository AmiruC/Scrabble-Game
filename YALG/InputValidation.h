#ifndef INPUT_VALIDATION
#define INPUT_VALIDATION

#include <istream>
#include <vector>
#include <memory>
#include <string>
#include <iostream>

#include "GameBoard.h"

#define PLAYER_1 0
#define PLAYER_2 1
#define ROW 0
#define COL 1
#define FIRST_ACTION 0
#define PLACING_ACTION 1
#define REPLACING_ACTION 2

// file containing all methods for input/option validation

bool checkGameOver(std::shared_ptr<GameBoard> game, bool& quit);
std::string validate_menu_option(std::istream& input);
bool validatePlaceRow(std::vector<std::string> placed_locations, std::string input_place);
std::vector<std::string> validateInputFormat(std::istream& input, int action);
void convertInput(std::string first_word, std::vector<std::string>& arguments, std::istream& input);
bool validatePlaceTileFormat(const std::vector<std::string>& arguments);
std::string validateName(std::istream& input, std::string existing_name);
std::string validateLoad(std::istream& input);
void quitGame();

#endif