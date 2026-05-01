#include "Board.h"
#include "Zobrist.h"
#include <random>

using namespace std;

uint64_t zobrist_table[NUM_TILES][MAX_TILE_VALUE];
uint64_t zobrist_player;

void init_zobrist() {
    mt19937_64 rng(123456);

    for(int i = 0; i < NUM_TILES; i++){
        for(int j = 0; j < MAX_TILE_VALUE; j++){
            zobrist_table[i][j] = rng();
        }
    }

    zobrist_player = rng();
}

uint64_t compute_hash(BoardOpti& board) {
    uint64_t h = 0;

    for(int i = 0; i < 61; i++){
        int idx = zobrist_index(board.tiles[i]);
        h ^= zobrist_table[i][idx];
    }

    if(board.player_turn)
        h ^= zobrist_player;

    return h;
}