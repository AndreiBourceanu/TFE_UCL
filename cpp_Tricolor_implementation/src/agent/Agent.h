#pragma once
#include "../board/Board.h"

class Agent{
public:

bool is_bot;

//Agent() : is_bot(true) {}
Agent(bool is_bot);

virtual ~Agent() = default;

virtual Action choose_action(Board& board, vector<Action>& actions){
    throw std::runtime_error("Not implemented");
}

virtual ActionOpti choose_action(BoardOpti& board, vector<ActionOpti>& actions){
    throw std::runtime_error("Not implemented");
}

virtual ActionOpti choose_action(BoardOpti& board, int player){
    throw std::runtime_error("Not implemented");
}

private:
};