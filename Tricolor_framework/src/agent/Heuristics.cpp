#include "Heuristics.h"

int const first_3_power[4] = {-1, 1, 3, 5};
int const remaining_power[4] = {-1, 1, 2, 4};

// best position:
// most pieces on red, then black, then white (= maximal total power)
//
// reinforced positions (= better to have less owned tiles but more pieces on each tile = encourage less tiles)
//
// the score can be the total power - number of stacks (to prefer positions with same power but less stacks = more compact)
//
// I do the same with opponent but subtract from the score

int max_compact_power(BoardOpti& board, int player){
    int opponent = (player + 1) % 2;
    int utility = 0;
    int nr_of_pieces = 0; // 1st priority
    int max_power = 0; // 2nd priority
    // for each tile
    for(int tile = 0; tile < 61; tile++){
        // if tile is owned by player
        if(board.get_player(tile) == player){
            // add the stack power
            nr_of_pieces += board.get_pieces(player, tile);
            max_power += min(3, board.get_pieces(player, tile)) * first_3_power[colors_opti[tile]] + max(0, board.get_pieces(player, tile) - 3) * remaining_power[colors_opti[tile]];
            // subtract 1 because of an owned stack
            max_power--;
        }
        else if(board.get_player(tile) == opponent){
            // subtract the stack power of the opponent
            nr_of_pieces -= board.get_pieces(opponent, tile);
            max_power -= min(3, board.get_pieces(opponent, tile)) * first_3_power[colors_opti[tile]] + max(0, board.get_pieces(opponent, tile) - 3) * remaining_power[colors_opti[tile]];
            // add 1 because of an owned stack by the opponent
        }
    }
    utility = nr_of_pieces * 100000 + max_power;
    return utility;
}

// As many friendly pieces as possible and as little opponent pieces as possible
// max power, with as little stacks as possible

int MAX_POWER = 61 * 18 * 18 * 3 + 1; // if a player has 18 pieces on red

int max_power_less_stacks(BoardOpti& board, int player){
    int opponent = (player + 1) % 2;
    int nr_of_pieces = 0; // 1st priority
    int max_power = 0; // 2nd priority
    int utility = 0;
    // for each tile
    for(int tile = 0; tile < 61; tile++){
        // if tile is owned by player
        if(board.get_player(tile) == player){
            // add the stack power
            nr_of_pieces += board.get_pieces(player, tile);
            max_power += board.get_pieces(player, tile) * board.get_pieces(player, tile) * colors_opti[tile];
        }
        else if(board.get_player(tile) == opponent){
            // subtract the stack power of the opponent
            nr_of_pieces -= board.get_pieces(opponent, tile);
            max_power -= board.get_pieces(opponent, tile) * board.get_pieces(opponent, tile) * colors_opti[tile];
            // add 1 because of an owned stack by the opponent
        }
    }
    utility = nr_of_pieces * MAX_POWER + max_power;
    return utility;
}