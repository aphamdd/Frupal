/* potential rewrite of Map class
   I just want to see a working model up
   on our github, so we'll see if this sticks
   November 17, 2020
*/

#include "map.h"
#include <fstream>
#include <iostream>
#include <ncurses.h>
#include <string>

using namespace std;

////////////////////////////////////////////////////////////////////////////////
/*
    Our default constructors.  Normal default constructor deleted. The first
    Map constructor doesn't get used except for testing.
*/
////////////////////////////////////////////////////////////////////////////////

// used for testing just sets entire map to unviewed itemless meadow
Map::Map(WINDOW *vp, WINDOW *gm) {

  viewport = vp;
  gamemenu = gm;

  for (int i = 0; i < KSIZE; ++i)
    for (int j = 0; j < KSIZE; ++j) {
      frupal[i][j].square = MEADOW;
      frupal[i][j].viewed = false; // true total map, false for view
      frupal[i][j].feature = NULL;
    }
}

// Our default constructor
Map::Map(WINDOW *vp, WINDOW *gm, char *inputfile) {

  viewport = vp; // sets viewport for easy access
  gamemenu = gm; // sets gamemenu for easy access

  // initally sets to unviewed, itemless MEADOW
  for (int i = 0; i < KSIZE; ++i)
    for (int j = 0; j < KSIZE; ++j) {
      frupal[i][j].square = MEADOW;
      frupal[i][j].viewed = false; // true total map, false for view
      frupal[i][j].feature = NULL;
    }

  load_map(inputfile);

  ////---loading/editing/saving items on map ----/////
  update_display(0, 0);
  ifstream infile;
  infile.open("inputfiles/item.txt");
  if (!infile) {
    wprintw(viewport, "Cannot open item file.");
    wgetch(viewport);
    exit(1);
  }
  int j = 0;
  while (!infile.eof()) {

    infile >> item_list[j];
    infile.ignore(100, '\n');
    j++;
  }
  infile.close();

  load_items();
  update_display(0, 0);

  //   edit_item_map();
  //   save_item_map();
}

////////////////////////////////////////////////////////////////////////////////
/*
    Loading functions, first function loads file based on string.  loads that
    into a string array.  That array is filled with other files with drawings
    for the map.  Those files are loaded in in the load_terrain function.
*/
////////////////////////////////////////////////////////////////////////////////

void Map::load_map(string file) {

  ifstream infile;
  infile.open(file);
  if (!infile) {
    printw("Cannot open file.");
    exit(1);
  }

  int i = 0;
  string terrain_list[NUMTERRAIN];

  while (!infile.eof()) {
    infile >> terrain_list[i];
    infile.ignore(100, '\n');
    ++i;
  }
  infile.close();

  for (i = 0; i < NUMTERRAIN; ++i)
    load_terrain(terrain_list[i]);
}

void Map::load_terrain(string file) {

  int i, j, temp_terrain;

  ifstream infile;
  infile.open(file);

  if (!infile) { // error handling
    printw("Cannot open file.");
    exit(1);
  }

  // sample input: terrain#, yval, xval
  while (!infile.eof()) {
    infile >> temp_terrain; // loads in first num = enum type
    infile.get();
    infile >> i; // gets the yvalue of grovnick
    infile.get();
    infile >> j; // gets the xvalue of grovnick
    infile.ignore(100, '\n');
    // this converts the number to the matching enum terrain type
    frupal[i][j].square = static_cast<terrain>(temp_terrain);
  }
  infile.close();
}

////////////////////////////////////////////////////////////////////////////////
/*
    Display functions.  update_display takes in the y, x shift and prints the
    map according to the shift.  uses enum type conversion to int to match
    the COLOR_PAIR functions described in main.cpp 1-MEADOW, 2-SWAMP, 3-WATER
    4-WALL, 5-UNSEEN.
*/
////////////////////////////////////////////////////////////////////////////////

void Map::update_display(int starty, int startx) {

  int i, j, color, rows, cols;
  getmaxyx(viewport, rows, cols);

  for (i = starty; i < starty + rows && i < KSIZE; ++i)
    for (j = startx; j < startx + cols && j < KSIZE; ++j) {
      // this converts enum type to int match to color pair
      if (frupal[i][j].viewed)
        color = frupal[i][j].square; // converts enum to #, matched in main
      else
        color = UNSEEN; // UNSEEN = 5 matches init_pair in main

      wattron(viewport, COLOR_PAIR(color));
      if (frupal[i][j].feature) {
        string temp;
        frupal[i][j].feature->get_type(temp);
        if ((temp.compare("diamond") == 0) && color != UNSEEN)
          wattron(viewport, COLOR_PAIR(7));

        mvwaddch(viewport, i - starty, j - startx,
                 frupal[i][j].feature->get_char());

        if ((temp.compare("diamond") == 0) && color != UNSEEN)
          wattroff(viewport, COLOR_PAIR(7));

      } else {
        mvwaddch(viewport, i - starty, j - startx, ' '); // prints grovnick tile
      }
      wattroff(viewport, COLOR_PAIR(color));
    }
}

