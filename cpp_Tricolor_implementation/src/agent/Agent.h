#pragma once
#include "../board/Board.h"

class Agent{
public:

bool is_bot;

Agent(bool is_bot);

virtual ~Agent() = default;

virtual Action choose_action(Board& board, vector<Action>& actions) = 0;

virtual ActionOpti choose_action(BoardOpti& board, vector<ActionOpti>& actions) = 0;

private:
};