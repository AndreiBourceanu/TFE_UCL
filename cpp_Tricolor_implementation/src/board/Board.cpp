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

/*
int main(){
    Board b1(5);
    
    for(int i = 0; i < b1.board.size(); i++){
        for(int j = 0; j < b1.board[i].size(); j++){
            cout<<b1.board[i][j].color<<' '<<b1.board[i][j].controls_p1<<' '<<b1.board[i][j].p1_pieces<<' '<<b1.board[i][j].p2_pieces<<' '<<endl;
        }
    }
    
   int i = 4, j = 8;
   for(int k = 0; k < 6; k++){
        if(b1.board[i][j].neighbours[k] != NULL){
        HexTile* h = b1.board[i][j].neighbours[k];
        cout<<h->color<<' '<<h->player<<' '<<h->pieces[0]<<' '<<h->pieces[1]<<' '<<endl;
        }
   }
   vector<Action> p1_actions = b1.actions(0);
   for(Action ac : p1_actions){
    cout<<ac.from.x<<' '<<ac.from.y<<' '<<ac.direction<<' '<<ac.pieces_moved<<' '<<ac.step<<endl;
   }
   return 0;
}
*/