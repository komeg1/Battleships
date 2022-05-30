#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string.h>
#include<math.h>
#include <stdlib.h>
#define DEFAULT_BOARD_SIZE_Y 21
#define DEFAULT_BOARD_SIZE_X 10
#define DEFAULT_SHIPS 
#define YES 1
#define NO 0
#define EXIT_PROGRAM exit(0)
#define A 0
#define B 1
#define SKIP -1  //flaga, która pomija niektóre argumenty w funkcji

using namespace std;

enum direction {
    N = 1,
    S,
    W,
    E
};
enum class_of_ship {
    CAR = 1,
    BAT,
    CRU,
    DES
};
enum errors {    //ID b³êdów wystêpuj¹cych w grze
    TOO_CLOSE,
    REEF,
    SHIP_PRESENT,
    ALL_SET,
    NOT_IN_POSITION,
    OUT_OF_BOARD,
    NO_MOVES,
    CANNON_DESTROYED,
    FIELD_NOT_EXIST,
    NOT_ALL_SHIPS_PLACED,
    TOO_MANY_SHOOTS,
    SHOOT_TOO_FAR,
    ENGINE_DESTROYED,
    ALL_PLANES_SENT

};
struct Coordinates {
    int y;
    int x;
};
struct ShipParts {
    Coordinates coords;
    char current_state;
    char type;
};
struct Ship {
    ShipParts* ship_parts; //ka¿dy statek posiada tablice, która definiuje ka¿d¹ czêœæ tego statku
    int dir;
    char dirString;
    bool is_placed;
    int moves;
    int shoots;
    int radar_range;
    int spy_amount;
    int spy_index;
    Coordinates* planes;    //koordynaty samolotów
};
struct ShipClass {
    int maxMoves;
    int maxAmount;
    int maxShoots;
    int amount;
    int length;
    int max_radar_range;
    Ship* ship_class_arr; //ka¿da klasa statków posiada tablicê na statki o wielkoœci ich iloœci
    int cannon_range;

};
struct Player {
    char id;
    ShipClass* ship_arr;    //ka¿dy gracz posiada tablicê klas statków
    bool isTurn;
    Coordinates rangeCordsY;
    Coordinates rangeCordsX;

};
struct GameState {
    Coordinates board_size;
    bool extended_ships;
    bool firstPlayer;
};
struct Arguments {
    char* action;
    int y;
    int x;
    char dirByString;
    int dirByNum;
    int shipIndex;
    char* classByString;
    int classByNum;
    int playerID;
    char playerIDbychar;
    char move_action;
    bool is_extended;
};

struct vector {
    int i;
    int allocated_size;
    Coordinates* ptr;
};