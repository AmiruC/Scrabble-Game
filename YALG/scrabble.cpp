#include "LinkedList.h"
#include "GameBoard.h"
#include "Player.h"
#include "Hand.h"
#include "TileBag.h"
#include "InputValidation.h"
#include "QuitGameExcept.h"

#include <sstream>
#include <fstream>
#include <memory>
#include <iostream>
#include <exception>

#define EXIT_SUCCESS    0
#define NON_PASS_ACTION 0
#define PASS_ACTION 1
#define NO_EXISTING_NAME ""
#define PLAYER_1_NAME 0
#define PLAYER_2_NAME 3
#define PLAYER_1_SCORE 1
#define PLAYER_2_SCORE 4
#define PLAYER_1_HAND 2
#define PLAYER_2_HAND 5
#define PLAYER_1_TILE 2
#define PLAYER_2_TILE 5
#define BOARD_START_LINE 7
#define BOARD_START_LINE_INDEX 8
#define TILES_INTERVAL 4
#define TILE_BAG_INTERVAL 4
#define HAND_INTERVEL 2
#define BOARD_END_LOCATOR 2

void optionAction(std::string option);
void playGame(std::shared_ptr<GameBoard> game);
void newGame();
void saveGame(std::shared_ptr<GameBoard> game, std::string fileName);
void loadGame(std::string input);
void quitGame();
void credits();
void printMenu();

int main(void) {

   int main_return = 1;

   std::cout << "Welcome to Scrabble!" << std::endl;
   std::cout << "-------------------" << std::endl;

   // print menu and begin execution!
   try {
      printMenu();
   }
   // custom exception to exit program
   catch (const QuitGameExcept& e) {
      main_return = EXIT_SUCCESS;
   }

   return main_return;
}

//Print Menu Method
void printMenu(){ 
   std::cout << "\nMenu" << std::endl;
   std::cout << "----" << std::endl;
   std::cout << "1. New Game" << std::endl;
   std::cout << "2. Load Game" << std::endl;
    std::cout << "3. Credits (Show student information)" << std::endl;
   std::cout << "4. Quit" << std::endl;
   std::cout << std::endl;

   std::string opt = validate_menu_option(std::cin);

   optionAction(opt);
}

//Allocates appropriate action to chosen option
void optionAction(std::string option){
   if(option == "1"){
      newGame();
   }
   if(option == "2"){
      loadGame(validateLoad(std::cin));
   }
   if(option == "3"){
      credits();
   }
   if(option == "4"){
      quitGame();
   }
}

