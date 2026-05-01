#include <iostream>
#include <random>
#include <chrono>
#include <algorithm>
#include <vector>
#include <random>
#include "AgentAlphaBeta.h"
#include "Heuristics.h"

using namespace std;

int INITIAL_DEPTH=0;

AgentAlphaBeta::AgentAlphaBeta(Heuristic h): Agent(true), heuristic(h) {}

int AgentAlphaBeta::alpha_beta(BoardOpti& board, int depth, int alpha, int beta, int initital_player, bool is_max, int player, chrono::high_resolution_clock::time_point deadline){

    if (chrono::high_resolution_clock::now() >= deadline) {
        return heuristic(board, initital_player);
    }

    vector<ActionOpti> actions = board.get_actions(player);

    if (actions.size() == 0) {
        // current player cannot move, then he loses
        if (player == initital_player)
            return INT32_MIN - (depth - INITIAL_DEPTH);  // we lose
        else
            return INT32_MAX - (INITIAL_DEPTH - depth);  // we win
    }

    if(depth == 0){
        return heuristic(board, initital_player);
    }

    // Precompute heuristic values for sorting
    vector<pair<int, ActionOpti>> scored_actions;

    for (ActionOpti action : actions) {
        BoardOpti result_board = board.result_state_after_action(player, action);
        int score = heuristic(result_board, initital_player);
        scored_actions.emplace_back(score, action);
    }

    // Sort actions
    if (is_max) {
        sort(scored_actions.begin(), scored_actions.end(),
            [](const auto& a, const auto& b) {
                return a.first > b.first; // descending
            });
    } else {
        sort(scored_actions.begin(), scored_actions.end(),
            [](const auto& a, const auto& b) {
                return a.first < b.first; // ascending
            });
    }

    if(is_max){
        int value = INT32_MIN;
        for(pair<int, ActionOpti>& p : scored_actions){
            BoardOpti result_board = board.result_state_after_action(player, p.second);
            value = max(value, alpha_beta(result_board, depth - 1, alpha, beta, initital_player, false, (player+1)%2, deadline));
            alpha = max(alpha, value);

            if(alpha >= beta) break;
        }
        return value;
    }
    else{
        int value = INT32_MAX;
        for(pair<int, ActionOpti>& p : scored_actions){
            BoardOpti result_board = board.result_state_after_action(player, p.second);
            value = min(value, alpha_beta(result_board, depth - 1, alpha, beta, initital_player, true, (player+1)%2, deadline));
            beta = min(beta, value);

            if(beta <= alpha) break;
        }
        return value;
    }
}

ActionOpti AgentAlphaBeta::choose_action(BoardOpti& board, int player){
    auto start_turn = chrono::high_resolution_clock::now();
    auto deadline = start_turn + chrono::milliseconds(1000);

    vector<ActionOpti> actions = board.get_actions(player);
    ActionOpti best_action = actions[0];

    // Precompute heuristic values for sorting
    vector<pair<int, ActionOpti>> scored_actions;

    for (ActionOpti action : actions) {
        BoardOpti result_board = board.result_state_after_action(player, action);
        int score;
        if(result_board.get_actions((player+1)%2).size() == 0){
            score = INT32_MAX;
        }
        else{
            score = heuristic(result_board, player);
        }
        scored_actions.emplace_back(score, action);
    }
    sort(scored_actions.begin(), scored_actions.end(),
            [](const auto& a, const auto& b) {
                return a.first > b.first;
            });

    int depth;

    std::random_device rd;
    std::mt19937 rng(rd());

    for(depth = 1; ; depth++) {

        INITIAL_DEPTH = depth;

        if (chrono::high_resolution_clock::now() >= deadline)
            break;

        int best_utility = INT32_MIN;
        //ActionOpti depth_best_action = actions[0];

        bool ran_out_of_time = false;

        vector<ActionOpti> best_actions;
        best_actions.push_back(actions[0]);

        for(ActionOpti action : actions){

            if (chrono::high_resolution_clock::now() >= deadline){
                ran_out_of_time = true;
                break;
            }

            BoardOpti result_board = board.result_state_after_action(player, action);

            int utility = alpha_beta(result_board, depth - 1, INT32_MIN, INT32_MAX, player, false, (player+1)%2, deadline);

            if(utility > best_utility){
                best_utility = utility;
                //depth_best_action = action;
                best_actions.clear();
                best_actions.push_back(action);
            }
            else if(utility == best_utility){
                best_actions.push_back(action);
            }
        }

        if(!ran_out_of_time){
            //best_action = depth_best_action;
            std::uniform_int_distribution<size_t> dist(0, best_actions.size() - 1);
            best_action = best_actions[dist((rng))];
        }
    }
    return best_action;
}