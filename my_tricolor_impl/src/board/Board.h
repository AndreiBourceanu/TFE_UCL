#pragma once

#include <vector>
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