void playGame(std::shared_ptr<GameBoard> game) {
   // initiates game play from a given game board

   bool quit = false;

   // if save game is selected, this variable will hold the filename
   std::string fileName;

   while (!(checkGameOver(game, quit))) {
      // continue play until quit is selected, or tile bag is empty and one player has passed twice or has an empty hand

      // player and hand of current player taking their turn
      std::shared_ptr<Player> current_player = game->players[game->get_turn()];
      std::shared_ptr<Hand> current_player_hand = game->players[game->get_turn()]->getHand();

      // print out all the current game details
      std::cout << std::endl;
      std::cout << current_player->getName() << ", it's your turn" << std::endl;
      std::cout << "Score for " << game->players[PLAYER_1]->getName() << ": " << game->players[PLAYER_1]->getScore() << std::endl;
      std::cout << "Score for " << game->players[PLAYER_2]->getName() << ": " << game->players[PLAYER_2]->getScore() << std::endl;
      game->print_board();
      std::cout << "Your hand is" << std::endl;
      current_player_hand->printHand();
      std::cout << std::endl;

      // takes player input and verifies it's in the correct format
      std::vector<std::string> choice = validateInputFormat(std::cin, FIRST_ACTION);

      // allow player to save before undertaking turn action
      while (choice[0] == "save") {
         saveGame(game, choice[1]);
         choice = validateInputFormat(std::cin, FIRST_ACTION);
      }

      // determine action chosen and begin branching through possible outcomes
      if (choice[0] == "place") {
         // if player wants to place tiles

         // track the location of every placed tile this turn to enforce row placement rules
         std::vector<std::string> placed_locations;

         // check if first action is place Done
         bool done = false;
         if (choice[1] == "Done") {
            done = true;
         }

         // if not, loop through tile placement
         while (done == false) {
            
            // retrieve tile and location from input
            char input_tile = choice[1].at(0);
            std::string input_location = choice[3];
         
            // check if input tile exists in player hand
            if (!(current_player_hand->searchTile(input_tile) == HAND_IS_EMPTY || 
                 current_player_hand->searchTile(input_tile) == INDEX_NOT_FOUND)) {
               std::shared_ptr<Tile> tile_to_place = current_player_hand->get(input_tile);

               // check if input location is follows all placement rules
               if (validatePlaceRow(placed_locations, input_location) && game->place_tile(tile_to_place, input_location)) {
                  current_player_hand->remove(choice[1].at(0));
                  placed_locations.push_back(input_location);
               }
               else {
                  std::cout << "Location not valid. Try again." << std::endl;
               }

               // check if player has bingo'ed this turn
               if (current_player_hand->size() == 0) {
                  std::cout << "BINGO!" << std::endl;
                  current_player->incScore(50);
                  done = true;
               }
            }
            else {
               std::cout << "Tile not found in hand. Try again." << std::endl;
            }

            // continue placing tiles until player specifies Done
            if (done == false) {
               choice = validateInputFormat(std::cin, PLACING_ACTION);
               if (choice[1] == "Done") {
                  done = true;
               }
            }
         }

         // replenish tiles missing from player hand
         bool replenish = true;
         while (replenish) {
            replenish = (!(game->bag->checkEmpty()) && current_player_hand->add(game->bag->drawTile()));
         }

         // if tile bag is empty, begin tracking consecutive passes for game over condition
         if (game->bag->checkEmpty()) {
            current_player->track_last_action(NON_PASS_ACTION);
         }

         // print the new updated board state and change turn
         std::cout << "\n";
         game->print_board();
         game->change_turn();
      }
      else if (choice[0] == "replace") {
         // if player wants to replace a tile

         // check if input tile exists in player hand and remove if so
         while (!(current_player_hand->remove(choice[1].at(0)))) {
            std::cout << "Invalid replace. Try again." << std::endl;
            choice = validateInputFormat(std::cin, REPLACING_ACTION);
         }

         // add replaced tile back to the bag
         game->bag->addTile(choice[1].at(0));

         // draw a new tile to replace removed tile
         current_player_hand->add(game->bag->drawTile());

         // if tile bag is empty, begin tracking consecutive passes for game over condition
         if (game->bag->checkEmpty()) {
            current_player->track_last_action(NON_PASS_ACTION);
         }

         // change turn
         game->change_turn();
      }
      else if (choice[0] == "pass") {
         // if player wants to pass his turn
         game->change_turn();

         // if tile bag is empty, begin tracking consecutive passes for game over condition
         if (game->bag->checkEmpty()) {
            current_player->track_last_action(PASS_ACTION);
         }
      }
      else if (choice[0] == "quit") {
         // if player wants to quit game
         quit = true;
      }
      else {
         quitGame();
      }
   }
   if (quit == true) {
      quitGame();
   }
   else {
      // print game results and exit
      std::cout << "\nGAME OVER" << std::endl;
      std::cout << "Score for " << game->players[PLAYER_1]->getName() << ": " << game->players[PLAYER_1]->getScore() << std::endl;
      std::cout << "Score for " << game->players[PLAYER_2]->getName() << ": " << game->players[PLAYER_2]->getScore() << std::endl;
      if (game->players[PLAYER_2]->getScore() > game->players[PLAYER_1]->getScore()) {
         std::cout << "Player " << game->players[PLAYER_2]->getName() << " won!" << std::endl;
      }
      else if (game->players[PLAYER_1]->getScore() > game->players[PLAYER_2]->getScore()) {
         std::cout << "Player " << game->players[PLAYER_1]->getName() << " won!" << std::endl;
      }
      else {
         std::cout << "Game ended in a tie!" << std::endl;
      }
      std::cout << std::endl;
      std::cout << "Goodbye" << std::endl;
      throw QuitGameExcept();
   }
}