////////////////////////////////////////////////////////////////////////////////
/*
    Computational functions, helper functions, etc.
*/
////////////////////////////////////////////////////////////////////////////////

// ret -1 if off map, wall, or water, 2-swamp, 1-meadow
int Map::energy_cost(int y, int x, bool ship) {

  // first checks if off grid, then checks grovnick enum type
  if (y < 0 || x < 0 || y >= KSIZE || x >= KSIZE)
    return -1;
  else if (frupal[y][x].square == MEADOW)
    return 1;
  else if (frupal[y][x].square == SWAMP)
    return 2;
  else if (frupal[y][x].square == WALL)
    return -1;
  else if (frupal[y][x].square == WATER)
    return (ship ? 0 : -1);

  return 0; // this should never happen though.
}

// takes in hero's pos & binoculars, adjust map accordingly
void Map::look_around(int ypos, int xpos, bool binoculars) {

  int sight = 1;
  sight += binoculars;

  for (int i = ypos - sight; i <= ypos + sight; ++i)
    for (int j = xpos - sight; j <= xpos + sight; ++j)
      if (i >= 0 && j >= 0 && i < KSIZE && j < KSIZE) // check if on map
        frupal[i][j].viewed = true;
}

//////----Functions for loading/editing/saving item objects on map ----////
void Map::load_items() {

  string type, name, other;
  char icon = ' ';
  int i, j, energy, whiffles;
  i = j = energy = whiffles = 0;
  ifstream infile;

  for (int n = 0; n < NUMITEM; n++) {
    infile.open(item_list[n]);

    if (!infile) {
      wprintw(viewport, "Cannot open item file.");
      exit(1);
    }

    while (!infile.eof()) {
      infile.get(icon);
      infile.get();
      getline(infile, type, ';');

      if (type.compare("food") == 0 || type.compare("tool") == 0 ||
          type.compare("obstacle") == 0) {
        getline(infile, name, ';');
        infile >> energy;
        infile.get();
      }

      if (type.compare("clue") == 0 || type.compare("tool") == 0 ||
          type.compare("ship") == 0)
        getline(infile, other, ';');

      if (type.compare("obstacle") != 0 && type.compare("clue") != 0) {
        infile >> whiffles;
        infile.get();
      }

      infile >> i;
      infile.get();
      infile >> j;
      infile.ignore(100, '\n');

      // create objects and input data

      if (type.compare("food") == 0)
        frupal[i][j].feature = new Food(icon, type, name, energy, whiffles);
      if (type.compare("tool") == 0)
        frupal[i][j].feature =
            new Tool(icon, type, name, energy, whiffles, other);
      if (type.compare("obstacle") == 0)
        frupal[i][j].feature = new Obstacle(icon, type, name, energy);
      if (type.compare("clue") == 0)
        frupal[i][j].feature = new Clue(icon, type, other);
      if (type.compare("treasure") == 0)
        frupal[i][j].feature = new Treasure_chest(icon, type, whiffles);
      if (type.compare("ship") == 0)
        frupal[i][j].feature = new Ship(icon, type, whiffles, other);
      if (type.compare("binoculars") == 0)
        frupal[i][j].feature = new Binoculars(icon, type, whiffles);
      if (type.compare("diamond") == 0)
        frupal[i][j].feature = new Diamond(icon, type, whiffles);
    }

    infile.close();
  }
}

void Map::create_item(char icon, int i, int j) {

  switch (icon) {
  case 'F':
    frupal[i][j].feature = new Food;
    break;
  case 'T':
    frupal[i][j].feature = new Tool;
    break;
  case '!':
    frupal[i][j].feature = new Obstacle;
    break;
  case '$':
    frupal[i][j].feature = new Treasure_chest;
    break;
  case '?':
    frupal[i][j].feature = new Clue;
    break;
  case 'S':
    frupal[i][j].feature = new Ship;
    break;
  case 'B':
    frupal[i][j].feature = new Binoculars;
    break;
  default:
    break;
  }
  int num = frupal[i][j].square;
  wattron(viewport, COLOR_PAIR(num));
  wprintw(viewport, "%c", icon);
  wmove(viewport, i, j);
  wattroff(viewport, COLOR_PAIR(num));
  wrefresh(viewport);
}
void Map::remove_item(int row, int col) {

  delete frupal[row][col].feature;
  frupal[row][col].feature = NULL;
  wattron(viewport, COLOR_PAIR(frupal[row][col].square));
  wprintw(viewport, " ");
  wattroff(viewport, COLOR_PAIR(frupal[row][col].square));
}
bool Map::has_item(int i, int j) { return frupal[i][j].feature; }
void Map::print_item(WINDOW *gm, int i, int j) {
  string type;
  frupal[i][j].feature->get_type(type);
  if (type.compare("clue") == 0) {
    int width, height;
    getmaxyx(gm, height, width);
    if (!height) // this just to eliminate an error code from not using height
      wprintw(viewport, "height very small");
    WINDOW *clue_win = newwin(7, width - 2, 2, COLS - width + 2);
    frupal[i][j].feature->display_info(clue_win, 0, 2);
    delwin(clue_win);
  } else
    frupal[i][j].feature->display_info(gm, 3, 2);
}

