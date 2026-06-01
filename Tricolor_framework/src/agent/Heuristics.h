#pragma once
#include "../board/Board.h"

extern const int first_3_power[4];
extern const int remaining_power[4];

// Evaluation function 1
int max_compact_power(BoardOpti& board, int player);

// Evaluation function 2
int max_power_less_stacks(BoardOpti& board, int player);