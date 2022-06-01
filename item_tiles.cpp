/*
 File name: item_tiles.cpp
 Author: Austin Anderson
 Group #7
 Version: 0.1
 */
#include "item_tiles.h"

// ITEM IMPLEMENTATION
Item::Item() {
  icon = ' ';
  item_type = " ";
  next = NULL;
}
Item::Item(char item_icon, string type) {
  item_type = type;
  icon = item_icon;
  next = NULL;
}

Item::Item(const Item &item) {
  item_type = item.item_type;
  icon = item.icon;
  next = NULL;
}
Item *&Item::get_next() { return next; }

void Item::get_type(string &type) { type = item_type; }

// XXX THIS IS STUFF/FUNCTIONS ADDING/SUBTRACTING

char Item::get_char() const { return icon; }

Item::~Item() {}

// REMOVED  print_icon(WINDOW *,int), get_icon(&char)
// END CHANGES BY CAMILO

// VIRTUAL FUNCTIONS IN ITEM
int Item::get_energy() { return 0; }
int Item::get_cost() { return 0; }
void Item::get_obstacle_type(string &ob_type) {}
int Item::get_whiffles() { return 0; }
void Item::get_message(string &msg) {}
void Item::get_name(string &name) {}
void Item::get_terrain_type(string &type) {}

// CLUE IMPLEMENTATION
Clue::Clue() {
  icon = '?';
  item_type = "clue";
  message = " "; //<---random_clue();
}
Clue::Clue(char icon, string type, string msg)
    : Item(icon, type), message(msg) {}
Clue::Clue(const Item &item, string msg) : Item(item), message(msg) {}
void Clue::display_info(WINDOW *gm, int row, int col) {

  // this part requires player to press 'c' to continue
  keypad(gm, TRUE);
  char choice = 'v'; // for view clue
  int input = 0;

  while (choice == 'v') {

    mvwprintw(gm, row + 1, col, " > Clue: ");
    waddstr(gm, message.data());
    mvwprintw(gm, row + 3, col, "  Press C to continue: ");
    wrefresh(gm);
    input = wgetch(gm);
    if (input == 'c' || input == 'C')
      choice = 'c';
  }
  wmove(gm, row + 1, col);
  wclrtoeol(gm);
  wmove(gm, row + 2, col);
  wclrtoeol(gm);
  wmove(gm, row + 3, col);
  wclrtoeol(gm);
  mvwprintw(gm, row + 1, col, "  Onward then! ");
  wrefresh(gm);
}

void Clue::show_info(WINDOW *gm, int row, int col) {

  mvwprintw(gm, row + 1, col, " > Clue: ");
  waddstr(gm, message.data());
  wrefresh(gm);
}
void Clue::get_message(string &msg) { msg = message; }
/*
void clue::random_clue(){
        //Stub
        //There will likely be an addition of random values to the header file
call
        //Using a seeded value, the possibility of the correct clue can be set
to
        //being reasonable to downright impossible to use.
        return;
}


*/

// TOOL IMPLEMENTATION
Tool::Tool() {
  icon = 'T';
  item_type = "tool";

  tool_name = " ";
  cost = 0;
  rating = 0;
  obstacle_type = " ";
}
Tool::Tool(char icon, string type, string name, int e_amount, int w_cost,
           string ob_type)
    : Item(icon, type), tool_name(name), cost(w_cost), rating(e_amount),
      obstacle_type(ob_type) {}
Tool::Tool(const Item &item, string name, int e_amount, int w_cost,
           string ob_type)
    : Item(item), tool_name(name), cost(w_cost), rating(e_amount),
      obstacle_type(ob_type) {}

Tool::Tool(const Tool &tool)
    : Item(tool), tool_name(tool.tool_name), cost(tool.cost),
      rating(tool.rating), obstacle_type(tool.obstacle_type) {}
int Tool::get_energy() { return rating; }
int Tool::get_cost() { return cost; }
void Tool::get_name(string &name) { name = tool_name; }
void Tool::get_obstacle_type(string &type) { type = obstacle_type; }
void Tool::display_info(WINDOW *gm, int row, int col) {
  mvwprintw(gm, row++, col, " > Tool: ");
  waddstr(gm, tool_name.data());
  mvwprintw(gm, row++, col, " > Cost: %d", cost);
  mvwprintw(gm, row++, col, " > Rating: %dX", rating);
  mvwprintw(gm, row++, col, " > Obstacle type: ");
  waddstr(gm, obstacle_type.data());
  mvwprintw(gm, row++, col, " > Would you like to buy?");
  mvwprintw(gm, row++, col, " > (Y)es or (N)o?");
  wrefresh(gm);
}

