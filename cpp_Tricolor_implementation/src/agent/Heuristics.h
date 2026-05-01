#pragma once
#include "../board/Board.h"

extern const int first_3_power[4];
extern const int remaining_power[4];

int max_compact_power(BoardOpti& board, int player);

int max_power_less_stacks(BoardOpti& board, int player);