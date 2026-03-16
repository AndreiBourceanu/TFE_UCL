#include "Board.h"
#include <iostream>

using namespace std;

bool hex_is_on_board(vector<vector<HexTile>> &board, int i, int j){
    return i >= 0 && i < board.size() && j >= 0 && j < board[i].size();
}

// board location:
// 0 -> before middle; 1 -> middle; 2 -> after middle 
void set_neighbours(vector<vector<HexTile>> &board, int i, int j, int board_location){
    // left
    // right
    // upleft
    // upright
    // downleft
    // downright
    int x[] = {0, 0, -1, -1, 1, 1};
    vector<int> y;
    if(board_location == 0){
        vector<int> y1{-1, 1, -1, 0, 0, 1};
        y = y1;
    }
    else if(board_location == 1){
        vector<int> y2{-1, 1, -1, 0, -1, 0};
        y = y2;
    }
    else{
        vector<int> y3{-1, 1, 0, 1, -1, 0};
        y = y3;
    }
    for(int k = 0; k < 6; k++){
        if(hex_is_on_board(board, i + x[k], j + y[k]))
            board[i][j].neighbours[k] = &board[i + x[k]][j + y[k]];
        else{
            board[i][j].neighbours[k] = NULL;
        }
    }
}

HexTile::HexTile(){
    position = Position{-1, -1};
    color = 'B';
    pieces[0] = pieces[1] = 0;
    player = 2;
    for(int i = 0; i < 6; i++){
        neighbours[i] = NULL;
    }
}

void initial_pieces_setup(vector<vector<HexTile>> &board, int pieces_lines){
    for(int i = 0; i < pieces_lines; i++){
        for(int j = 0; j < board[i].size(); j++){
            board[i][j].pieces[1] = 1;
            board[i][j].pieces[0] = 0;
            board[i][j].player = 1;
        }
    }
    for(int i = board.size() - 1; i >= board.size() - pieces_lines; i--){
        for(int j = 0; j < board[i].size(); j++){
            board[i][j].pieces[0] = 1;
            board[i][j].pieces[1] = 0;
            board[i][j].player = 0;
        }
    }
    for(int i = 0; i < board.size(); i++){
        for(int j = 0; j <board[i].size(); j++){
            board[i][j].position.x = i;
            board[i][j].position.y = j;
        }
    }
}

int color_to_power(char color){
    if(color == 'W') return 1;
    if(color == 'B') return 2;
    return 3;
}

HexTile* get_hextile_from_direction_and_steps(HexTile* current_tile, int direction, int steps){
    for(int i = 0; i < steps; i++){
        current_tile = current_tile->neighbours[direction];
    }
    return current_tile;
}

Action::Action(Position from, Position to, int direction, int step, int friend_pieces_moved, int opponent_pieces_moved, int is_imprisonment_or_capture){
    this->from = from;
    this->to = to;
    this->direction = direction;
    this->step = step;
    this->friend_pieces_moved = friend_pieces_moved;
    this->opponent_pieces_moved = opponent_pieces_moved;
    this->is_imprisonment_or_capture = is_imprisonment_or_capture;
}

Action::Action(){}

