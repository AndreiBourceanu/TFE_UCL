#include <iostream>
#include <chrono>
#include "game/Game.h"
#include "agent/AgentRandom.h"

using namespace std;

int main()
{
    int wins[3] = {0, 0, 0};
    int total_turns = 0;
    int games_played = 10000;
    auto start = chrono::high_resolution_clock::now();
    for (int i = 0; i < games_played; i++) {
        array<unique_ptr<Agent>, 2> players;
        players[0] = make_unique<AgentRandom>();
        players[1] = make_unique<AgentRandom>();

        Game game(5, move(players), 100);
        game.start();
        wins[game.winner]++;
        total_turns += game.turn;
    }
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double> duration = end - start;
    cout << "Time taken for "<<games_played<<" games: " << duration.count() << " seconds\n";
    cout << "Games simulated in 1 second: "<< games_played / duration.count()  << " games\n";
    cout << "Turns played in "<<games_played<<" games: " << total_turns << " turns\n";
    cout << "Turns played in 1 second: "<< total_turns / duration.count()  << " turns\n";
    cout << "White wins: "<<wins[0]<<endl;
    cout << "Black wins: "<<wins[1]<<endl;
    cout << "Draws: "<<wins[2]<<endl;
}