#include "LinkedList.h"
#include <stdexcept>
#include <memory>

//Default Constructor
LinkedList::LinkedList(){
    head = nullptr;
    tail = nullptr;
}

//Copy Constructor
LinkedList::LinkedList(LinkedList& other){
    head = nullptr;
    tail = nullptr;
    for(int i = 0; i < other.size(); ++i){
        std::shared_ptr<Tile> tile = std::make_shared<Tile>(*other.get(i));
        add_back(tile);
    }
}

//Deconstructor
LinkedList::~LinkedList(){
    clear();
}

//Return number of nodes in linked list
int LinkedList::size(){
    
    int length = 0;

    std::shared_ptr<Node> current = head;
    while(current != nullptr){
        ++length;
        current = current->next;
    }
    return length;
}

//Return tile at given index
std::shared_ptr<Tile> LinkedList::get(int index){
    std::shared_ptr<Tile> retTile = nullptr;

    if(index >= 0 && index < size()){

        int counter = 0;
        std::shared_ptr<Node> current = head;

        while(counter<index){
            ++counter;
            current = current->next;
        }

        retTile = current->tile;

    }
    return retTile;
}

//Return linked list
std::shared_ptr<Node> LinkedList::getHeadNode(){
    return head;
}
std::shared_ptr<Node> LinkedList::getTailNode(){
    return tail;
}

//Add tile to the head of linked list
void LinkedList::add_front(std::shared_ptr<Tile> data){
    std::shared_ptr<Node> node = std::make_shared<Node>();
    node->tile = data;
    node->next = head;
    node->prev = nullptr;

    if(head == nullptr){
        tail = node;
    }else{
        head->prev = node;
    }

    head = node;
}

//Add tile to back of linked list
void LinkedList::add_back(std::shared_ptr<Tile> data){
    std::shared_ptr<Node> node = std::make_shared<Node>();
    node->tile = data;
    node->next = nullptr;
    node->prev = tail;

    if(head == nullptr){
        head = node;
    }else{
        tail->next = node;
    }
    
    tail = node;

}

//Remove tile from head of linked list
void LinkedList::remove_front(){
    if(head != nullptr){
        head = head->next;

        if(head == nullptr){
            tail = nullptr;
        }else{
            head->prev = nullptr;
        }
    }else{
        throw std::runtime_error("Nothing to remove");
    }

}

//Remove tile from tail of linked list
void LinkedList::remove_back(){

    if(head != nullptr){
        tail = tail->prev;

        if(tail == nullptr){
            head = nullptr;
        }else{
            tail->next = nullptr;
        }

    }else{
        throw std::runtime_error("Nothing to remove");
    }
    
}

//Remove tile from given index of linked list
void LinkedList::remove(int index){
    if(index >= 0 && index < size()){
        if(head != nullptr){
            int counter = 0;
            std::shared_ptr<Node> current = head;
            while(counter != index){
                ++counter;
                current = current->next;
            }

            if(current->prev == nullptr){
                head = current->next;
            }else{
                current->prev->next = current->next;
            }

            if(current->next == nullptr){
                tail = current->prev;
            }else{
                current->next->prev = current->prev;
            }
        }
    }
}

//Empty linked list
void LinkedList::clear(){
    while(head != nullptr){
        remove_front();
    }
}
