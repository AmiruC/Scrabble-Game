#ifndef ASSIGN2_LINKEDLIST_H
#define ASSIGN2_LINKEDLIST_H

#include "Tile.h"
#include "Node.h"


// Original LinkedList.h

// #ifndef ASSIGN2_LINKEDLIST_H
// #define ASSIGN2_LINKEDLIST_H

// #include "Node.h"

// class LinkedList {
// public:

//    LinkedList();
//    ~LinkedList();

// private:
//    Node* head;
// };

// #endif // ASSIGN2_LINKEDLIST_H


class LinkedList{
public:
    LinkedList();
    LinkedList(LinkedList& other);
    ~LinkedList();

    int size();
    std::shared_ptr<Tile> get(int index);
    std::shared_ptr<Node> getHeadNode();
    std::shared_ptr<Node> getTailNode();

    void add_front(std::shared_ptr<Tile> data);
    void add_back(std::shared_ptr<Tile> data);

    //Contract: Elements should exist in the list to be deleted.
    void remove_front();
    void remove_back();
    void remove(int index);
    void clear();

private:
    std::shared_ptr<Node> head;
    std::shared_ptr<Node> tail;
};


#endif // ASSIGN2_LINKEDLIST_H