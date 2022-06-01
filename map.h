/*  a re do that I'm making
    maybe it'll be accepted, maybe it won't
    should be loaded onto the hero branch
    November 17, 2020
*/

#include "item_tiles.h"
#include <string>
#define KSIZE 128     // defines kingdom size
#define NUMTERRAIN 11 // defines # of terrain files
#define NUMITEM 8     // defines # of item filesclass Item; // pre declaration

enum terrain { NONE, MEADOW, SWAMP, WATER, WALL, UNSEEN };

using namespace std;

struct grovnick {

  enum terrain square; // holds enum above ^^^
  bool viewed;         // bool if viewed or no
  Item *feature;       // this will be an item
};

class Map {

public:
  //  Map() =delete;
  Map(WINDOW *vp, WINDOW *gm);
  Map(WINDOW *vp, WINDOW *gm, char *inputfile);

  void update_display(int starty, int startx); // updates viewport
  int energy_cost(int y, int x, bool ship); // ret -1 if off map, wall, water,
                                            // 2-swamp, 1-meadow

  void look_around(int ypos, int xpos, bool binoculars);
  void scroll_function(int &starty, int &start, int ypos, int xpos);
  void print_options(WINDOW *gm, int row, int col, bool has_ship);
  void print_current_grovnick(WINDOW *gm, int row, int col);

  ///// ----  Item input/output/edit/print functions   ---- /////
  void create_item(char icon, int i, int j);
  void remove_item(int row, int col);
  bool has_item(int i, int j);
  void print_item(WINDOW *gm, int i, int j);
  void show_item(WINDOW *gm, int i, int j);
  void get_item(Item *&item, int i, int j);
  void edit_item_map();
  void save_item_map();
  void save_item(string file);

private:
  grovnick frupal[KSIZE][KSIZE]; // holds 2D array of structs

  WINDOW *viewport; // easy print to viewport
  WINDOW *gamemenu; // might not need?

  void load_terrain(string file); // loads a terrain file
  void load_map(string file);     // loads ALL the terrain files
  void load_items();

  string item_list[NUMITEM];
};
