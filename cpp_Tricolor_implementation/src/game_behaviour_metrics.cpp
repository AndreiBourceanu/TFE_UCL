// Launch Tricolor game simulations between 2 random players and exract useful data

#include <iostream>
#include <fstream>
#include <chrono>
#include <math.h>
#include <iomanip>
#include "game/Game.h"
#include "agent/AgentRandom.h"
#include "agent/AgentAlphaBeta.h"
#include "agent/Heuristics.h"

using namespace std;

int GAMES_PLAYED = 1000000;

int main()
{
    int wins[3] = {0, 0, 0};

    ofstream fout_total_turns("csv_files/game_behaviour/total_turns.csv");
    fout_total_turns<<"total_turns\n";

    int total_turns = 0;
    int game_turns[GAMES_PLAYED];

    ofstream fout_total_repeated_states("csv_files/game_behaviour/total_repeated_states.csv");
    fout_total_repeated_states<<"total_repeated_states\n";

    int total_repeated_states = 0;

    ofstream fout_total_more_than_one_move("csv_files/game_behaviour/total_more_than_one_move.csv");
    fout_total_more_than_one_move<<"total_more_than_one_move\n";

    int total_moves = 0;
    int total_more_than_one_move = 0;

    ofstream fout_winrates("csv_files/game_behaviour/winrates.csv");
    fout_winrates<<"p1,p2,draw\n";

    ofstream fout_total_narrowness("csv_files/game_behaviour/total_narrowness.csv");
    fout_total_narrowness<<"total_narrowness\n";

    int total_narrowness = 0;

    ofstream fout_decisiveness("csv_files/game_behaviour/decisiveness.csv");
    fout_decisiveness<<"decisiveness\n";

    double earliest_turn_lead_change_game_percentage = 0;

    ofstream fout_total_lead_change_moves("csv_files/game_behaviour/total_lead_change_moves.csv");
    fout_total_lead_change_moves<<"total_lead_change_moves\n";

    int total_lead_change_moves = 0;

    ofstream fout_total_lead_change_actions("csv_files/game_behaviour/total_lead_change_actions.csv");
    fout_total_lead_change_actions<<"total_lead_change_actions\n";

    double total_lead_change_actions = 0;

    ofstream fout_total_drama("csv_files/game_behaviour/total_drama.csv");
    fout_total_drama<<"total_drama\n";

    double total_drama = 0;

    // game evaluations
    // 3 random games have already been generated (white win, black win, draw)
    /*
    ofstream fout_game_evaluation_1("csv_files/game_behaviour/game_evaluation_1.csv");
    fout_game_evaluation_1<<"game_evaluation_1\n";

    ofstream fout_game_evaluation_2("csv_files/game_behaviour/game_evaluation_2.csv");
    fout_game_evaluation_2<<"game_evaluation_2\n";

    ofstream fout_game_evaluation_3("csv_files/game_behaviour/game_evaluation_3.csv");
    fout_game_evaluation_3<<"game_evaluation_3\n";
    */

    //

    ofstream fout_total_occupied_tiles("csv_files/game_behaviour/total_occupied_tiles.csv");
    fout_total_occupied_tiles<<"total_occupied_tiles\n";

    int total_occupied_tiles = 0;

    ofstream fout_total_branches("csv_files/game_behaviour/total_branches.csv");
    fout_total_branches<<"total_branches\n";

    int total_branches = 0;

    ofstream fout_total_decision_factors("csv_files/game_behaviour/total_decision_factors.csv");
    fout_total_decision_factors<<"total_decision_factors\n";

    int total_decision_factors = 0;

    ofstream fout_total_distance("csv_files/game_behaviour/total_distance.csv");
    fout_total_distance<<"total_distance\n";

    double total_distance = 0;

    ofstream fout_total_number_of_pieces("csv_files/game_behaviour/total_number_of_pieces.csv");
    fout_total_number_of_pieces<<"total_number_of_pieces\n";

    int total_number_of_pieces = 0;

    for (int i = 0; i < GAMES_PLAYED; i++) {

        array<unique_ptr<Agent>, 2> players;
        players[0] = make_unique<AgentRandom>();
        players[1] = make_unique<AgentRandom>();

        GameWithBehaviourMetrics game(move(players), 50);
        game.start();

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

        // game evaluations (already generated)
        /*
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
        */
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

    // avg number of turns
    double avg_turns = (double) total_turns / GAMES_PLAYED;
    // std dev for turns
    double std_dev_avg_turns = 0;
    // compute variance
    for(int i = 0; i < GAMES_PLAYED; i++){
        std_dev_avg_turns += ((double) game_turns[i] - avg_turns) * ((double) game_turns[i] - avg_turns);
    }
    std_dev_avg_turns /= GAMES_PLAYED;
    // std dev
    std_dev_avg_turns = sqrt(std_dev_avg_turns);

    cout << fixed << setprecision(3);

    cout << "In " << GAMES_PLAYED << " games played, there are on average:\n\n";
    cout << (double) total_turns / GAMES_PLAYED << " turns per game with std dev of " << std_dev_avg_turns << ";\n";
    cout << (double) total_repeated_states / GAMES_PLAYED << " repeated states per game;\n";
    cout << (double) total_more_than_one_move * 100 / total_moves << "% moves where there was more than 1 possible move;\n\n";

    cout << "Narrowness " << total_narrowness / GAMES_PLAYED << ";\n";
    cout << "Game % when the move was decisive (lead didn't change since then) " << earliest_turn_lead_change_game_percentage * 100 / GAMES_PLAYED << "%;\n";
    cout << "Lead change moves " << total_lead_change_moves / GAMES_PLAYED << ";\n";
    cout << "Lead change turns during a game " << total_lead_change_actions * 100 / GAMES_PLAYED << "%;\n";
    cout << "Drama " << total_drama / GAMES_PLAYED << ";\n\n";

    cout << (double) total_occupied_tiles * 100 / (GAMES_PLAYED * 61) << "% of occupied tiles at least once per game;\n";
    cout << "Player 1 (White) won " << (double) wins[0] * 100 / GAMES_PLAYED << "% of the games;\n";
    cout << "Player 2 (Black) won " << (double) wins[1] * 100 / GAMES_PLAYED << "% of the games;\n";
    cout << "The rest of " << (double) wins[2] * 100 / GAMES_PLAYED << "% of the games are draws;\n";
    cout << "Branching factor " << total_branches / GAMES_PLAYED << "\n";
    cout << "Decision factor " << total_decision_factors / GAMES_PLAYED << "\n";
    cout << "Distance traveled by pieces " << total_distance / GAMES_PLAYED << "\n";
    cout << "Number of pieces on the board " << total_number_of_pieces / GAMES_PLAYED << "\n";

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