vector<Action> Board::actions(int player){
    vector<Action> actions;
    // first check if a capture or imprisonment is possible
    bool has_to_at_least_imprison = false;
    // iterate over the board
    for(int i = 0; i < board.size(); i++){
        for(int j = 0; j < board[i].size(); j++){
            // check if the hex is controlled by current player
            if(board[i][j].player == player){
                for(int nr_of_pieces_to_move = 1; nr_of_pieces_to_move <= board[i][j].pieces[player] + board[i][j].pieces[(player + 1) % 2]; nr_of_pieces_to_move++){
                    int max_distance = min(3, min(nr_of_pieces_to_move, board[i][j].pieces[player]));
                    int power = max_distance * color_to_power(board[i][j].color);
                    for(int k = 0; k < 6; k++){
                        HexTile current_hex_tile = board[i][j];
                        for(int l = 0; l < max_distance; l++){
                            // if no more neighbour in that direction stop
                            if(current_hex_tile.neighbours[k] == NULL) break;
                            // get the next neighbour in that direction
                            current_hex_tile = *current_hex_tile.neighbours[k];
                            // if friendly piece, stop, because you can't jump over pieces
                            if(current_hex_tile.player == player) break;
                            // if no pieces at all, continue
                            if(current_hex_tile.player == 2) continue;
                            // get the power of opponent
                            int opponent_power = min(3, current_hex_tile.pieces[(player + 1) % 2]) * color_to_power(current_hex_tile.color);
                            //check if current player can at least imprison the opponent
                            if(power > opponent_power){
                                // set to true for priority of moves
                                has_to_at_least_imprison = true;
                                // add the action
                                actions.push_back(Action(Position{i, j}, current_hex_tile.position, k, l + 1, min(nr_of_pieces_to_move, board[i][j].pieces[player]), nr_of_pieces_to_move - min(nr_of_pieces_to_move, board[i][j].pieces[player]), true));
                            }
                            // stop because you can't jump over pieces
                            break;
                        }
                    }
                }
            }
        }
    }
    // if no priorities, add all possible actions
    if(!has_to_at_least_imprison){
        // iterate over the board
        for(int i = 0; i < board.size(); i++){
            for(int j = 0; j < board[i].size(); j++){
                // check if the hex is controlled by current player
                if(board[i][j].player == player){
                    for(int nr_of_pieces_to_move = 1; nr_of_pieces_to_move <= board[i][j].pieces[player] + board[i][j].pieces[(player + 1) % 2]; nr_of_pieces_to_move++){
                        int max_distance = min(3, min(nr_of_pieces_to_move, board[i][j].pieces[player]));
                        int power = max_distance * color_to_power(board[i][j].color);
                        for(int k = 0; k < 6; k++){
                            HexTile current_hex_tile = board[i][j];
                            for(int l = 0; l < max_distance; l++){
                                // if no more neighbour in that direction stop
                                if(current_hex_tile.neighbours[k] == NULL) break;
                                // get the next neighbour in that direction
                                current_hex_tile = *current_hex_tile.neighbours[k];
                                // if opponent piece, stop, because you can't jump over pieces
                                if(current_hex_tile.player == (player + 1) % 2) break;
                                // add the action (might change the class)
                                actions.push_back(Action(Position{i, j}, current_hex_tile.position, k, l + 1, min(nr_of_pieces_to_move, board[i][j].pieces[player]), nr_of_pieces_to_move - min(nr_of_pieces_to_move, board[i][j].pieces[player]), false));
                                // stop because you can't jump over pieces
                                if(current_hex_tile.player != 2) break;
                            }
                        }
                    }
                }
            }
        }
    }
    return actions;
}

void Board::execute_action(int player, Action action){
    int x = action.from.x;
    int y = action.from.y;
    HexTile* from = &board[x][y];
    HexTile* to = get_hextile_from_direction_and_steps(from, action.direction, action.step);
    // first know if the action is a capture
    bool is_capture;
    is_capture = color_to_power(from->color) * min(3, action.friend_pieces_moved) > 2 * color_to_power(to->color) * min(3, to->pieces[to->player]);
    if(is_capture && action.is_imprisonment_or_capture){
        to->pieces[to->player] = 0;
    }
    // always remove `moved` pieces from `from`
    from->pieces[player] -= action.friend_pieces_moved;
    from->pieces[(player + 1) % 2] -= action.opponent_pieces_moved;
    // always add the `moved` pieces to `to`
    to->pieces[player] += action.friend_pieces_moved;
    to->pieces[(player + 1) % 2] += action.opponent_pieces_moved;
    
    // change the occupation of the tile to player from `from`
    to->player = from->player;
    if(from->pieces[player] == 0){
        if(from->pieces[(player + 1) % 2] == 0){
            from->player = 2;
        }
        else{
            from->player = (player + 1) % 2;
        }
    }
}

Board::Board(int size){
    this->size = size;
    pieces_lines = size - 2;
    int lines = size * 2 - 1;
    // board
    board.resize(lines);
    // allocate columns sizes
    for(int i = 0; i < lines; i++){
        int diff = abs(size - 1 - i);
        int cols = lines - diff;
        board[i].resize(cols);
    }
    // order: black -> white -> red
    char nr_to_color[3] = {'B', 'W', 'R'};
    int curr_color = 0;
    // first line
    for(int j = 0; j < board[0].size(); j++){
        board[0][j].color = nr_to_color[curr_color];
        curr_color = (curr_color + 1) % 3;
        set_neighbours(board, 0, j, 0);
    }
    // lines: 1 -> lines / 2
    for(int i = 1; i < lines / 2; i++){
        for(int j = 0; j < board[i].size(); j++){
            int col_hint;
            if(j + 1 < board[i-1].size()){
                col_hint = 1;
            }
            else{
                col_hint = -2;
            }
            board[i][j].color = board[i-1][j+col_hint].color;
            set_neighbours(board, i, j, 0);
        }
    }
    // line: lines / 2
    int i = lines / 2;
    for(int j = 0; j < board[i].size(); j++){
        int col_hint;
        if(j + 1 < board[i-1].size()){
            col_hint = 1;
        }
        else{
            col_hint = -2;
        }
        board[i][j].color = board[i-1][j+col_hint].color;
        set_neighbours(board, i, j, 1);
    }
    // lines: lines / 2 + 1 -> lines
    for(int i = lines / 2 + 1; i < lines; i++){
        for(int j = 0; j < board[i].size(); j++){
            int col_hint;
            if(j + 2 < board[i-1].size()){
                col_hint = 2;
            }
            else{
                col_hint = -1;
            }
            board[i][j].color = board[i-1][j+col_hint].color;
            set_neighbours(board, i, j, 2);
        }
    }
    initial_pieces_setup(board, pieces_lines);
}

