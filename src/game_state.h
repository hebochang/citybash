/*
 * GameState maintains the entire state of the game.
 *  i.e. Initialization, actions done in cities, attacks, etc.
 *
 * The game is a state machine with the following states/transisions:
 *  ADMIN_WAIT: the initial state, waiting for the admin to join and send
 *              [PLAYER_JOIN] to start that phase
 *  PLAYER_JOIN: in this state, players can connect to the game by sending
 *              their unique key (so each player signs up once) and their
 *              city name. Admin must send [START_GAME] to proceed.
 *  PLAYING: the world is generated and the game commences, players are
 *           allowed to send the regular game commands.
 *  FINISHED: the game is stopped automatically, determined by the winning
 *            conditions as stated in the README.
 *
 * Valid Admin commands for each state are:
 *  ADMIN_WAIT
 *   - PLAYER_JOIN: commences PLAYER_JOIN state
 *   - SERVER_INFO: gives useful information about the game server, TDB
 *
 *  PLAYER_JOIN
 *   - START_GAME: generates the world and commences PLAYING state
 *   - PLAYERS: return players who have joined
 *   - SERVER_INFO: (same as in ADMIN_WAIT)
 *
 *  PLAYING
 *   - STATS: give for each city its gold, gold/sec, level, and army size
 *   - MAP: return a coordinate list of the cities by id,name
 *   - FORCE_FINISH: forces the game to stop, commencing FINISHED state
 *
 *  FINISHED
 *   - LEADERBOARD: return the player rankings for the game just played
 *   - TERMINATE: signals the server to terminate
 */
#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <map>
#include <string>

#include "city.h"
#include "world.h"

class GameState {
  private:
    enum PlayState {
      ADMIN_WAIT,
      PLAYER_JOIN,
      PLAYING,
      FINISHED,
    };
    PlayState state_;
    World world;

    /* The game state associates each city with a city_id */
    std::map<city_id, City> city_map_;

    std::string invalid_command(std::string command,
        std::vector<std::string> valid_commands);

    static std::string PLAYER_JOIN_REQ;
    static std::string SERVER_INFO;
    static std::string START_GAME;
    static std::string PLAYERS;
    static std::string STATS;
    static std::string MAP;
    static std::string FORCE_FINISH;
    static std::string LEADERBOARD;
    static std::string TERMINATE;

    std::string admin_player_join();
    std::string admin_server_info();
    std::string admin_start_game();
    std::string admin_players();
    std::string admin_stats();
    std::string admin_map();
    std::string admin_force_finish();
    std::string admin_leaderboard();
    std::string admin_terminate();

    std::string get_world_info(city_id id);
    std::string get_city_info(city_id id);
    std::string get_costs_info(city_id id);
    std::string upgrade_city(city_id id);
    std::string train_soldiers(city_id id, int soldiers);
    std::string start_attack(city_id from_city, std::string to_city, int soldiers);

  public:
    GameState();

    std::string admin_request(std::string command);
    std::string player_request(std::string command, city_id id,
        std::vector<std::string> args);

};

#endif // GAME_STATE_H
