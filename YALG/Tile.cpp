#include <iostream>
#include <vector>
#include <fstream>
#include "Tile.h"

using namespace std;

//Default Constructor
Tile::Tile(Letter letter, Value value) {
    this->letter = letter;
    this->value = value;
}

// //Getters
// char Tile::getLetter() {
//     return this->letter;
// }

// int Tile::getValue() {
//     return this->value;
// }