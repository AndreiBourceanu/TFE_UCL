// Launch Tricolor game simulations between 2 agents and extract useful metrics

#include <iostream>
#include <fstream>
#include <filesystem>
#include <chrono>
#include <math.h>
#include <iomanip>
#include "game/Game.h"
#include "agent/AgentRandom.h"
#include "agent/AgentAlphaBeta.h"
#include "agent/AgentMCTS.h"
#include "agent/Heuristics.h"

using namespace std;
namespace fs = std::filesystem;

// default number of games simulated
int GAMES_PLAYED = 1000000;

// create an agent based on input
unique_ptr<Agent> create_agent(const string& name) {
    if (name == "random")
        return std::make_unique<AgentRandom>();
    else if (name == "alphabeta")
        return std::make_unique<AgentAlphaBeta>(max_power_less_stacks);
    else if (name == "mcts")
        return std::make_unique<AgentMCTS>(1000);
    else {
        cerr << "Unknown agent type: " << name << endl;
        exit(1);
    }
}

/*
- the first argument must be the name of the first Agent and Player 1 (White)
- the second argument must be the name of the second Agent and Player 2 (Black)
- the third argument must be the number of games played between the agents
agents: "random", "alphabeta", "mcts"
*/
int main(int argc, char* argv[])
{
    if(argc < 4){
        cout << "Usage: ./game_behaviour_metrics <player1> <player2> GAMES_PLAYED\n";
        return 1;
    }

    GAMES_PLAYED = stoi(argv[3]);

    string agent1 = argv[1];
    string agent2 = argv[2];
    string filepath = "csv_files/game_behaviour_" + agent1 + "_vs_" + agent2;
    fs::create_directories(filepath);

    // declaring all the metrics variables and output files

    int wins[3] = {0, 0, 0};

    ofstream fout_total_turns(filepath + "/total_turns.csv");
    fout_total_turns<<"total_turns\n";

    int total_turns = 0;
    int game_turns[GAMES_PLAYED];

    ofstream fout_total_repeated_states(filepath + "/total_repeated_states.csv");
    fout_total_repeated_states<<"total_repeated_states\n";

    int total_repeated_states = 0;

    ofstream fout_total_more_than_one_move(filepath + "/total_more_than_one_move.csv");
    fout_total_more_than_one_move<<"total_more_than_one_move\n";

    int total_moves = 0;
    int total_more_than_one_move = 0;

    ofstream fout_winrates(filepath + "/winrates.csv");
    fout_winrates<<"p1,p2,draw\n";

    ofstream fout_total_narrowness(filepath + "/total_narrowness.csv");
    fout_total_narrowness<<"total_narrowness\n";

    int total_narrowness = 0;

    ofstream fout_decisiveness(filepath + "/decisiveness.csv");
    fout_decisiveness<<"decisiveness\n";

    double earliest_turn_lead_change_game_percentage = 0;

    ofstream fout_total_lead_change_moves(filepath + "/total_lead_change_moves.csv");
    fout_total_lead_change_moves<<"total_lead_change_moves\n";

    int total_lead_change_moves = 0;

    ofstream fout_total_lead_change_actions(filepath + "/total_lead_change_actions.csv");
    fout_total_lead_change_actions<<"total_lead_change_actions\n";

    double total_lead_change_actions = 0;

    ofstream fout_total_drama(filepath + "/total_drama.csv");
    fout_total_drama<<"total_drama\n";

    double total_drama = 0;

    // game evaluations examples
    
    ofstream fout_game_evaluation_1(filepath + "/game_evaluation_1.csv");
    fout_game_evaluation_1<<"game_evaluation_1\n";

    ofstream fout_game_evaluation_2(filepath + "/game_evaluation_2.csv");
    fout_game_evaluation_2<<"game_evaluation_2\n";

    ofstream fout_game_evaluation_3(filepath + "/game_evaluation_3.csv");
    fout_game_evaluation_3<<"game_evaluation_3\n";

    //

    ofstream fout_total_occupied_tiles(filepath + "/total_occupied_tiles.csv");
    fout_total_occupied_tiles<<"total_occupied_tiles\n";

    int total_occupied_tiles = 0;

    ofstream fout_total_branches(filepath + "/total_branches.csv");
    fout_total_branches<<"total_branches\n";

    int total_branches = 0;

    ofstream fout_total_decision_factors(filepath + "/total_decision_factors.csv");
    fout_total_decision_factors<<"total_decision_factors\n";

    int total_decision_factors = 0;

    ofstream fout_total_distance(filepath + "/total_distance.csv");
    fout_total_distance<<"total_distance\n";

    double total_distance = 0;

    ofstream fout_total_number_of_pieces(filepath + "/total_number_of_pieces.csv");
    fout_total_number_of_pieces<<"total_number_of_pieces\n";

    int total_number_of_pieces = 0;

    // loop with all the simulated games

    for (int i = 0; i < GAMES_PLAYED; i++) {

        // create the agents
        array<unique_ptr<Agent>, 2> players;
        players[0] = create_agent(argv[1]);
        players[1] = create_agent(argv[2]);

        // create the game
        GameWithBehaviourMetrics game(move(players), 100);
        game.start();

        // update the metrics after each game

        wins[game.winner]++;

        total_turns += game.turn;
        fout_total_turns<<game.turn<<"\n";

        //

        game_turns[i] = game.turn;

        int current_repeated_states = 0;

        for(auto pair : game.hashed_state_freq){
            total_repeated_states += pair.second - 1;
            current_repeated_states += pair.second - 1;
        }
        fout_total_repeated_states<<current_repeated_states<<"\n";

        //

        total_moves += game.total_moves;
        total_more_than_one_move += game.more_than_one_move;

        fout_total_more_than_one_move << (double) total_more_than_one_move * 100 / total_moves << "\n";

        //

        total_narrowness += game.total_narrowness / game.total_moves;

        fout_total_narrowness << (double) game.total_narrowness / game.total_moves << "\n";

        //

        earliest_turn_lead_change_game_percentage += (double) game.earliest_turn_lead_change / game.turn;

        fout_decisiveness << (double) game.earliest_turn_lead_change * 100 / game.turn << "\n";

        //

        total_lead_change_moves += game.total_lead_change_moves / game.total_moves;

        fout_total_lead_change_moves << (double) game.total_lead_change_moves / game.total_moves << "\n";

        //

        total_lead_change_actions += (double) game.total_lead_change_actions / game.total_moves;

        fout_total_lead_change_actions << (double) game.total_lead_change_actions * 100 / game.total_moves << "\n";

        //

        total_drama += game.total_drama / game.total_moves;

        fout_total_drama << (double) game.total_drama / game.total_moves << "\n";
        
        if(i == 0){
            for(double val : game.positions_evalutaions){
                fout_game_evaluation_1 << val << "\n";
            }
        }
        if(i == 1){
            for(double val : game.positions_evalutaions){
                fout_game_evaluation_2 << val << "\n";
            }
        }
        
        if(i == 2){
            for(double val : game.positions_evalutaions){
                fout_game_evaluation_3 << val << "\n";
            }
        }
    
        //

        int current_occupied_tiles = 0;

        for(int j = 0; j < 61; j++){
            if(game.is_occupied[j]) {
                total_occupied_tiles++;
                current_occupied_tiles++;
            }
        }

        fout_total_occupied_tiles << current_occupied_tiles << "\n";

        //

        total_branches += game.total_branches / game.total_moves;

        fout_total_branches << (double) game.total_branches / game.total_moves << "\n";

        //

        total_decision_factors += (game.total_branches - (game.total_moves - game.more_than_one_move)) / game.more_than_one_move;

        fout_total_decision_factors << (double) (game.total_branches - (game.total_moves - game.more_than_one_move)) / game.more_than_one_move << "\n";

        //

        total_distance += (double) game.total_distance / game.total_moves;

        fout_total_distance << (double) game.total_distance / game.total_moves << "\n";

        //

        total_number_of_pieces += game.number_of_pieces / game.total_moves;

        fout_total_number_of_pieces << (double) game.number_of_pieces / game.total_moves << "\n";

        //
    }

    fout_winrates << wins[0] << ',' << wins[1] << ',' << wins[2];

    // close all opened files

    fout_decisiveness.close();
    fout_total_branches.close();
    fout_total_decision_factors.close();
    fout_total_distance.close();
    fout_total_drama.close();
    fout_total_lead_change_actions.close();
    fout_total_lead_change_moves.close();
    fout_total_more_than_one_move.close();
    fout_total_narrowness.close();
    fout_total_number_of_pieces.close();
    fout_total_occupied_tiles.close();
    fout_total_repeated_states.close();
    fout_total_turns.close();
    fout_winrates.close();
    
    return 0;
}