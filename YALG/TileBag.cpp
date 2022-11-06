#include "TileBag.h"

//Random method function 
int myrandom (int i) { return std::rand()%i;}

TileBag::TileBag(){
    //Declared Variables
    char tileLetter;
    int tileNum;
    std::string line_;
    std::string let;
    std::string next;
    //Reads in file name
    std::ifstream file_("ScrabbleTiles.txt");
    orderedTileList = std::make_shared<LinkedList>();
    

//reads in the file on scrabble tiles 
    if(file_.is_open())
    {
        while(getline(file_, line_)) {
            //Files the vector with the letters 
            //Clears the vector 
            arr.clear();
            //Adds the input as string to vector one at a time
             std::istringstream iss(line_);
             while (iss >> let) {
               arr.push_back(let);
             }

            
            //sets tile variable to first letter 
            //converts the string into char and int 
            tileLetter = arr[0].at(0);
            tileNum = stoi(arr[1]);

            //loads into tile 
            std::shared_ptr<Tile> b1 = std::make_shared<Tile> (tileLetter,tileNum);

       
            allTiles.push_back(b1);
            
        }

        //closes file after reading   
        file_.close();
    }else{
        std::cout<<"File not open"<<std::endl;
    }

    std::srand (unsigned (std::time(0)));
    //Randomly pick an index of the vetcor and store it in a linked list
    // std::random_shuffle(std::begin(allTiles), std::end(allTiles),myrandom);
    // std::shuffle(std::begin(allTiles), std::end(allTiles), std::default_random_engine());
    std::random_device rd;
    std::mt19937 g(rd());
 
    std::shuffle(allTiles.begin(), allTiles.end(), g);


    for(int i = 0; i < int(allTiles.size()); i++){
        orderedTileList->add_back(allTiles[i]);
    }
}


TileBag::~TileBag(){}


std::shared_ptr<Tile> TileBag::drawTile(){

    std::shared_ptr<Tile> tileToDraw;

    if (this->checkEmpty()) {
        tileToDraw = nullptr;
    }
    else {
        tileToDraw = orderedTileList->getHeadNode()->tile;
        orderedTileList->remove_front();
    }
    
    return tileToDraw;
} 

bool TileBag::checkEmpty(){
    return (orderedTileList->getHeadNode() == nullptr);
}

std::string TileBag::saveTileBag() {
    std::string saveBag;
    std::shared_ptr<Node> t = orderedTileList->getHeadNode();
    while (t != nullptr) {
       
        saveBag += t->tile->letter;
        saveBag += "-";
        saveBag += std::to_string(t->tile->value);
        if (t->next != nullptr) {
            saveBag += ", ";
        }
       
        t = t->next;
    }
    saveBag += "\n";
    return saveBag;
}

void TileBag::clearBag(){
    orderedTileList->clear();
}

std::shared_ptr<Tile> TileBag::getTile(char letter) {
    
    std::shared_ptr<Tile> retVal = nullptr;

    for (int i = 0; i < int(allTiles.size()); ++i) {
        if (allTiles[i]-> letter == letter) {
            retVal = allTiles[i];
        }
    }

    return retVal;
}
void TileBag::addTile(std::shared_ptr<Tile> tile) {
    orderedTileList->add_back(tile);
}

void TileBag::addTile(char letter) {
    this->addTile(this->getTile(letter));
}
