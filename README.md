Hi welcome. This is a C++ Terminal 2 - Player Scrabble game built during late 2022.

This game was challenging to make forcing me to learn very quickly about C++ and its capabilities which i throughougly enjoye.

To play this game:
  - Download the Source Code 
  - In your terminal navigate to the file & Run this command: make 
    - This runns the make file and compiles the code to be run
  - Next run this command: ./make
  - This will start the game make sure to read the rules before playing the game(Note: Names must be typed All Caps)
  - Enjoy
  


  How To Play:
     The current player may then take one of two actions:
    1. Place tile(s) onto the board to form a word
    2. Replace one tile in their hand
    3. Pass. Miss the turn and proceed to the other player.
    Once the player successfully takes their action, their turn ends, and the other player’s turn starts.
    Alternatively, the player may perform one of two game functions:
    1. Save the game to a file
    2. Quit the game
    Below is an example of a sequence of actions and game functions for two players, named A and B. 
    
    - To place a tile type: 
      - Place <Letter you want eg..C> at <Square you want eg..E1>


Example Game Play:

A, it's your turn
Score for A: 6
Score for B: 6
0 1 2 3 4 5
-------------------------
A | | | | | | |
B | | | | W | I | N |
C | | | | A | | |
D | P | L | A | Y | S | |
E | | | | | | |
F | | | | | | |
Your hand is
Y-4, C-3, A-1, B-3, R-1, S-1, E-1
> place C at C1
> place A at E1
> place Y at F1
> place Done
B, it's your turn
Score for A: 14
Score for B: 6
0 1 2 3 4 5
-------------------------
A | | | | | | |
B | | | | W | I | N |
C | | C | | A | | |
D | P | L | A | Y | S | |
E | | A | | | | |
F | | Y | | | | |
Your hand is
E-1, A-1, H-4, J-8, O-1, G-2, W-4
> replace A
< gameplay continues >
