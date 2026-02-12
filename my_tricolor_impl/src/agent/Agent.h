#pragma once
#include "../board/Board.h"

class Agent{
public:

bool is_bot;

Agent(bool is_bot);

virtual ~Agent() = default;

virtual Action choose_action(Board& board, vector<Action>& actions) = 0;

private:
};