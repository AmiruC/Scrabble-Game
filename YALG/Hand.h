#ifndef ASSIGN_2_HAND_H
#define ASSIGN_2_HAND_H

#include "LinkedList.h"
#include "memory"
#define MAX_HAND_SIZE      7
#define INDEX_NOT_FOUND    100
#define HAND_IS_EMPTY         200

class Hand {
public:
    Hand();
    Hand(Hand& other);
    ~Hand();

    int size();

    // Get index by letter
    int searchTile(char letter);

    // Get tile by letter
    std::shared_ptr<Tile> get(char letter);
    // Get tile by index
    std::shared_ptr<Tile> get(int index);
    // Add tile
    bool add(std::shared_ptr<Tile> tile);
    // Add tile by letter,value
    bool add(char letter, int value);


    // Remove tile by letter
    bool remove(char letter);
    
    void printHand();
    void clear();
    std::string saveHand();
    
private:
    std::shared_ptr<LinkedList> hand;
};

#endif