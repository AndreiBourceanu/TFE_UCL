#pragma once
#include <iostream>
#include <vector>
#include <memory>
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