void newGame() {
   // creates a new game board and initiates game play

   std::string player1Name;
   std::string player2Name;

   // validate player name entry
   std::cout << "Enter a name for player 1 (uppercase characters only)" << std::endl;
   player1Name = validateName(std::cin, NO_EXISTING_NAME);
   std::cout << "\nEnter a name for player 2 (uppercase characters only)" << std::endl;
   player2Name = validateName(std::cin, player1Name);

   // create each player
   std::shared_ptr<Player> player_1 = std::make_shared<Player>();
   player_1->setName(player1Name);
   std::shared_ptr<Player> player_2 = std::make_shared<Player>();
   player_2->setName(player2Name);

   // create randomised tile bag
   std::shared_ptr<TileBag> tile_bag = std::make_shared<TileBag>();

   // create new game board from players and tile bag
   std::shared_ptr<GameBoard> game = std::make_shared<GameBoard>(player_1, player_2, tile_bag);

   // initiate game
   std::cout << "\nLet's Play!" << std::endl;
   playGame(game);
}

void saveGame(std::shared_ptr<GameBoard> game, std::string saveFile) {
   //Saving Player 1 Information
   std::ofstream outFile;
   outFile.open(saveFile);
   outFile << game->players[PLAYER_1]->getName() << std::endl;
   outFile << game->players[PLAYER_1]->getScore() << std::endl;
   std::shared_ptr<Hand> handSave = game->players[PLAYER_1]->getHand();
   std::string p1Hand = handSave->saveHand();
   outFile << p1Hand;

   //Saving Player 2 Information
   outFile << game->players[PLAYER_2]->getName() << std::endl;
   outFile << game->players[PLAYER_2]->getScore() << std::endl;
   std::shared_ptr<Hand> handSave2 = game->players[PLAYER_2]->getHand();
   std::string p2Hand = handSave2->saveHand();
   outFile << p2Hand;

   //Saving Board Information
   std::string board = game->save_game();
   outFile << board;

   //Saving Tile Bag Contents
   std::string bagTiles = game->bag->saveTileBag();
   outFile << bagTiles;

   //Saving Turn Tracker
   outFile << game->players[game->get_turn()]->getName();
   outFile.close();
   std::cout << "Game Saved Successfully" << std::endl;
}