void Tool::show_info(WINDOW *gm, int row, int col) {

  mvwprintw(gm, row++, col, " > Tool: ");
  waddstr(gm, tool_name.data());
  mvwprintw(gm, row++, col, " > Cost: %d", cost);
  mvwprintw(gm, row++, col, " > Rating: %dX", rating);
  mvwprintw(gm, row++, col, " > Obstacle type: ");
  waddstr(gm, obstacle_type.data());
  wrefresh(gm);
}
// FOOD IMPLEMENTATION
Food::Food() {
  icon = 'F';
  item_type = "food";
  food_name = " ";
  food_cost = 0;
  energy_value = 0;
}
Food::Food(char icon, string type, string name, int energy, int cost)
    : Item(icon, type), food_name(name), food_cost(cost), energy_value(energy) {

}
Food::Food(const Item &item, string name, int energy, int cost)
    : Item(item), food_name(name), food_cost(cost), energy_value(energy) {}
int Food::get_energy() { return energy_value; }
int Food::get_cost() { return food_cost; }
void Food::get_name(string &name) { name = food_name; }
void Food::display_info(WINDOW *gm, int row, int col) {
  mvwprintw(gm, row++, col, " > Food: ");
  waddstr(gm, food_name.data());
  mvwprintw(gm, row++, col, " > Cost: %d whiffles", food_cost);
  mvwprintw(gm, row++, col, " > Energy: %d", energy_value);
  mvwprintw(gm, row++, col, " > Would you like to buy?");
  mvwprintw(gm, row++, col, " > (Y)es or (N)o?");
  wrefresh(gm);
}

void Food::show_info(WINDOW *gm, int row, int col) {

  mvwprintw(gm, row++, col, " > Food: ");
  waddstr(gm, food_name.data());
  mvwprintw(gm, row++, col, " > Cost: %d whiffles", food_cost);
  mvwprintw(gm, row++, col, " > Energy: %d", energy_value);
  wrefresh(gm);
}
// OBSTACLE IMPLEMENTATION
Obstacle::Obstacle() {
  icon = '!';
  item_type = "obstacle";
  energy_cost = 0;
  obstacle_name = " ";
}
Obstacle::Obstacle(char icon, string type, string name, int energy)
    : Item(icon, type), energy_cost(energy), obstacle_name(name) {}
Obstacle::Obstacle(const Item &item, string name, int energy)
    : Item(item), energy_cost(energy), obstacle_name(name) {}
int Obstacle::get_energy() { return energy_cost; }
void Obstacle::get_name(string &name) { name = obstacle_name; }
void Obstacle::display_info(WINDOW *gm, int row, int col) {
  //   werase(gm);
  mvwprintw(gm, row++, col, " > Obstacle: ");
  waddstr(gm, obstacle_name.data());
  mvwprintw(gm, row++, col, " > Removal energy: %d", energy_cost);
  wrefresh(gm);
}

void Obstacle::show_info(WINDOW *gm, int row, int col) {
  mvwprintw(gm, row++, col, " > Obstacle: ");
  waddstr(gm, obstacle_name.data());
  mvwprintw(gm, row++, col, " > Removal energy: %d", energy_cost);
  wrefresh(gm);
}
// TREASURE_CHEST IMPLEMENTATION
Treasure_chest::Treasure_chest() {
  icon = '$';
  item_type = "treasure";
  whiffle_value = 0;
}
Treasure_chest::Treasure_chest(char icon, string type, int whiffles)
    : Item(icon, type), whiffle_value(whiffles) {}
Treasure_chest::Treasure_chest(const Item &item, int whiffles)
    : Item(item), whiffle_value(whiffles) {}
int Treasure_chest::get_whiffles() { return whiffle_value; }
void Treasure_chest::display_info(WINDOW *gm, int row, int col) {
  //   werase(gm);
  mvwprintw(gm, row++, col, " > You found a Treasure Chest!!");
  mvwprintw(gm, row++, col, " > Reward: %d whiffles!!", whiffle_value);
  mvwprintw(gm, row++, col, " > Press <ENTER> to continue.");
  wrefresh(gm);
}