Board::Board(){
    Board(5);
}

// Optimized implementation

const int colors_opti[61] = {
        2, 1, 3, 2, 1,
       1, 3, 2, 1, 3, 2,
      3, 2, 1, 3, 2, 1, 3,
     2, 1, 3, 2, 1, 3, 2, 1,
    1, 3, 2, 1, 3, 2, 1, 3, 2,
     2, 1, 3, 2, 1, 3, 2, 1,
      3, 2, 1, 3, 2, 1, 3,
       1, 3, 2, 1, 3, 2,
        2, 1, 3, 2, 1
};

// left, right, upleft, upright, downleft, downright
const int neighbours_opti[61][6] = {
        {-1, 1, -1, -1, 5, 6}, {0, 2, -1, -1, 6, 7}, {1, 3, -1, -1, 7, 8}, {2, 4, -1, -1, 8, 9}, {3, -1, -1, -1, 9, 10},
       {-1, 6, -1, 0, 11, 12}, {5, 7, 0, 1, 12, 13}, {6, 8, 1, 2, 13, 14}, {7, 9, 2, 3, 14, 15}, {8, 10, 3, 4, 15, 16}, {9, -1, 4, -1, 16, 17},
      {-1, 12, -1, 5, 18, 19}, {11, 13, 5, 6, 19, 20}, {12, 14, 6, 7, 20, 21}, {13, 15, 7, 8, 21, 22}, {14, 16, 8, 9, 22, 23}, {15, 17, 9, 10, 23, 24}, {16, -1, 10, -1, 24, 25},
     {-1, 19, -1, 11, 26, 27}, {18, 20, 11, 12, 27, 28}, {19, 21, 12, 13, 28, 29}, {20, 22, 13, 14, 29, 30}, {21, 23, 14, 15, 30, 31}, {22, 24, 15, 16, 31, 32}, {23, 25, 16, 17, 32, 33}, {24, -1, 17, -1, 33, 34},
    {-1, 27, -1, 18, -1, 35}, {26, 28, 18, 19, 35, 36}, {27, 29, 19, 20, 36, 37}, {28, 30, 20, 21, 37, 38}, {29, 31, 21, 22, 38, 39}, {30, 32, 22, 23, 39, 40}, {31, 33, 23, 24, 40, 41}, {32, 34, 24, 25, 41, 42}, {33, -1, 25, -1, 42, -1},
     {-1, 36, 26, 27, -1, 43}, {35, 37, 27, 28, 43, 44}, {36, 38, 28, 29, 44, 45}, {37, 39, 29, 30, 45, 46}, {38, 40, 30, 31, 46, 47}, {39, 41, 31, 32, 47, 48}, {40, 42, 32, 33, 48, 49}, {41, -1, 33, 34, 49, -1},
      {-1, 44, 35, 36, -1, 50}, {43, 45, 36, 37, 50, 51}, {44, 46, 37, 38, 51, 52}, {45, 47, 38, 39, 52, 53}, {46, 48, 39, 40, 53, 54}, {47, 49, 40, 41, 54, 55}, {48, -1, 41, 42, 55, -1},
       {-1, 51, 43, 44, -1, 56}, {50, 52, 44, 45, 56, 57}, {51, 53, 45, 46, 57, 58}, {52, 54, 46, 47, 58, 59}, {53, 55, 47, 48, 59, 60}, {54, -1, 48, 49, 60, -1},
        {-1, 57, 50, 51, -1, -1}, {56, 58, 51, 52, -1, -1}, {57, 59, 52, 53, -1, -1}, {58, 60, 53, 54, -1, -1}, {59, -1, 54, 55, -1, -1}
};

BoardOpti::BoardOpti(){
    // initialize board with standard game board
    for(uint16_t tile = 0; tile < 18; tile++){
        tiles[tile] = 0b10000100000;
    }
    for(uint16_t tile = 18; tile < 43; tile++){
        tiles[tile] = 0b100000000000;
    }
    for(uint16_t tile = 43; tile < 61; tile++){
        tiles[tile] = 0b1;
    }
}

int BoardOpti::get_player(int tile){
    // shift 10 bits to get player's turn
    return tiles[tile] >> 10;
}

int BoardOpti::get_white_pieces(int tile){
    // get the 5 first bits
    return tiles[tile] & 0b11111;
}

int BoardOpti::get_black_pieces(int tile){
    // get the 5 bits after the first 5 bits
    return (tiles[tile] >> 5) & 0b11111;
}