void Map::show_item(WINDOW *gm, int i, int j) {
  frupal[i][j].feature->show_info(gm, 3, 2);
}

void Map::get_item(Item *&item, int i, int j) { item = frupal[i][j].feature; }
void Map::edit_item_map() {

  update_display(0, 0);

  int row = 15;
  int col = 15;
  char icon = ' ';
  char c = 'e';
  int input = 0;
  int starty = row;
  int startx = col;
  wrefresh(viewport);
  keypad(viewport, TRUE);

  while (1) {

    if (c == 'e') {
      input = wgetch(viewport);
      werase(gamemenu);

      mvwprintw(gamemenu, 40, 2, " Press 'f' for Food ");
      mvwprintw(gamemenu, 41, 2, " Press 't' for Tool");
      mvwprintw(gamemenu, 42, 2, " Press '!' for Clue");
      mvwprintw(gamemenu, 43, 2, " Press '$' for Treasure chest ");
      mvwprintw(gamemenu, 44, 2, " Press 's' for Ship");
      mvwprintw(gamemenu, 45, 2, " Press 'b' for Binoculars");
      mvwprintw(gamemenu, 46, 2, " Press 'r' for Remove item");
      mvwprintw(gamemenu, 47, 2, " Press 'q' for Exit");
      wrefresh(gamemenu);

      wrefresh(viewport);
      switch (input) {
      case KEY_UP:
        if (row > 0) {
          starty = --row;
          if (frupal[row][col].feature)
            frupal[row][col].feature->display_info(gamemenu, 4, 2);
          scroll_function(starty, startx, row, col);
          update_display(starty, startx);
          wmove(viewport, row, col);
        }
        break;
      case KEY_DOWN:
        if (row < KSIZE - 1) {
          starty = ++row;
          if (frupal[row][col].feature)
            frupal[row][col].feature->display_info(gamemenu, 4, 2);
          scroll_function(starty, startx, row, col);
          update_display(starty, startx);
          wmove(viewport, row, col);
        }
        break;
      case KEY_LEFT:
        if (col > 0) {
          startx = --col;
          if (frupal[row][col].feature)
            frupal[row][col].feature->display_info(gamemenu, 4, 2);
          scroll_function(starty, startx, row, col);
          update_display(starty, startx);
          wmove(viewport, row, col);
        }
        break;
      case KEY_RIGHT:
        if (col < KSIZE - 1) {
          startx = ++col;
          if (frupal[row][col].feature)
            frupal[row][col].feature->display_info(gamemenu, 4, 2);
          scroll_function(starty, startx, row, col);
          update_display(starty, startx);
          wmove(viewport, row, col);
        }
        break;
      case 'f':
        icon = 'F';
        create_item(icon, row, col);
        break;
      case 't':
        icon = 'T';
        create_item(icon, row, col);
        break;
      case '!':
        icon = '!';
        create_item(icon, row, col);
        break;
      case '$':
        icon = '$';
        create_item(icon, row, col);
        break;
      case '?':
        icon = '?';
        create_item(icon, row, col);
        break;
      case 's':
        icon = 'S';
        create_item(icon, row, col);
        break;
      case 'b':
        icon = 'B';
        create_item(icon, row, col);
        break;
      case 'r':
        remove_item(row, col);
        break;
      case 'q':
        c = 'q';
        break;
      default:
        break;
      }
      wrefresh(viewport);
    } else if (c == 'q')
      break;
    else
      break;

  } // end while loop
}

