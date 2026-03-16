#include <iostream>
#include <vector>
#include "../board/Board.h"
#include "../game/Game.h"
#include "../agent/AgentHuman.h"
#include <bitset>

using namespace std;

Game::Game(int board_size, array<unique_ptr<Agent>, 2> players, int turns_with_no_imprisonment_or_capture_until_draw) : players(move(players)){
    board = Board(board_size);
    turn = 1;
    last_turn_capture = 1;
    player_turn = 0;
    this->turns_with_no_imprisonment_or_capture_until_draw = turns_with_no_imprisonment_or_capture_until_draw;
    winner = 2;
    game_is_over = false;
}

void Game::start(){
    while(true){
        // it stops when:
        // 1) the current player doesn't have any moves, the other player wins and the game ends
        vector<Action> actions = board.actions(player_turn);
        if(actions.size() == 0){
            winner = (player_turn + 1) % 2;
            game_is_over = true;
            break;
        }
        // 2) no imprisonment or capture moves were made in the last 50 moves => draw
        if(turn - last_turn_capture > turns_with_no_imprisonment_or_capture_until_draw){
            winner = 2;
            game_is_over = true;
            break;
        }

        Action action = players[player_turn]->choose_action(board, actions);
        if(action.is_imprisonment_or_capture){
            last_turn_capture = turn;
        }
        board.execute_action(player_turn, action);

        player_turn = (player_turn + 1) % 2;
        turn++;
    }
}

void Game::make_move(Action human_action){
    // it stops when:
    // 1) the current player doesn't have any moves, the other player wins and the game ends
    vector<Action> actions = board.actions(player_turn);
    if(actions.size() == 0){
        winner = (player_turn + 1) % 2;
        game_is_over = true;
        return;
    }
    // 2) no imprisonment or capture moves were made in the last 50 moves => draw
    if(turn - last_turn_capture > turns_with_no_imprisonment_or_capture_until_draw){
        winner = 2;
        game_is_over = true;
        return;
    }

    Action action;
    if(players[player_turn]->is_bot){
        action = players[player_turn]->choose_action(board, actions);
    }
    else{
        action = human_action;
    }
    if(action.is_imprisonment_or_capture){
        last_turn_capture = turn;
    }
    board.execute_action(player_turn, action);

    player_turn = (player_turn + 1) % 2;
    turn++;
    actions = board.actions(player_turn);
    if(actions.size() == 0){
        winner = (player_turn + 1) % 2;
        game_is_over = true;
        return;
    }
}

void Game::make_move(){
    // it stops when:
    // 1) the current player doesn't have any moves, the other player wins and the game ends
    vector<Action> actions = board.actions(player_turn);
    if(actions.size() == 0){
        winner = (player_turn + 1) % 2;
        game_is_over = true;
        return;
    }
    // 2) no imprisonment or capture moves were made in the last 50 moves => draw
    if(turn - last_turn_capture > turns_with_no_imprisonment_or_capture_until_draw){
        winner = 2;
        game_is_over = true;
        return;
    }

    Action action = players[player_turn]->choose_action(board, actions);
    if(action.is_imprisonment_or_capture){
        last_turn_capture = turn;
    }
    board.execute_action(player_turn, action);

    player_turn = (player_turn + 1) % 2;
    turn++;
    actions = board.actions(player_turn);
    if(actions.size() == 0){
        winner = (player_turn + 1) % 2;
        game_is_over = true;
        return;
    }
}

GameOpti::GameOpti(array<unique_ptr<Agent>, 2> players, int turns_with_no_imprisonment_or_capture_until_draw) : players(move(players)){
    board = BoardOpti();
    turn = 1;
    last_turn_capture = 1;
    player_turn = 0;
    this->turns_with_no_imprisonment_or_capture_until_draw = turns_with_no_imprisonment_or_capture_until_draw;
    winner = 2;
    game_is_over = false;
}

void GameOpti::start(){
    while(true){
        // it stops when:
        // 1) the current player doesn't have any moves, the other player wins and the game ends
        vector<ActionOpti> actions = board.get_actions(player_turn);
        if(actions.size() == 0){
            winner = (player_turn + 1) % 2;
            game_is_over = true;
            break;
        }
        // 2) no imprisonment or capture moves were made in the last 50 moves => draw
        if(turn - last_turn_capture > turns_with_no_imprisonment_or_capture_until_draw){
            winner = 2;
            game_is_over = true;
            break;
        }

        ActionOpti action = players[player_turn]->choose_action(board, actions);
        if(action.is_imprisonment_or_capture){
            last_turn_capture = turn;
        }
        board.execute_action(player_turn, action);

        player_turn = (player_turn + 1) % 2;
        turn++;
    }
}

void GameOpti::make_move(ActionOpti human_action){
    // it stops when:
    // 1) the current player doesn't have any moves, the other player wins and the game ends
    vector<ActionOpti> actions = board.get_actions(player_turn);
    if(actions.size() == 0){
        winner = (player_turn + 1) % 2;
        game_is_over = true;
        return;
    }
    // 2) no imprisonment or capture moves were made in the last 50 moves => draw
    if(turn - last_turn_capture > turns_with_no_imprisonment_or_capture_until_draw){
        winner = 2;
        game_is_over = true;
        return;
    }

    ActionOpti action;
    if(players[player_turn]->is_bot){
        action = players[player_turn]->choose_action(board, actions);
    }
    else{
        action = human_action;
    }
    if(action.is_imprisonment_or_capture){
        last_turn_capture = turn;
    }
    board.execute_action(player_turn, action);

    player_turn = (player_turn + 1) % 2;
    turn++;
    actions = board.get_actions(player_turn);
    if(actions.size() == 0){
        winner = (player_turn + 1) % 2;
        game_is_over = true;
        return;
    }
}

void GameOpti::make_move(){
    // it stops when:
    // 1) the current player doesn't have any moves, the other player wins and the game ends
    vector<ActionOpti> actions = board.get_actions(player_turn);
    if(actions.size() == 0){
        winner = (player_turn + 1) % 2;
        game_is_over = true;
        return;
    }
    // 2) no imprisonment or capture moves were made in the last 50 moves => draw
    if(turn - last_turn_capture > turns_with_no_imprisonment_or_capture_until_draw){
        winner = 2;
        game_is_over = true;
        return;
    }

    ActionOpti action = players[player_turn]->choose_action(board, actions);
    if(action.is_imprisonment_or_capture){
        last_turn_capture = turn;
    }
    board.execute_action(player_turn, action);

    player_turn = (player_turn + 1) % 2;
    turn++;
    actions = board.get_actions(player_turn);
    if(actions.size() == 0){
        winner = (player_turn + 1) % 2;
        game_is_over = true;
        return;
    }
}