int BoardOpti::get_pieces(int player, int tile){
    // return the number of pieces of the player
    if(player == 0) return get_white_pieces(tile);
    return get_black_pieces(tile);
}

void BoardOpti::add_white_pieces(int tile, int nr_pieces){
    // white is the rightmost 5 bits so just add
    tiles[tile] += nr_pieces;
}

void BoardOpti::add_black_pieces(int tile, int nr_pieces){
    // rebuild the same tile but with new nr of black pieces
    tiles[tile] = (get_player(tile) << 10) | ((get_black_pieces(tile) + nr_pieces) << 5) | get_white_pieces(tile);
}

void BoardOpti::add_pieces(int tile, int nr_pieces, int player){
    if(player == 0) add_white_pieces(tile, nr_pieces);
    else add_black_pieces(tile, nr_pieces);
}

void BoardOpti::set_owner(int tile, int player){
    tiles[tile] = (player << 10) | (get_black_pieces(tile) << 5) | get_white_pieces(tile);
}

ActionOpti::ActionOpti(int from, int to, int pieces_moved, bool is_imprisonment_or_capture){
    this->from = from;
    this->to = to;
    this->pieces_moved = pieces_moved;
    this->is_imprisonment_or_capture = is_imprisonment_or_capture;
}

ActionOpti::ActionOpti(){}

vector<ActionOpti> BoardOpti::get_actions(int player){
    vector<ActionOpti> actions;
    // first, check if a capture or imprisonment is possible
    // and enforce it in case it is
    bool has_to_at_least_imprison = false;
    // iterate over the board
    for(int tile = 0; tile < 61; tile++){
        // check if the hex is controlled by current player
        if(get_player(tile) == player){
            for(int nr_of_pieces_to_move = 1; nr_of_pieces_to_move <= get_pieces(player, tile) + get_pieces((player+1)%2, tile); nr_of_pieces_to_move++){
                int max_distance = min(3, min(nr_of_pieces_to_move, get_pieces(player, tile)));
                int power = max_distance * colors_opti[tile];
                for(int direction = 0; direction < 6; direction++){
                    int current_hex_tile = tile;
                    for(int distance = 0; distance < max_distance; distance++){
                        // if no more neighbour in that direction stop
                        if(neighbours_opti[current_hex_tile][direction] == -1) break;
                        // get the next neighbour in that direction
                        current_hex_tile = neighbours_opti[current_hex_tile][direction];
                        // if the opponent is on the tile
                        if(get_player(current_hex_tile) == (player+1)%2){
                            // get the power of opponent
                            int opponent_power = min(3, get_pieces((player+1)%2, current_hex_tile)) * colors_opti[current_hex_tile];
                            if(power > opponent_power){
                                if(!has_to_at_least_imprison){
                                    has_to_at_least_imprison = true;
                                    actions.clear();
                                }
                                actions.push_back(ActionOpti(tile, current_hex_tile, nr_of_pieces_to_move, true));
                            }
                            break;
                        }
                        else{
                            if(!has_to_at_least_imprison){
                                actions.push_back(ActionOpti(tile, current_hex_tile, nr_of_pieces_to_move, false));
                            }
                            // if someone is there you can't continue
                            if(get_player(current_hex_tile) != 2) break;
                        }
                    }
                }
            }
        }
    }
    return actions;
}

void BoardOpti::execute_action(int player, ActionOpti action){
    int opponent = (player+1)%2;
    int friend_pieces_moved = min(get_pieces(player, action.from), action.pieces_moved);
    int opponent_pieces_moved = action.pieces_moved - friend_pieces_moved;
    int friend_power = min(friend_pieces_moved, 3) * colors_opti[action.from];
    int opponent_power = min(get_pieces(opponent, action.to), 3) * colors_opti[action.to];

    // check if it's a capture
    if(get_player(action.to) == opponent && friend_power > opponent_power * 2){
        // if yes, set opponent pieces to 0 by adding the negative of his nr of pieces
        add_pieces(action.to, -get_pieces(opponent, action.to), opponent);
    }

    // always remove `moved` pieces from `from`
    add_pieces(action.from, -friend_pieces_moved, player);
    add_pieces(action.from, -opponent_pieces_moved, opponent);
    // always add the `moved` pieces to `to`
    add_pieces(action.to, friend_pieces_moved, player);
    add_pieces(action.to, opponent_pieces_moved, opponent);

    // change the owner of the `to` tile to `from` player
    set_owner(action.to, player);
    // set `from` tile to neutral if no pieces remaining
    if(get_pieces(player, action.from) == 0){
        if(get_pieces(opponent, action.from) == 0){
            set_owner(action.from, 2);
        }
        else{
            set_owner(action.from, opponent);
        }
    }
}