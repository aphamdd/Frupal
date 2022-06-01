// CS300 Fall Term 2020
// Group 7
// Kingdom of Frupal

#include "hero.h"
#include "map.h"
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <ncurses.h>
#include <string>
#define MENUWIDTH 35 // sets the menu width

using namespace std;

int startmenu();
void gameplay();
bool end_game_menu(Hero *hero);

int main(int argc, char **argv) {

  if (argc > 1) {
    cout << "please enter ./frupal\n";
    return 0;
  }

  initscr();
  if (LINES < 24 || COLS < 80) {
    char string[] =
        "Terminal is smaller than 80x24. Please resize your terminal!";
    int x = (COLS / 2) - (strlen(string) / 2);
    int y = LINES / 2;

    mvprintw(y, x, string);
    wborder(stdscr, '#', '#', '#', '#', '#', '#', '#', '#');
    refresh();
    getch();

    endwin();
    return 0;
  }

  noecho();              // doesn't print input char to screen
  keypad(stdscr, true);  // takes in the up/down/left/right
  nodelay(stdscr, true); // immediately takes in chars from input
  curs_set(0);           // doesn't print cursor to screen

  start_color();                            // colors are matched to enum #'s
  init_pair(1, COLOR_BLACK, COLOR_GREEN);   // MEADOW = 1
  init_pair(2, COLOR_BLACK, COLOR_MAGENTA); // SWAMP = 2
  init_pair(3, COLOR_BLACK, COLOR_BLUE);    // WATER = 3
  init_pair(4, COLOR_BLACK, COLOR_WHITE);   // WALLS = 4
  init_pair(5, COLOR_BLACK, COLOR_BLACK);   // UNSEEN = 5
  init_pair(6, COLOR_YELLOW, COLOR_RED);    // Hero
  init_pair(7, COLOR_WHITE, COLOR_CYAN);    // Royal Diamond

  //////////////////////////////////////////////////////////////////////////////
  // The primary control of game is move to the function below called gameplay
  //////////////////////////////////////////////////////////////////////////////

  if (startmenu() == 0) {
    erase();
    gameplay();
  }
  endwin();
  return 0;
}

//////////////////////////////////////////////////////////////////////////////
// startmenu has the start menu, cool scrolling highlightint thing.
// gameplay has the up/down/left/right WINDOWS thing
// end_game_menu has a little end game menu that needs improvement.
//////////////////////////////////////////////////////////////////////////////

int startmenu() {
  // This is start menu
  // Maybe add customized function?
  string option[2] = {"1.Start Game", "0.Exit"};
  int hightlight = 0;
  int choice; // get the input

  ///////////////////////////////////
  // for the title
  int widthX = 100;
  int heightY = 20;
  int posX = (COLS - widthX) / 2;
  int posY = (LINES - heightY) / 2;
  char draw = 'X';
  //////////////////////////////////

  while (true) {
    // window to draw the title FRUPAL
    WINDOW *mainTitle = newwin(heightY, widthX, posY, posX);

    ///////////////////////////////////////////////
    // Draw 'F'
    mvwvline(mainTitle, 2, 3, draw, 11);
    mvwhline(mainTitle, 2, 4, draw, 13);
    mvwvline(mainTitle, 3, 16, draw, 2);
    mvwhline(mainTitle, 4, 6, draw, 10);
    mvwvline(mainTitle, 5, 6, draw, 1);
    mvwhline(mainTitle, 6, 6, draw, 10);
    mvwvline(mainTitle, 6, 16, draw, 2);
    mvwhline(mainTitle, 8, 6, draw, 11);
    mvwvline(mainTitle, 9, 6, draw, 4);
    mvwhline(mainTitle, 12, 4, draw, 2);

    // Draw 'R'
    mvwvline(mainTitle, 2, 19, draw, 11);
    mvwhline(mainTitle, 2, 20, draw, 14);
    mvwvline(mainTitle, 3, 33, draw, 5);
    mvwhline(mainTitle, 8, 31, draw, 3);
    mvwvline(mainTitle, 9, 31, draw, 1);
    mvwhline(mainTitle, 10, 31, draw, 5);
    mvwvline(mainTitle, 11, 35, draw, 1);
    mvwhline(mainTitle, 12, 27, draw, 9);
    mvwvline(mainTitle, 9, 27, draw, 3);
    mvwhline(mainTitle, 8, 23, draw, 5);
    mvwvline(mainTitle, 9, 23, draw, 3);
    mvwhline(mainTitle, 12, 20, draw, 4);
    mvwhline(mainTitle, 4, 23, draw, 7);
    mvwvline(mainTitle, 5, 23, draw, 1);
    mvwhline(mainTitle, 6, 23, draw, 7);
    mvwvline(mainTitle, 5, 29, draw, 1);

    // Draw 'U'
    mvwvline(mainTitle, 2, 38, draw, 11);
    mvwhline(mainTitle, 2, 39, draw, 4);
    mvwvline(mainTitle, 3, 42, draw, 7);
    mvwhline(mainTitle, 10, 42, draw, 5);
    mvwvline(mainTitle, 3, 46, draw, 7);
    mvwhline(mainTitle, 2, 46, draw, 5);
    mvwvline(mainTitle, 3, 50, draw, 10);
    mvwhline(mainTitle, 12, 39, draw, 11);

    // Draw 'P'
    mvwvline(mainTitle, 2, 53, draw, 11);
    mvwhline(mainTitle, 2, 54, draw, 12);
    mvwvline(mainTitle, 3, 65, draw, 5);
    mvwhline(mainTitle, 8, 57, draw, 9);
    mvwvline(mainTitle, 9, 57, draw, 3);
    mvwhline(mainTitle, 12, 54, draw, 4);
    mvwhline(mainTitle, 4, 57, draw, 5);
    mvwhline(mainTitle, 5, 57, draw, 1);
    mvwhline(mainTitle, 6, 57, draw, 5);
    mvwhline(mainTitle, 5, 61, draw, 1);

    // Draw 'A'
    mvwvline(mainTitle, 2, 68, draw, 11);
    mvwhline(mainTitle, 2, 69, draw, 12);
    mvwvline(mainTitle, 3, 80, draw, 10);
    mvwhline(mainTitle, 12, 76, draw, 5);
    mvwvline(mainTitle, 9, 76, draw, 3);
    mvwhline(mainTitle, 8, 72, draw, 5);
    mvwvline(mainTitle, 9, 72, draw, 3);
    mvwhline(mainTitle, 12, 69, draw, 4);
    mvwhline(mainTitle, 4, 72, draw, 5);
    mvwhline(mainTitle, 5, 72, draw, 1);
    mvwhline(mainTitle, 6, 72, draw, 5);
    mvwhline(mainTitle, 5, 76, draw, 1);

    // Draw 'L'
    mvwvline(mainTitle, 2, 83, draw, 11);
    mvwhline(mainTitle, 2, 84, draw, 4);
    mvwvline(mainTitle, 3, 87, draw, 8);
    mvwhline(mainTitle, 10, 87, draw, 9);
    mvwhline(mainTitle, 11, 95, draw, 1);
    mvwhline(mainTitle, 12, 84, draw, 12);
    /////////////////////////////////////////////////////////

    WINDOW *mainMenu = newwin(7, COLS - 16, LINES - 13, 7);
    box(mainMenu, 0, 0);
    refresh();
    wrefresh(mainMenu);
    wrefresh(mainTitle);
    keypad(mainMenu, true);
    for (int i = 0; i < 2; ++i) {
      if (i == hightlight)
        wattron(mainMenu, A_REVERSE);
      // make sure it's on the center.
      // 12 is length of the "1.Start Game"
      mvwprintw(mainMenu, i + 1, (COLS - 16 - 12) / 2, option[i].c_str());
      wattroff(mainMenu, A_REVERSE);
    }
    choice = wgetch(mainMenu);
    switch (choice) {
    case 10: // 10 is the value of the key Enter
             // returns 1 quit game, 0 for start game
      return hightlight;
      break;
    case KEY_UP:
      --hightlight;
      if (hightlight < 0)
        hightlight = 0;
      break;
    case KEY_DOWN:
      ++hightlight;
      if (hightlight > 1)
        hightlight = 1;
      break;
    default:
      break;
    }
    clear();
    refresh();
  }
  endwin(); // ends window functionality
  return 0;
}

