#pragma once
#include "Agent.h"

class AgentHuman : public Agent{
public:

AgentHuman();

Action choose_action(Board& board, vector<Action>& actions) override;

ActionOpti choose_action(BoardOpti& board, vector<ActionOpti>& actions) override;

ActionOpti choose_action(BoardOpti& board, int player) override;

private:
};