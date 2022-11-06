#include "Hand.h"
#include <iostream>


Hand::Hand() {
    hand = std::make_shared<LinkedList>();
}

Hand::Hand(Hand& other) {
    hand = std::make_shared<LinkedList>(*other.hand);

}

Hand::~Hand() {
}

int Hand::size() {
    return hand->size();
}



int Hand::searchTile(char letter) {
    std::shared_ptr <Node> tmp = std::make_shared<Node>();
    int index = hand->size() - 1;
    int flag = 0;
    int result = 0;
    tmp = hand->getTailNode();
    // Check if it is empty linked list
    if (tmp == nullptr){
        result = HAND_IS_EMPTY;
    }
    // If not empty
    else{
        while (tmp != nullptr){
            // If element is in the linked list
            if (tmp->tile->letter == letter){
                flag = 0;
                result = index;
            }
            else{
                flag++;
            }
            index--;
            tmp = tmp->prev;
        }
        // If element is not in the linked list
        if (flag == hand->size()){
            result = INDEX_NOT_FOUND;
        }
    }
    return result;
}

std::shared_ptr<Tile> Hand::get(char letter) {
    return hand->get(this->searchTile(letter));
}

std::shared_ptr<Tile> Hand::get(int index) {
    return hand->get(index);
}

bool Hand::add(std::shared_ptr<Tile> tile) {

    bool retVal;
    if (hand->size() >= MAX_HAND_SIZE) {
        retVal = false;
    } else {
        hand->add_back(tile);
        retVal = true;
    }
    return retVal;
}

bool Hand::add(char letter, int value) {
    bool retVal;
    std::shared_ptr<Tile> tile = std::make_shared<Tile>(letter,value);
    if (hand->size() < 7) {
        this->add(tile);
        retVal = true;
    }
    else {
        retVal = false;
    }
    return retVal;
}

bool Hand::remove(char letter) {

    bool retVal;

    if (this->searchTile(letter) == INDEX_NOT_FOUND || this->searchTile(letter) == HAND_IS_EMPTY){
        retVal = false;
    }
    else{
        hand->remove(this->searchTile(letter));
        retVal = true;
    }
    return retVal;
}

void Hand::printHand() {
    std::cout << saveHand() << std::endl;
}
void Hand::clear() {
    hand->clear();
}

std::string Hand::saveHand() {
    std::string saveHand;
    int size = hand->size();
    if (size == 0)
    {
        saveHand = "Hand is empty\n";
    }
    else
    {
        for (int i = 0; i < size; ++i)
        {
            saveHand += hand->get(i)->letter;
            saveHand += "-";
            saveHand += std::to_string(hand->get(i)->value);
            if (i < (size - 1)) {
                saveHand += ", ";
            }
        }
        saveHand += "\n";
    }
    return saveHand;
}