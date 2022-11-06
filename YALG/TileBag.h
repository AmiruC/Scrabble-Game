#ifndef  ASSIGN2_TileBag_H 
#define ASSIGN2_TileBag_H
#include <sstream>
#include <memory>
#include <iostream>
#include <fstream>
#include "Node.h"
#include "LinkedList.h"
#include "Tile.h"
#include <algorithm>    // std::random_shuffle
#include <vector>       // std::vector
#include <ctime>        // std::time
#include <cstdlib>  
#include <random>    // std::rand, std::srand


class TileBag {
public:
TileBag();
~TileBag();
bool checkEmpty();
std::shared_ptr<Tile> drawTile();
std::string saveTileBag();
void clearBag();
std::shared_ptr<Tile> getTile(char letter);
void addTile(std::shared_ptr<Tile> tile);
void addTile(char letter);

private:
    std::vector<std::string> arr;
    std::vector<std::shared_ptr<Tile> > allTiles;
    std::shared_ptr<LinkedList> orderedTileList;
};

#endif 
