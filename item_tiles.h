/*
 File name: item_tiles.h
 Author: Austin Anderson
 Group #7
 Version: 0.2
 */

#ifndef ITEMS_H
#define ITEMS_H

#include <ncurses.h>
#include <string>
// Needed for the binoculars
//#include "hero.h"
// Needed for placement
//#include "map.h"
using namespace std;

// ABSTRACT ITEM BASE CLASS
class Item {
public:
  Item();
  Item(char icon, string type);
  Item(const Item &item); // initializes to item. useful for adding tools to
                          // hero's toolbelt
  Item *&get_next();      // returns next
  void get_type(string &type); // returns type

  // XXX THIS IS ALL STUFF THAT I AM ADDING / SUBTRACTING.
  char get_char() const; // returns items icon for map to print it.

  virtual void display_info(WINDOW *game_menu, int row, int col) = 0;
  virtual void show_info(WINDOW *game_menu, int row, int col) = 0;
  virtual int get_energy();
  virtual int get_cost();
  virtual void get_obstacle_type(string &ob_type); // function for Tool class
  virtual int get_whiffles();                      // function for Treasure
  virtual void get_message(string &msg);           // function for Clue class
  virtual void get_name(string &name); // returns Tool, Food, Obstacle name
  virtual void get_terrain_type(string &type); // function for Ship class
  virtual ~Item();

protected:
  char icon;        // For print function
  string item_type; // Food, Tools, etc..
  Item *next;
};

// DERIVED CLUE CLASS
class Clue : public Item {
public:
  Clue();
  Clue(char icon, string type, string message);
  Clue(const Item &item, string message);
  void display_info(WINDOW *game_menu, int row, int col);
  void show_info(WINDOW *game_menu, int row, int col);
  void get_message(string &msg); // for outfile
  ~Clue(){};

private:
  string message; // true or false clue
};

// DERIVED TOOL CLASS
class Tool : public Item {
public:
  Tool();
  Tool(char icon, string type, string name, int energy, int cost,
       string obstacle_type);
  Tool(const Item &item, string name, int energy, int cost,
       string obstacle_type);
  Tool(const Tool &tool);
  int get_energy();
  int get_cost();
  void get_name(string &name);
  void get_obstacle_type(string &type);
  void display_info(WINDOW *game_menu, int row, int col);
  void show_info(WINDOW *game_menu, int row, int col);
  ~Tool(){};

private:
  string tool_name;
  int cost;
  int rating;
  string obstacle_type; // obstacle type the tool matches to, so if the tool is
                        // a hammer, the matching obstacle is 'boulder'
};

// DERIVED FOOD CLASS
class Food : public Item {
public:
  Food();
  Food(char icon, string type, string name, int energy, int cost);
  Food(const Item &item, string name, int energy, int cost);
  //           Food(const Food & food);  <--   may not need
  int get_energy();
  int get_cost();
  void get_name(string &name);
  void display_info(WINDOW *game_menu, int row, int col);
  void show_info(WINDOW *game_menu, int row, int col);
  ~Food(){};

private:
  string food_name;
  int food_cost;
  int energy_value;
};

// DERIVED OBSTACLE CLASS
class Obstacle : public Item {
public:
  Obstacle();
  Obstacle(char icon, string type, string name, int energy);
  Obstacle(const Item &item, string name, int energy);
  int get_energy(); // return energy cost
  void get_name(string &name);
  void display_info(WINDOW *game_menu, int row, int col);
  void show_info(WINDOW *game_menu, int row, int col);
  ~Obstacle(){};

private:
  int energy_cost;
  string obstacle_name;
};

// DERIVED TREASURE CHEST CLASS
class Treasure_chest : public Item {
public:
  Treasure_chest();
  Treasure_chest(char icon, string type, int whiffles);
  Treasure_chest(const Item &item, int whiffles);
  int get_whiffles();
  void display_info(WINDOW *game_menu, int row, int col);
  void show_info(WINDOW *game_menu, int row, int col);
  ~Treasure_chest(){};

private:
  int whiffle_value;
};

// DERIVED DIAMOND CLASS
class Diamond : public Item {
public:
  Diamond();
  Diamond(char icon, string type, int whiffles);
  Diamond(const Item &item, int whiffles);
  int get_whiffles();
  void display_info(WINDOW *game_menu, int row, int col);
  void show_info(WINDOW *game_menu, int row, int col);
  ~Diamond(){};

private:
  long long whiffle_value;
};

// DERIVED SHIP CLASS
class Ship : public Item {
public:
  Ship();
  Ship(char icon, string type, int cost, string obstacle_type);
  Ship(const Item &item, int cost, string obstacle_type);
  Ship(const Ship &ship);
  int get_cost();
  void get_terrain_type(string &type);
  void display_info(WINDOW *game_menu, int row, int col);
  void show_info(WINDOW *game_menu, int row, int col);
  ~Ship(){};

private:
  int cost;
  string terrain_type; // terrain type ship matches to, so 'water' goes here
};

// DERIVED BINOCULARS CLASS
class Binoculars : public Item {
public:
  Binoculars();
  Binoculars(char icon, string type, int cost);
  Binoculars(const Item &item, int cost);
  int get_cost();
  void display_info(WINDOW *game_menu, int row, int col);
  void show_info(WINDOW *game_menu, int row, int col);
  ~Binoculars(){};

private:
  int cost;
};

#endif