void Treasure_chest::show_info(WINDOW *gm, int row, int col) {
  //   werase(gm);
  mvwprintw(gm, row++, col, " > You found a Treasure Chest!!");
  mvwprintw(gm, row++, col, " > Reward: %d whiffles!!", whiffle_value);
  mvwprintw(gm, row++, col, " > Press <ENTER> to continue.");
  wrefresh(gm);
}
// DIAMOND IMPLEMENTATION
Diamond::Diamond() {
  icon = '$';
  item_type = "diamond";
  whiffle_value = 0;
}
Diamond::Diamond(char icon, string type, int whiffles)
    : Item(icon, type), whiffle_value(whiffles) {}
Diamond::Diamond(const Item &item, int whiffles)
    : Item(item), whiffle_value(whiffles) {}
int Diamond::get_whiffles() { return whiffle_value; }
void Diamond::display_info(WINDOW *gm, int row, int col) {
  //   werase(gm);
  mvwprintw(gm, row++, col, " > ROYAL DIAMOND!!");
  mvwprintw(gm, row++, col, " > Bounty: %d whiffles!!", whiffle_value);
  wrefresh(gm);
}

void Diamond::show_info(WINDOW *gm, int row, int col) {
  //   werase(gm);
  mvwprintw(gm, row++, col, " > ROYAL DIAMOND!!");
  mvwprintw(gm, row++, col, " > Bounty: %d whiffles!!", whiffle_value);
  wrefresh(gm);
}
// SHIP IMPLEMENTATION
Ship::Ship() {
  icon = 'S';
  item_type = "ship";
  cost = 0;
  terrain_type = " ";
}
Ship::Ship(char icon, string type, int w_cost, string t_type)
    : Item(icon, type), cost(w_cost), terrain_type(t_type) {}
Ship::Ship(const Item &item, int w_cost, string t_type)
    : Item(item), cost(w_cost), terrain_type(t_type) {}
Ship::Ship(const Ship &ship)
    : Item(ship), cost(ship.cost), terrain_type(ship.terrain_type) {}
int Ship::get_cost() { return cost; }
void Ship::get_terrain_type(string &type) { type = terrain_type; }
void Ship::display_info(WINDOW *gm, int row, int col) {
  //   werase(gm);
  mvwprintw(gm, row++, col, " > Ship!!");
  mvwprintw(gm, row++, col, " > Cost: %d whiffles", cost);
  mvwprintw(gm, row++, col, " > Terrain type: ");
  waddstr(gm, terrain_type.data());
  mvwprintw(gm, row++, col, "Would you like to buy?");
  mvwprintw(gm, row++, col, "(Y)es or (N)o?");
  wrefresh(gm);
}

void Ship::show_info(WINDOW *gm, int row, int col) {
  mvwprintw(gm, row++, col, " > Ship!!");
  mvwprintw(gm, row++, col, " > Cost: %d whiffles", cost);
  mvwprintw(gm, row++, col, " > Terrain type: ");
  waddstr(gm, terrain_type.data());
  wrefresh(gm);
}
// BINOCULAR IMPLEMENTATION
Binoculars::Binoculars() {
  icon = 'B';
  item_type = "binoculars";
  cost = 0;
}
Binoculars::Binoculars(char icon, string type, int w_cost)
    : Item(icon, type), cost(w_cost) {}
Binoculars::Binoculars(const Item &item, int w_cost)
    : Item(item), cost(w_cost) {}
int Binoculars::get_cost() { return cost; }
void Binoculars::display_info(WINDOW *gm, int row, int col) {
  //   werase(gm);
  mvwprintw(gm, row++, col, " > Binoculars!!");
  mvwprintw(gm, row++, col, " > Cost: %d whiffles", cost);
  mvwprintw(gm, row++, col, "Would you like to buy?");
  mvwprintw(gm, row++, col, "(Y)es or (N)o?");
  wrefresh(gm);
}

void Binoculars::show_info(WINDOW *gm, int row, int col) {
  mvwprintw(gm, row++, col, " > Binoculars!!");
  mvwprintw(gm, row++, col, " > Cost: %d whiffles", cost);
  wrefresh(gm);
}
