#pragma once

#include <vector>
#include <iostream>
using namespace std;

struct Position{
int x, y;

bool is_equal(Position position){
    return x == position.x && y == position.y;
}

};

class HexTile{
public:
// coordinates in the matrix
Position position;
// 'B', 'W' or 'R' 
char color;
// 0 = p1, 1 = p2
int pieces[2];
// nr of pieces of player 1
//int p1_pieces;
// nr of pieces of player 2
//int p2_pieces;
// 0 = white (down), 1 = black (black), 2 = neutral
int player;
// all neighbours of the hex tile
// left, right, upleft, upright, downleft, downright
HexTile* neighbours[6];
HexTile();
private:
};

bool hex_is_on_board(vector<vector<HexTile>> &board, int i, int j);

// board location:
// 0 -> before middle; 1 -> middle; 2 -> after middle 
void set_neighbours(vector<vector<HexTile>> &board, int i, int j, int board_location);

void initial_pieces_setup(vector<vector<HexTile>> &board, int pieces_lines);

int color_to_power(char color);

HexTile* get_hextile_from_direction_and_steps(HexTile* current_tile, int direction, int steps);

class Action{
public:
// 0 = white (down), 1 = black (up)
Position from;
Position to;
// 0 = left, 1 = right, 2 = upleft, 3 = upright, 4 = downleft, 5 = downright
int direction;
// from 1 to 3
int step;
// not all ally pieces have to be moved (dislocation)
int friend_pieces_moved;
int opponent_pieces_moved;
bool is_imprisonment_or_capture = false;
Action();
Action(Position from, Position to, int direction, int step, int friend_pieces_moved, int opponent_pieces_moved, int is_imprisonment_or_capture);

private:
};

class Board{
public:
int size;
int pieces_lines;
vector<vector<HexTile>> board;
Board();
Board(int size);
// get the vector of actions for the current player
vector<Action> actions(int player);

void execute_action(int player, Action action);
private:
};


// Optimized implementation using bit representation

struct ActionOpti{
    int from;
    int to;
    int pieces_moved;
    bool is_imprisonment_or_capture;

    ActionOpti(int from, int to, int pieces_moved, bool is_imprisonment_or_capture);

    ActionOpti();
};

struct BoardOpti{
    // 61 tiles in the original board (length 5 of a hex side)
    // The tiles are counted from left to right and up to down
    // There are 18 white pieces and 18 black pieces, so there will be at most 36 on one tile
    // I need to know the nr of white and black pieces, as well as who controls it.
    // I use 16 bits integer for the following info:
    // . . . . . . | . . . . . | . . . . .
    //    owner    |   black   |   white
    // (from right to left):
    // first 5 bits = nr of white pieces
    // next 5 bits = nr of black pieces
    // rest of bits = owner (0 = white, 1 = black, 2 = neutral)
    uint16_t tiles[61];

    // 0 -> white, 1 -> black
    bool player_turn;

    uint64_t hash;

    BoardOpti();

    int get_player(int tile);

    int get_white_pieces(int tile);

    int get_black_pieces(int tile);

    int get_pieces(int player, int tile);

    void add_white_pieces(int tile, int nr_pieces);
    
    void add_black_pieces(int tile, int nr_pieces);

    void add_pieces(int tile, int nr_pieces, int player);

    void set_owner(int tile, int player);

    // player 0 = white, player 1 = black
    vector<ActionOpti> get_actions(int player);

    void execute_action(int player, ActionOpti& action);

    BoardOpti result_state_after_action(int player, ActionOpti& action);
};

// updates the hash value of a board with the action
void update_zobrist_hash(BoardOpti& board, const ActionOpti& action);


// Hard-coded colors (white = 1, black = 2, red = 3, like the power bonus)
extern const int colors_opti[61];

// Hard-coded neighbours for each tile (each tile has 6 neighbors)
// if the neighbour in that direction doesn't exist, then it's -1
// left, right, upleft, upright, downleft, downright
// e.g. neighbours_opti[3][4] represents the neighbour at downleft of 4th square 
extern const int neighbours_opti[61][6];

extern const vector<vector<int>> position_to_tile;

extern const vector<Position> tile_to_position;