void loadGame(std::string fileName) {
   // Create objects
   std::shared_ptr<Player> player1 = std::make_shared<Player>();
   std::shared_ptr<Player> player2 = std::make_shared<Player>();
   std::shared_ptr<TileBag> bag = std::make_shared<TileBag>();
   bool turn_tracker = 0;
   bool checkHand = true;
   // Read file into a string vector
   std::string line;
   std::ifstream inFile;
   inFile.open(fileName);
   std::vector<std::string> loadedGame{};
   if (inFile.is_open()){
      std::cout << "Scrabble game successfully loaded" << std::endl;
      while (std::getline(inFile, line)){
         loadedGame.push_back(line);
      }
   }
   else{
      std::cout << "Failed to open saved game" << std::endl;
      optionAction("2");
   }

   // Check Load File Format (no empty lines execpt for tile bag)
   for (int i = 0; i < int(loadedGame.size() - BOARD_END_LOCATOR); ++i){
      if (loadedGame[i].empty()){
         std::cout << "Invalid Format of Load File. Please Try Again" << std::endl;
         optionAction("2");
      };
   }
   if (loadedGame[loadedGame.size() - 1].empty()){
      std::cout << "Invalid Format of Load File. Please Try Again" << std::endl;
      optionAction("2");
   }

   // From SaveFile.cpp we know that for each line
   // 1: player1Name;
   if (std::all_of(loadedGame[PLAYER_1_NAME].begin(), loadedGame[PLAYER_1_NAME].end(), [](unsigned char c){ return std::isupper(c); })){
      player1->setName(loadedGame[PLAYER_1_NAME]);
   }else{
      std::cout << "Invalid Format of Load File: PLAYER NAME. Please Try Again" << std::endl;
      optionAction("2");
   }

   // 2: player1Score;
   for (int i = 0; i < int(loadedGame[PLAYER_1_SCORE].length()); ++i){
      if (!(isdigit(loadedGame[PLAYER_1_SCORE][i])) || (loadedGame[PLAYER_1_SCORE][i] == ' ')){
         std::cout << "Invalid Format of Load File: SCORE. Please Try Again" << std::endl;
         optionAction("2");
         }
      }
   player1->setScore(std::stoi(loadedGame[PLAYER_1_SCORE]));


   // 3: player1Tiles;
   std::istringstream ss(loadedGame[PLAYER_1_TILE]);
   std::vector<std::string> tilesToLoad;
   std::string tileValuePair;
   while (ss >> tileValuePair){
      tilesToLoad.push_back(tileValuePair);
   }
   for (std::string tmp: tilesToLoad){
      // check tmp size
      if (!((tmp.size() <= 5) && (tmp.size() >= 3))) {
         std::cout << "Invalid Format of Load File: TILE BAG. Please Try Again" << std::endl;
         optionAction("2");
      }
      // check hand tiles letter and '-'
      if (!(isupper(tmp[0]) && tmp[1] == '-')) {
         std::cout << "Invalid Format of Load File: TILE BAG. Please Try Again" << std::endl;
         optionAction("2");
      }

      char letterToLoad = tmp[0];
      int valueToLoad = std::stoi(tmp.substr(2, 2));
      checkHand = player1->getHand()->add(letterToLoad, valueToLoad);
      if (checkHand == 0){
         std::cout << "Load failed: hand size reaches out of bound" << std::endl;
         std::cout << "Invalid Format of Load File. Please try again" << std::endl;
         optionAction("2");
      }
   }

   // 4: player2Name;
   if (std::all_of(loadedGame[PLAYER_2_NAME].begin(), loadedGame[PLAYER_2_NAME].end(), [](unsigned char c){ return std::isupper(c); })){
      player2->setName(loadedGame[PLAYER_2_NAME]);
   }else{
      std::cout << "Invalid Format of Load File: PLAYER NAME. Please try again" << std::endl;
      optionAction("2");
   }

   // 5: player2Score;
   for (int i = 0; i < int(loadedGame[PLAYER_2_SCORE].length()); ++i){
      if (!(isdigit(loadedGame[PLAYER_2_SCORE][i])) || (loadedGame[PLAYER_2_SCORE][i] == ' ')){
         std::cout << "Invalid Format of Load File: SCORE. Please Try Again" << std::endl;
         optionAction("2");
         }
      }
   player2->setScore(std::stoi(loadedGame[PLAYER_2_SCORE]));
   
   // 6: player2Tiles;
   std::istringstream ss1(loadedGame[PLAYER_2_TILE]);
   std::vector<std::string> tilesToLoad1;
   std::string tileValuePair1;
   while (ss1 >> tileValuePair1)
   {
      tilesToLoad1.push_back(tileValuePair1);
   }
   for (std::string tmp: tilesToLoad1){
      // check tmp size
      if (!((tmp.size() <= 5) && (tmp.size() >= 3))) {
         std::cout << "Invalid Format of Load File: TILE BAG. Please Try Again" << std::endl;
         optionAction("2");
      }
      // check hand tiles letter and '-'
      if (!(isupper(tmp[0]) && tmp[1] == '-')) {
         std::cout << "Invalid Format of Load File: TILE BAG. Please Try Again" << std::endl;
         optionAction("2");
      }
      char letterToLoad1 = tmp[0];
      int valueToLoad1 = std::stoi(tmp.substr(2, 2));
      checkHand = player2->getHand()->add(letterToLoad1, valueToLoad1);
      if (checkHand == 0){
         std::cout << "Load failed: hand size reaches out of bound" << std::endl;
         std::cout << "Invalid Format of Load File. Please try again" << std::endl;
         optionAction("2");
      }
   }

   // Process Board Info
   // Locate the Board, starts at line 7, coordinates start at line 9, which is index 8 for loadedGame. 
   // At least 2x2 saved board
   int boardEndIndex = 0;
   int boardRows = 0;
   int boardCols = 0;
   // calculate board rows
   boardEndIndex = loadedGame.size() - BOARD_END_LOCATOR - 1;
   boardRows = boardEndIndex - BOARD_START_LINE_INDEX + 1;
   std::istringstream ss2(loadedGame[BOARD_START_LINE - 1]);
   std::vector<std::string> colsToCalc;
   std::string col;
   while (ss2 >> col)
   {
      colsToCalc.push_back(col);
   }
   for (std::string tmp: colsToCalc){
      boardCols = std::stoi(tmp) + 1;
   }

   // Initialize a board for loading
   std::vector<std::vector<std::shared_ptr<Tile>>> loadedBoard(MAX_BOARD_SIZE, std::vector<std::shared_ptr<Tile>>(MAX_BOARD_SIZE, 0));
   for (int row = 0; row < MAX_BOARD_SIZE; ++row) {
      for (int col = 0; col < MAX_BOARD_SIZE; ++col) {
         loadedBoard[row][col] = nullptr;
      }
   }

   // Place tiles into board to load
   for (int row = 0; row < boardRows; ++row){
      for (int col = 0; col < boardCols; ++col){
         if (loadedGame[BOARD_START_LINE_INDEX + row][(col + 1) * TILES_INTERVAL] != ' ') {
            loadedBoard[row][col] = bag->getTile(loadedGame[BOARD_START_LINE_INDEX + row][(col + 1) * TILES_INTERVAL]);
         }
      }
   }

   // Load Tilebag
   // Clear bag
   bag->clearBag();
   // Load tiles into tile bag
   int tileBagIndex = boardEndIndex + 1;
   std::istringstream sss(loadedGame[tileBagIndex]);
   std::vector<std::string> tilebagToLoad;
   std::string tileBagPair;
   while (sss >> tileBagPair){
      tilebagToLoad.push_back(tileBagPair);
   }
   for (std::string tmp: tilebagToLoad){
      char letterToLoad = tmp[0];
      bag->addTile(letterToLoad);
   }

   // load turn_tracker
   if (loadedGame[loadedGame.size() - 1] == loadedGame[PLAYER_1_NAME]) {
      turn_tracker = false;
   } else if (loadedGame[loadedGame.size() - 1] == loadedGame[PLAYER_2_NAME])
   {
      turn_tracker = true;
   }
   
   // initialize the gameboard to load
   std::shared_ptr<GameBoard> gameBoardToLoad = std::make_shared<GameBoard>(player1, player2, bag, loadedBoard, turn_tracker);
   playGame(gameBoardToLoad);
}

