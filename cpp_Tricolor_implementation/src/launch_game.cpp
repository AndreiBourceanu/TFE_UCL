// Launch a Tricolor game between 2 players

#include <iostream>
#include <SFML/Graphics.hpp>
#include <vector>
#include <math.h>
#include <filesystem>
#include "game/Game.h"
#include "agent/AgentRandom.h"
#include "agent/AgentHuman.h"

using namespace std;
namespace fs = std::filesystem;

unordered_map<char, sf::Color> my_color_to_sf_color;

struct pieces_touched {

    bool clicked_on_first_hex = false;
    Position position_of_first_hex = Position{-1, -1};
    int times_clicked_of_first_hex = 0;

    bool is_equal(Position p){
        return position_of_first_hex.x == p.x && position_of_first_hex.y == p.y;
    }
};

bool is_outside_board(Position position){
    return position.x == -1 && position.y == -1;
}

bool action_and_human_decision_are_the_same(Action action, pieces_touched human_pieces_touched, Position clicked_hex_position) {
    return action.from.is_equal(human_pieces_touched.position_of_first_hex) && action.to.is_equal(clicked_hex_position) && human_pieces_touched.times_clicked_of_first_hex == action.friend_pieces_moved + action.opponent_pieces_moved;
}

bool pointInHex(const sf::ConvexShape& hex, sf::Vector2f point)
{
    int count = hex.getPointCount();
    bool inside = false;

    for (int i = 0, j = count - 1; i < count; j = i++)
    {
        sf::Vector2f pi = hex.getTransform().transformPoint(hex.getPoint(i));
        sf::Vector2f pj = hex.getTransform().transformPoint(hex.getPoint(j));

        if (((pi.y > point.y) != (pj.y > point.y)) &&
            (point.x < (pj.x - pi.x) * (point.y - pi.y) / (pj.y - pi.y) + pi.x))
        {
            inside = !inside;
        }
    }

    return inside;
}

Position get_hex_coordinates_by_pixel(sf::RenderWindow &window, vector<vector<HexTile>> &board, sf::Vector2f &mouse_position, float radius){

    Position position{-1, -1};

    sf::Vector2u center_hex = {window.getSize().y / 2, window.getSize().x / 2};
    int line_col_center_hex = (board.size() - 1) / 2;

    for(int i = 0; i < board.size(); i++){
        for(int j = 0; j < board[i].size(); j++){

            sf::ConvexShape hex;
            hex.setPointCount(6);

            for (int k = 0; k < 6; k++) {
                float angle = 60.f * k - 30.f;
                float rad = angle * 3.141592653f / 180.f;

                hex.setPoint(k, {
                    radius * cos(rad) + radius,
                    radius * sin(rad) + radius
                });
            }

            float hex_center_position_x = center_hex.x + abs(i - line_col_center_hex) * radius * sqrt(3) / 2.f + radius * sqrt(3) * (j - line_col_center_hex) - radius;
            float hex_center_position_y = center_hex.y + 3.f * radius * (i - line_col_center_hex) / 2.f - radius;
            hex.setPosition(hex_center_position_x, hex_center_position_y);

            if(pointInHex(hex, mouse_position)){
                position.x = i;
                position.y = j;
                return position;
            }
        }
    }

    return position;
}

