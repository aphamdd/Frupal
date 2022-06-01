# Project-Frupal
## Group 7 (authors):
 - Chiharu Akiyama
 - Huidong Wang
 - Austin Anderson
 - Anthony Pham
 - Camilo Schaser-Hughes
 
**November 10, 2020**
 
## Instructions:
 1. Make with `make`
 2. Run with `./frupal`
 3. Use arrow keys to explore, `q` to quit.
    Use `c` to enter and quit the check around mode. In check around mode, press `w`,`s`,`a`,`d` can check information of up, down, left, right direction.
 4. How to win: Find the Royal Diamonds (white '$' symbol).
 5. How to lose: Run out of energy before finding the Royal Diamonds.
 6. How to play: Hero can walk over meadows (green background) and swamps (purple background) with energy cost of 1 for meadows, 2 for swamps. Water (blue background) and walls (white background) are impassable, but water can be traversed at no energy cost if a ship ('S' symbol) is purchased. Food ('F' symbol) can be purchased with currency (whiffles) for energy.  Obstacles ('!' symbol) require energy to remove. Tools ('T' symbol) can be purchased to minimize energy cost when landing on obstacles. Binoculars ('B' symbol) can be purchased to increase visual range 2X. Treasure chests (black '$' symbol) give currency (whiffles). Clues ('?' symbol) give completely true or completely false information about the location of the Royal Diamonds. Beware. 
 7. Game menu: The right side panel of the screen will display important information about the game, such as, picked-up item details, movement options available to the hero, tools in tool belt, and current energy and currency. 
 
## Version:
 This branch version has tiles colored black until you discover them.  It has implementation of a load file that is somewhat reproducable to create a 128x128 map.  It has hero produced on a dynamic viewport over a terrain input from input files.  It has a scroll mechanism.  All color is correctly displayed.  SWAMPS cost 2 energy, MEADOWS 1, WALLS and WATER cost 1 and send you back to your old position, as does trying to walk off the map.  The hero dies when energy is 0 or less but has option to keep playing if the player purchases 100 energy. The grovnick is a struct. 
 There is implementation for loading item files and printing them on the map. All items are displayed in the game menu when the hero lands on them and the hero may purchase them if they are food, tools, binoculars or ships. The tool belt is always displayed in the game menu. When the hero lands on an obstacle, the hero may choose a tool to remove the obstacle. 
 

## Load terrain files:
 The load mechanism is built off of Chiharu's load mechanism. It is modified however to be able to create instance of terrain structures.  Any new file created needs to be linked inside of `terrain.txt`.  The `NUMTERRAIN` definition also needs to be changed in map.h.  The input structure is `int terrain;int y coord; int x coord`. y and x start from the top and left, respective to the map and controls.  After that you can type notes on the rest of the line to describe what you are making.
 Numbers asigned to terrain as follows:
  1. MEADOW is 1, if you want to overlay terrains.
  2. SWAMP is 2
  3. WATER is 3
  4. WALL is 4
  
 It initially sets everything to MEADOW, before loading in inputfiles.
 
 ##Load item files:
   All item text file names are in 'item.txt'. Any new item text files must be included in 'item.txt'. The 'NUMITEM' will need to be changed in map.h. The input structure for each item is written in 'item_data_order.txt'. 
 
 ## What we managed to implement:
  We were able to implement the majority of features that were required for the Frupal project. The features that weren't implemented were the added stretch goals such as the randomly generated levels, the map loader, and the level editor. Otherwise, we were able to implement all of the main requirements for the Frupal project.
  
## Presentation Video Link:
https://media.pdx.edu/media/t/1_ibh57uuc
 
 
 
 
 
 
