#include <memory>
#include <iostream>
#include "Node.h"
#include "Tile.h"

//Default Constructor
Node::Node() {
   this->tile = nullptr;
   this->next = nullptr;
   this->prev = nullptr;
}

//Assign Values
Node::Node(std::shared_ptr<Tile> tile, std::shared_ptr<Node> next)
{
   this->tile = tile;
   this->next = next;
   this->prev = nullptr;
}

//Copy Constructor
Node::Node(Node& other)
{
   if (other.tile != nullptr) {
      std::shared_ptr<Tile> t = std::make_shared<Tile>(other.tile->letter, other.tile->value);
      this->tile = t;
      this->next = other.next;
      this->prev = other.prev;
   } else {
      std::cout << "ERROR: Input a Node Pointer" << std::endl;
   }
}

//Deconstructor
Node::~Node(){
    next = nullptr;
    prev = nullptr;
    tile = nullptr;
}