void draw_hex_tiles(sf::RenderWindow &window, vector<vector<HexTile>> &board, float radius, sf::Sprite &whitePiece, sf::Sprite &blackPiece, pieces_touched &human_pieces_touched, Game &game, vector<Action> &actions){

    sf::Vector2u center_hex = {window.getSize().y / 2, window.getSize().x / 2};
    int line_col_center_hex = (board.size() - 1) / 2;

    for(int i = 0; i < board.size(); i++){
        for(int j = 0; j < board[i].size(); j++){

            // first draw the hexagon
            sf::ConvexShape hex;
            hex.setPointCount(6);

            for (int k = 0; k < 6; k++) {
                float angle = 60.f * k - 30.f;
                float rad = angle * 3.141592653f / 180.f;

                hex.setPoint(k, {
                    radius * cos(rad) + radius,
                    radius * sin(rad) + radius
                });
            }

            hex.setFillColor(my_color_to_sf_color[board[i][j].color]);
            hex.setOutlineColor(sf::Color::Black);
            hex.setOutlineThickness(1.f);
            float hex_center_position_x = center_hex.x + abs(i - line_col_center_hex) * radius * sqrt(3) / 2.f + radius * sqrt(3) * (j - line_col_center_hex) - radius;
            float hex_center_position_y = center_hex.y + 3.f * radius * (i - line_col_center_hex) / 2.f - radius;
            hex.setPosition(hex_center_position_x, hex_center_position_y);
            window.draw(hex);

            // 
            bool stack_is_playable = false;
            for(Action action : actions){
                if(action.from.x == i && action.from.y == j){
                    stack_is_playable = true;
                    break;
                }
            }

            // then draw the pieces on the hexagon
            int place_of_tile_on_stack = 0;
            float const_offset = 10.f; // 10 pixels of offset per stacked piece
            int player = board[i][j].player;
            int owned_player = (player + 1) % 2;
            if(player == 2) continue;
            sf::Sprite opponent_piece_type, friend_piece_type;
            if(owned_player == 0){
                opponent_piece_type = whitePiece;
                friend_piece_type = blackPiece;
            }
            else{
                opponent_piece_type = blackPiece;
                friend_piece_type = whitePiece;
            }
            int total_pieces = board[i][j].pieces[owned_player] + board[i][j].pieces[player];
            int ith_piece = 0; 
            for(int k = 0; k < board[i][j].pieces[owned_player]; k++){
                ith_piece++;
                opponent_piece_type.setPosition(hex_center_position_x, hex_center_position_y - const_offset * place_of_tile_on_stack);
                if(stack_is_playable && human_pieces_touched.position_of_first_hex.x == i && human_pieces_touched.position_of_first_hex.y == j && ith_piece + human_pieces_touched.times_clicked_of_first_hex > total_pieces){
                    opponent_piece_type.setColor(sf::Color::Green);
                }
                window.draw(opponent_piece_type);
                place_of_tile_on_stack++;
            }
            for(int k = 0; k < board[i][j].pieces[player]; k++){
                ith_piece++;
                friend_piece_type.setPosition(hex_center_position_x, hex_center_position_y - const_offset * place_of_tile_on_stack);
                if(stack_is_playable && human_pieces_touched.position_of_first_hex.x == i && human_pieces_touched.position_of_first_hex.y == j && ith_piece + human_pieces_touched.times_clicked_of_first_hex > total_pieces){
                    friend_piece_type.setColor(sf::Color::Green);
                }
                window.draw(friend_piece_type);
                place_of_tile_on_stack++;
            }
            opponent_piece_type.setColor(sf::Color::White);
            friend_piece_type.setColor(sf::Color::White);
            if(stack_is_playable && board[i][j].player == game.player_turn && !game.players[game.player_turn]->is_bot){
                int circle_radius = radius - 30.f;
                sf::CircleShape aivalable_move_circle(circle_radius);
                float circle_center_offset = radius - circle_radius;
                aivalable_move_circle.setPosition(hex_center_position_x + circle_center_offset, hex_center_position_y + circle_center_offset);
                aivalable_move_circle.setFillColor(sf::Color(0, 0, 255, 150));
                window.draw(aivalable_move_circle);
            }
        }
    }
}

unique_ptr<Agent> create_agent(const string& name) {
    if (name == "human")
        return std::make_unique<AgentHuman>();
    else if (name == "random")
        return std::make_unique<AgentRandom>();
    else {
        std::cerr << "Unknown agent type: " << name << std::endl;
        exit(1);
    }
}


