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

        ActionOpti action = players[player_turn]->choose_action(board, player_turn);
        if(action.is_capture){
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
        action = players[player_turn]->choose_action(board, player_turn);
    }
    else{
        action = human_action;
    }
    if(action.is_capture){
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

    ActionOpti action = players[player_turn]->choose_action(board, player_turn);
    if(action.is_capture){
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

GameWithBehaviourMetrics::GameWithBehaviourMetrics(array<unique_ptr<Agent>, 2> players, int turns_with_no_imprisonment_or_capture_until_draw) : players(move(players)){
    board = BoardOpti();
    turn = 1;
    last_turn_capture = 1;
    player_turn = 0;
    this->turns_with_no_imprisonment_or_capture_until_draw = turns_with_no_imprisonment_or_capture_until_draw;
    winner = 2;
    game_is_over = false;

    for(int i = 0; i < 18; i++){
        is_occupied[i] = true;
    }
    for(int i = 18; i < 43; i++){
        is_occupied[i] = false;
    }
    for(int i = 43; i < 61; i++){
        is_occupied[i] = true;
    }
}

// Get the distance traveled by a stack after a move
int get_move_distance(int tile_from, int tile_to){
    for(int i = 0; i < 6; i++){
        int current_tile = tile_from;
        for(int j = 0; j < 3; j++){
            if(neighbours_opti[current_tile][i] == -1) break;
            if(neighbours_opti[current_tile][i] == tile_to){
                return j + 1;
            }
            current_tile = neighbours_opti[current_tile][i];
        }
    }
    return -1;
}

// return a score between -1 and 1 which represents how much
// better white or black are in the game
// 1 -> white is >> black in the game
// -1 -> black is >> white in the game
// 0 -> the position is equal
// the score is computed based on the number of pieces and the power of the tiles they are on
double position_evaluation(BoardOpti& board){
    int white_utility = 0;
    int black_utility = 0;

    for(int tile = 0; tile < 61; tile++){
        if(board.get_player(tile) == 0){
            white_utility += board.get_white_pieces(tile) * colors_opti[tile];
        }
        else if(board.get_player(tile) == 1){
            black_utility += board.get_black_pieces(tile) * colors_opti[tile];
        }
    }

    return (double) (white_utility - black_utility) / (white_utility + black_utility);
}

void GameWithBehaviourMetrics::start(){
    while(true){

        // add the evalutation of the current position (for drama measures)
        positions_evalutaions.push_back(position_evaluation(board));

        // count the position in the hash map
        hashed_state_freq[board.hash]++;

        // first get the actions
        vector<ActionOpti> actions = board.get_actions(player_turn);

        // the game stops when:
        // 1) the current player doesn't have any moves, the other player wins and the game ends
        if(actions.size() == 0){
            winner = (player_turn + 1) % 2;
            game_is_over = true;
            turn--;

            // decisiveness
            // match the values for the winner (0 = white and 1 = black)
            if(current_leader == -1) current_leader = 1;
            else if(current_leader == 1) current_leader = 0;
            if(winner != current_leader){
                earliest_turn_lead_change = turn;
            }

            break;
        }
        // 2) no imprisonment or capture moves were made in the last 50 moves => draw
        if(turn - last_turn_capture > turns_with_no_imprisonment_or_capture_until_draw){
            winner = 2;
            game_is_over = true;
            turn--;
            break;
        }

        // increase total moves by 1
        total_moves++;
        // check if player has more than 1 move
        if(actions.size() > 1){
            more_than_one_move++;
        }

        // add number of actions to total number of branches
        total_branches += actions.size();

        // count number of pieces on board
        for(int i = 0; i < 61; i++){
            number_of_pieces += board.get_white_pieces(i) + board.get_black_pieces(i);
        }

        // check how many actions lead to an equal or better position than the current one (narrowness)
        double current_position_evaluation = position_evaluation(board);
        for(ActionOpti action : actions){
            BoardOpti result_board = board.result_state_after_action(player_turn, action);
            double result_board_evaluation = position_evaluation(result_board);
            double diff_evaluation = result_board_evaluation - current_position_evaluation;

            // if white to play, the diff must be postive to be an improvement
            if(player_turn == 0){
                if(diff_evaluation >= 0){
                    total_narrowness++;
                }
            }
            // if black to play, the diff must be negative to be an improvement
            else{
                if(diff_evaluation <= 0){
                    total_narrowness++;
                }
            }

            // if the 2 boards have different player who is better, then it's a lead change move
            if(current_position_evaluation * result_board_evaluation <= 0){
                total_lead_change_moves++;
            }
        }

        // the player chooses an action
        ActionOpti action = players[player_turn]->choose_action(board, player_turn);

        // check the occupied tile from the action
        is_occupied[action.to] = true;

        // get the distance of the move
        total_distance += get_move_distance(action.from, action.to);

        // check if it's a capture
        if(action.is_capture){
            last_turn_capture = turn;
        }
        // execute the action
        board.execute_action(player_turn, action);

        // decisiveness
        double result_board_eval = position_evaluation(board);
        // the leader changes
        if(result_board_eval * current_leader <= 0){
            earliest_turn_lead_change = turn;
            if(result_board_eval > 0){
                current_leader = 1;
            }
            else if(result_board_eval < 0){
                current_leader = -1;
            }
            else{
                current_leader = 0;
            }
        }

        // drama
        total_drama += abs(result_board_eval - current_position_evaluation);

        // how many times lead changes in a game
        if(current_position_evaluation * result_board_eval <= 0){
            total_lead_change_actions++;
        }

        // modify player's turn
        player_turn = (player_turn + 1) % 2;
        // increase turn
        turn++;
    }
}