//quits the program
void quitGame(){
   std::cout<<"\nGoodbye"<<std::endl;
   throw QuitGameExcept();
}

//prints the credits of the app
void credits(){
   std::cout<<"\n----------------------------------"<<std::endl;
   std::cout<<"Name: Amiru Chandrasena"<<std::endl;
   std::cout<<"Student ID: s3898594"<<std::endl;
   std::cout<<"Email: s3898594@student.rmit.edu.au"<<std::endl;
   std::cout<<std::endl;
   std::cout<<"Dominic Hu"<<std::endl;
   std::cout<<"Student ID: s3850132"<<std::endl;
   std::cout<<"Email: s3850132@student.rmit.edu.au"<<std::endl;
   std::cout<<std::endl;
   std::cout<<"Runhui Zhou"<<std::endl;
   std::cout<<"Student ID: s3865846"<<std::endl;
   std::cout<<"Email: s3865846@student.rmit.edu.au"<<std::endl;
   std::cout<<std::endl;
   std::cout<<"Philip Lee"<<std::endl;
   std::cout<<"Student ID: s3900509"<<std::endl;
   std::cout<<"Email: s3900509@student.rmit.edu.au"<<std::endl;
   std::cout<<"----------------------------------"<<std::endl;
   std::cout<<std::endl;
   printMenu();

}