int main(int argc, char* argv[]) {

    if (argc < 3) {
        std::cout << "Usage: ./launch_game <player1> <player2>\n";
        std::cout << "Example: ./launch_game human random\n";
        return 1;
    }
    
    array<unique_ptr<Agent>, 2> players;
    players[0] = create_agent(argv[1]);
    players[1] = create_agent(argv[2]);
    Game game(5, move(players), 50);

    sf::RenderWindow window(sf::VideoMode(860, 860), "Tricolor");
    window.setFramerateLimit(10);

    // text fonts

    // Determine the folder where the executable lives
    fs::path exeDir = fs::absolute(argv[0]).parent_path();

    // Construct path to the font inside the assets folder
    fs::path fontPath = exeDir / "assets" / "DejaVuSans.ttf";

    sf::Font font;
    if (!font.loadFromFile(fontPath.string()))
        return -1;
    
    sf::Text text;
    text.setFont(font);
    text.setString("");
    int characters_size = 40;
    text.setCharacterSize(characters_size);
    text.setFillColor(sf::Color::Black);
    text.setPosition(window.getSize().x / 2, window.getSize().y - characters_size - 50.f);

    // pieces and textures

    float hex_radius = 40.f;

    my_color_to_sf_color['B'] = sf::Color::Black;
    my_color_to_sf_color['W'] = sf::Color::White;
    my_color_to_sf_color['R'] = sf::Color::Red;

    // Construct path to the png inside the assets folder
    fs::path pngPath = exeDir / "assets" / "checkers_pieces.png";

    sf::Texture piecesTexture;
    if (!piecesTexture.loadFromFile(pngPath.string())) {
        return -1;
    }

    sf::Vector2i pieceSize(768, 1024); // size of a single piece

    // White piece (left)
    sf::Sprite whitePiece(piecesTexture);
    whitePiece.setTextureRect(sf::IntRect(140, 70, pieceSize.x - 200, pieceSize.y - 70));

    // Black piece (right)
    sf::Sprite blackPiece(piecesTexture);
    blackPiece.setTextureRect(sf::IntRect(pieceSize.x + 25, 70, pieceSize.x - 25, pieceSize.y - 70));

    float targetSize = 100.f;
    float scaleX = targetSize / pieceSize.x;
    float scaleY = targetSize / pieceSize.y;
    whitePiece.setScale(scaleX, scaleY);
    blackPiece.setScale(scaleX, scaleY);

    sf::Clock clock;
    sf::Time timeSinceLastMove = sf::Time::Zero;
    float moveDelay = 1.f; // seconds per move

    bool printed_winner = false;

    pieces_touched human_pieces_touched;

    vector<Action> actions = game.board.actions(game.player_turn);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left) {
                
                sf::Vector2f mouse_position = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                Position clicked_hex_position = get_hex_coordinates_by_pixel(window, game.board.board, mouse_position, hex_radius);
                
                // clicking anywhere outside the board resets the clicked pieces
                if(is_outside_board(clicked_hex_position)){
                    human_pieces_touched = {false, Position{-1, -1}, 0};
                }
                // clicking inside the board only has effect if the player to move is not a bot
                else if(!game.players[game.player_turn]->is_bot){
                    
                    int total_pieces = game.board.board[clicked_hex_position.x][clicked_hex_position.y].pieces[0] + game.board.board[clicked_hex_position.x][clicked_hex_position.y].pieces[1];
                    // if the last click was outside the board, a new position is remembered
                    if(is_outside_board(human_pieces_touched.position_of_first_hex)){
                        human_pieces_touched = {true, clicked_hex_position, total_pieces};
                    }
                    // if the last click was inside the board we have 2 cases:
                    // 1) the click is on the same last hex
                    else if(human_pieces_touched.is_equal(clicked_hex_position)){
                        human_pieces_touched.times_clicked_of_first_hex--;
                        if(human_pieces_touched.times_clicked_of_first_hex == -1){
                            human_pieces_touched.times_clicked_of_first_hex = total_pieces;
                        }
                    }
                    // 2) the click is on a new hex
                    else{
                        // 2 cases:
                        // 1) it's a legal move
                        bool is_legal = false;
                        for(Action action : actions){
                            if(action_and_human_decision_are_the_same(action, human_pieces_touched, clicked_hex_position)){
                                game.make_move(action);
                                actions = game.board.actions(game.player_turn);
                                human_pieces_touched = {false, Position{-1, -1}, 0};
                                is_legal = true;
                                clock.restart();
                                timeSinceLastMove = sf::Time::Zero;
                                break;
                            }
                        }
                        // 2) it's an ilegal move, so change the last hex clicked
                        if(!is_legal){
                            human_pieces_touched = {true, clicked_hex_position, 0};
                        }
                    }
                }

            }
            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Right) {
                
                sf::Vector2f mouse_position = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                Position clicked_hex_position = get_hex_coordinates_by_pixel(window, game.board.board, mouse_position, hex_radius);

                // right clicking can only decrease the number of selected pieces of an alreadt clicked hex tile
                if(!is_outside_board(clicked_hex_position) && !game.players[game.player_turn]->is_bot && human_pieces_touched.is_equal(clicked_hex_position)){
                    int total_pieces = game.board.board[human_pieces_touched.position_of_first_hex.x][human_pieces_touched.position_of_first_hex.y].pieces[0] + game.board.board[human_pieces_touched.position_of_first_hex.x][human_pieces_touched.position_of_first_hex.y].pieces[1];
                    human_pieces_touched.times_clicked_of_first_hex++;
                    if(human_pieces_touched.times_clicked_of_first_hex == total_pieces + 1){
                        human_pieces_touched.times_clicked_of_first_hex = 0;
                    }
                }
            }
        }

        // if the current player, is a bot, don't interact with the window
        if(game.players[game.player_turn]->is_bot){
            // time update
            sf::Time dt = clock.restart();
            timeSinceLastMove += dt;

            // bot move step
            if (timeSinceLastMove.asSeconds() >= moveDelay && !game.game_is_over) {
                game.make_move();
                actions = game.board.actions(game.player_turn);
                timeSinceLastMove = sf::Time::Zero;
            }
        }

        if(game.game_is_over && !printed_winner){
            cout<<"Winner: ";
            if(game.winner == 0){
                cout<<" white";
                text.setString("White won");
            }
            else if(game.winner == 1){
                cout<<" black";
                text.setString("Black won");
            }
            else{
                cout<<" draw";
                text.setString("Draw");
            }
            cout<<endl;
            printed_winner = true;
        }

        window.clear(sf::Color::White);

        draw_hex_tiles(window, game.board.board, hex_radius, whitePiece, blackPiece, human_pieces_touched, game, actions);

        sf::FloatRect bounds = text.getLocalBounds();
        text.setOrigin(bounds.left + bounds.width / 2.0f, 0);
        text.setPosition(window.getSize().x / 2.0f, window.getSize().y - characters_size - 50.f);
        window.draw(text);

        window.display();
    }
}
