#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <unordered_map>
#include "../agent/Agent.h"

using namespace std;

class Game{
public:
Board board;
// 0 or 1
int player_turn;
int turn;
int last_turn_capture;
// for the moment 50
int turns_with_no_imprisonment_or_capture_until_draw;
// 0 -> player 1 wins, 1 -> player 2 wins, 2 -> draw
int winner;
bool game_is_over;
array<unique_ptr<Agent>, 2> players;
Game(int board_size, array<unique_ptr<Agent>, 2> players, int turns_with_no_imprisonment_or_capture_until_draw);
void start();
void make_move();
void make_move(Action action);
private:
};

struct GameOpti{

    BoardOpti board;
    // 0 or 1
    int player_turn;
    int turn;
    int last_turn_capture;
    // for the moment 50
    int turns_with_no_imprisonment_or_capture_until_draw;
    // 0 -> player 1 wins, 1 -> player 2 wins, 2 -> draw
    int winner;
    bool game_is_over;
    array<unique_ptr<Agent>, 2> players;
    GameOpti(array<unique_ptr<Agent>, 2> players, int turns_with_no_imprisonment_or_capture_until_draw);
    void start();
    void make_move();
    void make_move(ActionOpti action);
};

double position_evaluation(BoardOpti& board);

struct GameWithBehaviourMetrics{

    BoardOpti board;
    // 0 or 1
    int player_turn;
    int turn;
    int last_turn_capture;
    // for the moment 50
    int turns_with_no_imprisonment_or_capture_until_draw;
    // 0 -> player 1 wins, 1 -> player 2 wins, 2 -> draw
    int winner;
    bool game_is_over;
    array<unique_ptr<Agent>, 2> players;

    ////
    //// Metrics

    // 4.1 State Repetition: State repetition. (done)
    // 4.1.1 Positional Repetition (DoubleData): Average number of repeated positional states. (done)
    unordered_map<uint64_t, int> hashed_state_freq;

    // 4.2 Duration: Game duration. (done)
    // 4.2.3 Duration Turns (DoubleData): Number of turns in a game. (done)
    // 4.2.4 Duration Turns Std Dev (DoubleData): Number of turns in a game (std dev). (done)

    // 4.3 Complexity: Game complexity. (done)
    // 4.3.1 Decision Moves (DoubleData): Percentage of moves where there was more than one possible move. (done)
    // 4.3.2 Game Tree Complexity (DoubleData): Game Tree Complexity Estimate. (done)
    // 4.3.3 State Tree Complexity (DoubleData): State Space Complexity Upper Bound. (done)
    int more_than_one_move=0;
    int total_moves=0;

    // 4.4 Board Coverage: Board Coverage. (not interesting for Tricolor)
    // 4.4.1 Board Coverage Default (DoubleData): Percentage of default board sites which a piece was placed on at some point.
    // 4.4.2 Board Coverage Full (DoubleData): Percentage of all board sites which a piece was placed on at some point.
    // 4.4.3 Board Coverage Used (DoubleData): Percentage of used board sites which a piece was placed on at some point.

    // 4.5 Game Outcome: Game Outcome. (done)
    // 4.5.1 Advantage P1 (DoubleData): Percentage of games where player 1 won.
    // 4.5.2 Balance (DoubleData): Similarity between player win rates.
    // 4.5.3 Completion (DoubleData): Percentage of games which have a winner (not draw or timeout).
    // 4.5.4 Drawishness (DoubleData): Percentage of games which end in a draw.

    // 4.6 State Evaluation: State Evaluation.
    //    4.6.1 Clarity: Clarity.
    //        4.6.1.1 Narrowness (DoubleData): Narrowness.
    //        4.6.1.2 Variance (DoubleData): Variance.
    //    4.6.2 Decisiveness: Decisiveness.
    //        4.6.2.1 Decisiveness Moves (DoubleData): Decisiveness Moves.
    //        4.6.2.2 Decisiveness Threshold (DoubleData): Decisiveness Threshold.
    //    4.6.3 Lead Change (DoubleData): LeadChange.
    //    4.6.4 Stability (DoubleData): Stability.
    //    4.6.5 Drama: Drama.
    //    4.6.6 Move Evaluation: Drama.
    //    4.6.7 State Evaluation Difference: Drama.

    // how many moves maintain or improve the evaluation of the position
    int total_narrowness=0;
    // for decisiveness
    // 0 -> equal, 1 -> white, -1 -> black
    int current_leader=0;
    int earliest_turn_lead_change=1;
    // drama
    int total_lead_change_moves=0;
    int total_lead_change_actions=0;
    double total_drama=0;
    // game evaluation
    vector<double> positions_evalutaions;

    // 4.7 Board Sites Occupied: Board sites occupied. (done)
    bool is_occupied[61];

    // 4.8 Branching Factor: Branching factor. (done)
    int total_branches=0;

    // 4.9 Decision Factor: Decision factor. (done)

    // 4.10 Move Distance: Move distance.
    int total_distance=0;

    // 4.11 Piece Number: Piece number.
    int number_of_pieces=0;

    ////

    GameWithBehaviourMetrics(array<unique_ptr<Agent>, 2> players, int turns_with_no_imprisonment_or_capture_until_draw);
    void start();
    void make_move();
    void make_move(ActionOpti action);
};