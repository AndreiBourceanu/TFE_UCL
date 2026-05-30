#pragma once
#include <cstdint>

constexpr int NUM_TILES = 61;
constexpr int MAX_STACK = 18;
constexpr int MAX_TILE_VALUE = (MAX_STACK + 1) * (MAX_STACK + 1) * 2; // 722

extern uint64_t zobrist_table[NUM_TILES][MAX_TILE_VALUE];
extern uint64_t zobrist_player;

void init_zobrist();

inline int zobrist_index(uint16_t tile) {
    int white = tile & 0b11111;
    int black = (tile >> 5) & 0b11111;
    int owner = (tile >> 10) & 1;

    return (white * (MAX_STACK + 1) + black) * 2 + owner;
}

uint64_t compute_hash(BoardOpti& board);