void gameplay() {
  // TODO if argc == 2, create Hero(viewport, in_

  Hero *hero;                  // hero object
  WINDOW *viewport, *gamemenu; // our 2 windows

  // figures out what the height and width of menu/viewport is
  int gmheight, gmwidth = MENUWIDTH, vpheight, vpwidth;
  vpheight = LINES;
  gmheight = LINES;
  vpwidth = COLS - gmwidth;

  // changes menu width if screen is really big.
  if (vpwidth > KSIZE) {
    vpwidth = KSIZE;
    gmwidth = COLS - vpwidth;
  }

  viewport =
      newwin(vpheight, vpwidth, 0, 0); // sets location/height/width of viewport
  gamemenu = newwin(gmheight, gmwidth, 0, vpwidth + 1); // same with menu
  wborder(gamemenu, '#', 0, ' ', ' ', '#', 0, '#',
          0); // should put just on left side

  hero = new Hero(viewport, gamemenu, gmwidth);
  int ch;
  refresh();              // starts the screen
  hero->update_display(); // prints the initial screen

  // this is the primary control of game.
  while ((ch = getch()) != 'q' && ch != 'Q') {
    switch (ch) {
    case KEY_LEFT: // move left
      hero->move_left();
      break;
    case KEY_RIGHT:
      hero->move_right();
      break;
    case KEY_UP:
      hero->move_up();
      break;
    case KEY_DOWN:
      hero->move_down();
      break;
    case 'c':
      hero->check_around();
      break;
    default:
      break;
    }
    // check_energy returns true if hero's 'dead' false otherwise
    if (hero->check_energy()) {
      if (end_game_menu(hero)) // returns true to keep playing, false otherwise
        continue;
      else
        break;
    }
  }
}

// end game menu.  adds 100 energy and returns true if player
// wants to continue the game, returns false to quit game.
// function is handled in gameplay function
bool end_game_menu(Hero *hero) {
  nodelay(stdscr, false);
  WINDOW *endgame;

  endgame = newwin(20, 50, (LINES - 20) / 2, (COLS - 50) / 2);

  wborder(endgame, '#', '#', '#', '#', '#', '#', '#', '#'); // should put border

  mvwprintw(endgame, 7, 20, "Game Over!");
  mvwprintw(endgame, 8, 20, "Hero died!");
  mvwprintw(endgame, 9, 14, "Press <ENTER> to exit.");
  wrefresh(endgame);

  int ch;
  ch = getch();
  while (ch != 10)
    ch = getch();

  // delete the game over window after they've chosen
  delwin(endgame);

  return false;
}