void Map::save_item_map() {

  for (int i = 0; i < NUMITEM; i++)
    save_item(item_list[i]);
}
void Map::save_item(string file) {

  string type;
  string name;
  string ob_type;
  string message;

  Item *item = NULL;
  ofstream outfile;

  outfile.open(file);
  for (int i = 0; i < KSIZE; i++) {
    for (int j = 0; j < KSIZE; j++) {

      if (frupal[i][j].feature) {
        item = frupal[i][j].feature;
        item->get_type(type);

        if (file.compare("inputfiles/" + type + ".txt") == 0) {

          // XXX THIS I CHANGED TO GET CHAR(), HOPEFULLY DOESN'T BREAK.
          outfile << item->get_char();
          outfile << ";";
          outfile << type; // outputs 'food', 'tool', etc..
          outfile << ";";

          if (type.compare("food") == 0 || type.compare("tool") == 0 ||
              type.compare("obstacle") == 0) {
            item->get_name(
                name); // outputs 'hammer', 'baked ham', 'boulder' etc..
            outfile << name;
            outfile << ";";
            outfile << item->get_energy(); // outputs Food/Obstacle energy or
                                           // Tool rating
            outfile << ";";

            if (type.compare("tool") == 0 ||
                type.compare("ship") == 0) {    // outputs tool's obstacle type
              item->get_obstacle_type(ob_type); // outputs 'boulder', 'tree',..
              outfile << ob_type;
              outfile << ";";
            }
          }

          if (type.compare("ship") == 0) { // outputs 'water'
            item->get_terrain_type(ob_type);
            outfile << ob_type;
            outfile << ";";
          }

          if (type.compare("clue") == 0) { // outputs clue message
            item->get_message(
                message); // outputs 'the diamonds are x grovnicks north...',..
            outfile << message;
            outfile << ";";
          } else if (type.compare("obstacle") !=
                     0) { // outputs whiffles for all classes except 'clue'and
                          // 'obstacle'
            if (type.compare("treasure") == 0)
              outfile << item->get_whiffles();
            else
              outfile << item->get_cost();
            if (type.compare("diamond") == 0)
              outfile << item->get_whiffles();
            else
              outfile << item->get_cost();
            outfile << ";";
          }

          outfile << i << ";" << j; // outputs coordinates
          outfile << "\n";
        }
      }
    }
  }

  outfile.close();
}

// THIS IS THE START OF A COMPLICATED SCROLLING FUNCTION
// it's not that complicated but worth taking a look
void Map::scroll_function(int &starty, int &startx, int ypos, int xpos) {

  int centerx, endx, centery, endy, rows, cols;

  getmaxyx(viewport, rows, cols); // find center of viewport
  centery = rows / 2;
  centerx = cols / 2;

  // finds beg and end of x-axis of viewport
  startx = xpos - centerx;
  endx = xpos + centerx;

  // makes sure we don't go out of bounds
  if (cols >= KSIZE)
    startx = 0;
  else if (startx < 0)
    startx = 0;
  else if (endx > KSIZE)
    startx = KSIZE - cols;

  // finds beg and end of y-axis of viewport
  starty = ypos - centery;
  endy = ypos + centery;

  // makes sure we don't go out of bounds
  if (rows >= KSIZE)
    starty = 0;
  else if (starty < 0)
    starty = 0;
  else if (endy > KSIZE) {
    starty = KSIZE - rows;
  }
  // END OF SCROLLING MECHANISM
}

void Map::print_options(WINDOW *gm, int row, int col, bool has_ship) {
  // shows which actions are currently available to the player and which keys to
  // hit to take those actions

  int r = 9;
  int c = 1;
  int num = 1;
  mvwprintw(gm, r++, c, " Options: ");

  if (row > 0 && (frupal[row - 1][col].square == MEADOW ||
                  frupal[row - 1][col].square == SWAMP ||
                  (frupal[row - 1][col].square == WATER && has_ship == true)))
    mvwprintw(gm, r++, c, " %d) North ", num++);
  if (col < KSIZE - 1 &&
      (frupal[row][col + 1].square == MEADOW ||
       frupal[row][col + 1].square == SWAMP ||
       (frupal[row][col + 1].square == WATER && has_ship == true)))
    mvwprintw(gm, r++, c, " %d) East ", num++);
  if (row < KSIZE - 1 &&
      (frupal[row + 1][col].square == MEADOW ||
       frupal[row + 1][col].square == SWAMP ||
       (frupal[row + 1][col].square == WATER && has_ship == true)))
    mvwprintw(gm, r++, c, " %d) South", num++);
  if (col > 0 && (frupal[row][col - 1].square == MEADOW ||
                  frupal[row][col - 1].square == SWAMP ||
                  (frupal[row][col - 1].square == WATER && has_ship == true)))
    mvwprintw(gm, r++, c, " %d) West", num++);

  mvwprintw(gm, r++, c,
            " c) Check About"); // added in for new check_around function
}

void Map::print_current_grovnick(WINDOW *gm, int row, int col) {
  int r = 3;
  string type;

  if (frupal[row][col].square == MEADOW)
    type = "MEADOW";
  else if (frupal[row][col].square == SWAMP)
    type = "SWAMP";
  else if (frupal[row][col].square == WATER)
    type = "WATER";
  else
    type = "restricted area";

  mvwprintw(gm, r++, 1, "  Grovnick Terrain: ");
  mvwprintw(gm, r++, 1, "  ");
  wprintw(gm, type.data());
}
