#include "InputValidation.h"

std::string validate_menu_option(std::istream& input) {
   // validates input for menu selection

   std::string option;
   bool valid = false;

   // while no eof character in input stream and valid selection yet to be made
   while (!(input.eof()) && valid == false) {
      std::cout << ">";
      std::getline(input, option);

      if (option == "1") {
         std::cout<<"\nStarting a New Game\n"<<std::endl;
         valid = true;
      }
      else if (option == "2") {
         std::cout<<"\nEnter the file name from which you wish to load a game"<<std::endl;
         valid = true;
      }
      else if (option == "3") {
         valid = true;
      }
      else if (option == "4") {
         valid = true;
      }
      else {
         std::cout << "Invalid Input. Try again." << std::endl;
      }
      if (input.eof()) {
         quitGame();
      }
   }

   return option;
}

bool checkGameOver(std::shared_ptr<GameBoard> game, bool& quit) {
   // checks if the current game state should result in a game over or quit game

   bool retVal = false;
   if (quit == true) {
      retVal = true;
   }
   else {
      if (game->bag->checkEmpty()) {
         if ((game->players[PLAYER_1]->getHand()->size() == 0) || 
             (game->players[PLAYER_2]->getHand()->size() == 0)) {
         retVal = true;
         }
         else if ((game->players[PLAYER_1]->check_consec_passes()) || 
                  (game->players[PLAYER_2]->check_consec_passes())) {
         retVal = true;
         }
      }
   }
   return retVal;
}

bool validatePlaceRow(std::vector<std::string> placed_locations, std::string input_place) {
   // check that the location of tile to be placed conforms with row/col restriction of already placed tiles
   // placement of first two tiles dictate the exact row/col further placements are locked into

   bool valid = false;
   char input_row = input_place[ROW];
   int input_col = std::stoi(input_place.substr(1));

   // if first placement, no restriction
   if (placed_locations.empty()) {
      valid = true;
   }

   // if second placement, must conform with row or column of first placement
   else if (placed_locations.size() == 1) {
      if ((input_row == placed_locations[0][ROW]) || (input_col == std::stoi(placed_locations[0].substr(1)))) {
         valid = true;
      }
   }

   // for all other placements, row or column with first two tiles must match
   else {
      if (((input_row == placed_locations[0][ROW]) && (input_row == placed_locations[1][ROW])) ||
          ((input_col == std::stoi(placed_locations[1].substr(1))) && (input_col == std::stoi(placed_locations[1].substr(1))))) {
             valid = true;
          }
   }
   return valid;
}

std::vector<std::string> validateInputFormat(std::istream& input, int action) {
   /* 
      checks that input is in correct format. does not check for legal moves. returns a vector of valid input arguments
      action variable determines what action player is in the midst of, and restricts input to that action (so a player
      cannot pass/replace while he is in the midst of placing tiles, for example)
   */

   std::string check_word;
   std::vector<std::string> arguments;
   bool valid_input = false;

   while (!(input.eof()) && valid_input == false) {
      // will continue prompting player for valid input unless EOF character is given

      arguments.clear();
      std::cout << ">";
      input >> check_word;

      if ((action == PLACING_ACTION) || (action == FIRST_ACTION && check_word == "place")) {

         // create string vector of all successive arguments
         convertInput(check_word, arguments, input);

         // checks that entire input obeys correct formatting rules
         valid_input = validatePlaceTileFormat(arguments);
      }
      else if ((action == REPLACING_ACTION) || (action == FIRST_ACTION && check_word == "replace")) {

         convertInput(check_word, arguments, input);

         if (arguments.size() == 2) {
            if (arguments[1].length() == 1) {
               valid_input = true;
            }
         }
      }
      else if (check_word == "pass") {
         arguments.push_back(check_word);
         valid_input = true;
      }
      else if (check_word == "save") {

         convertInput(check_word, arguments, input);
         
         if (arguments.size() == 2) {
            valid_input = true;
         }
      }
      else if (check_word == "quit") {
         arguments.push_back(check_word);
         valid_input = true;
      }
      if (input.eof()) {
         quitGame();
      }
      if (valid_input == false) {
         if (arguments.size() > 0) {
            std::cout << "Invalid input. Try again." << std::endl;
         }
         else {
            std::cout << "Invalid input. Try again." << std::endl;
            input.ignore(256, '\n');
         }
      }
   }
   return arguments;
}

void convertInput(std::string word, std::vector<std::string>& arguments, std::istream& input) {
   // converts whole input to string vector

   std::string line;
   arguments.push_back(word);
   std::getline(input, line);
   std::istringstream iss(line);
   while (iss >> word) {
      arguments.push_back(word);
   }
}

bool validatePlaceTileFormat(const std::vector<std::string>& arguments) {
   // returns whether arguments given comprise a validly formatted 'place' action

   bool retVal = false;

   if (arguments.size() == 4) {
      if (arguments[0] == "place") {
         if (arguments[1].length() == 1) {
            if (arguments[2] == "at") {
               if (arguments[3].length() == 2 || arguments[3].length() == 3) {
                  retVal = true;
               }
            }
         }
      }
   }
   else if (arguments.size() == 2) {
      if (arguments[0] == "place") {
         if (arguments[1] == "Done") {
            retVal = true;
         }
      }
   }

   return retVal;
}

std::string validateName(std::istream& input, std::string existing_name) {
   // return a valid name (uppercase letters only, and not already entered), prompt user until valid name is entered

   std::string name;
   bool valid_name = false;
   while (!(input.eof()) && valid_name == false) {
      std::cout << ">";
      std::getline(input, name);

      bool illegal_name = false;
      for (char& c : name) {
         if (c > 'Z' || c < 'A') {
            illegal_name = true;
         }
      }
      if (illegal_name == false && name != existing_name) {
         valid_name = true;
      }
      else {
         std::cout << "Invalid name. Try again." << std::endl;
      }
      if (input.eof()) {
         quitGame();
      }
   }
   return name;
}

std::string validateLoad(std::istream& input) {
   // checks for eof before passing file name to load_game()

   std::string load_file;
   bool valid = false;
   while (!(input.eof()) && valid == false) {
      std::cout << ">";
      std::getline(input, load_file);

      if (input.eof()) {
         quitGame();
      }
      else {
         valid = true;
      }
   }
   return load_file;
}