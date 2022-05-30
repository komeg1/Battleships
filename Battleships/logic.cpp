#include "Definitions.h"



void init(vector** v) {
    (*v)->i = 0;
    (*v)->allocated_size = 1;
    (*v)->ptr = new Coordinates[(*v)->allocated_size];
}
void reallocate(vector** v, int reallocate_size) {
    (*v)->allocated_size = reallocate_size;
    Coordinates* newPtr = new Coordinates[(*v)->allocated_size];
    memcpy(newPtr, (*v)->ptr, (*v)->i * sizeof(Coordinates));
    delete (*v)->ptr;
    (*v)->ptr = newPtr;
}
void push_back(vector** v, Coordinates val) {
    if ((*v)->i == (*v)->allocated_size)
        reallocate(v, 2 * (*v)->allocated_size);
    (*v)->ptr[(*v)->i] = { val.y,val.x };
    (*v)->i++;
}

int CheckParts(Player* player, int id) //zliczanie czêœci statków, które nie s¹ zniszczone
{
    int count = 0;
    for (int i = 1; i < 5; i++)
        for (int j = 0; j < player[id].ship_arr[i].maxAmount; j++)
            if (player[id].ship_arr[i].ship_class_arr[j].is_placed)
            {
                for (int k = 0; k < player[id].ship_arr[i].length; k++)
                {

                    if (player[id].ship_arr[i].ship_class_arr[j].ship_parts[k].current_state == '+')
                    {
                        count++;
                    }
                }
            }

    return count;
}
void PrintColumnsAndRows(char* tempBoard[], GameState* game) {
    cout << "  ";
    if (game->board_size.x > 10)
    {
        int counter = 0;
        int pom = 0;
        for (int i = 0; i < game->board_size.x; i++)
        {
            pom++;
            if (pom > 10)
            {
                pom -= 10;
                counter++;
            }
            cout << counter;
        }
    }
    cout << endl;
    cout << "  ";
    for (int i = 0; i < game->board_size.x; i++) {
        if (i < 10)
            cout << i;
        else if (i >= 10 && i < 100)
            cout << i % 10;
        else if (i > 100)
            cout << i % 100;
    }
    cout << endl;
    for (int i = 0; i < game->board_size.y; i++)
    {
        if (i < 10)
            cout << '0';
        cout << i;
        for (int j = 0; j < game->board_size.x; j++)
            cout << tempBoard[i][j];
        cout << endl;
    }
}
void ExtendedPrinting(Player* player, char* tempBoard[], GameState* game)
{
    for (int id = A; id <= B; id++)
        for (int i = CAR; i <= DES; i++)
            for (int j = 0; j < player[id].ship_arr[i].maxAmount; j++)
            {
                for (int k = 0; k < player[id].ship_arr[i].length; k++)
                {
                    int y = player[id].ship_arr[i].ship_class_arr[j].ship_parts[k].coords.y;
                    int x = player[id].ship_arr[i].ship_class_arr[j].ship_parts[k].coords.x;
                    if (player[id].ship_arr[i].ship_class_arr[j].ship_parts[k].current_state == 'x')
                    {
                        tempBoard[y][x] = 'x';
                    }
                    else if (player[id].ship_arr[i].ship_class_arr[j].ship_parts[k].current_state == '+')
                    {

                        if (player[id].ship_arr[i].ship_class_arr[j].ship_parts[k].type == 'e' || player[id].ship_arr[i].ship_class_arr[j].ship_parts[k].type == 'b')
                        {
                            tempBoard[y][x] = '%';
                        }
                        else if (player[id].ship_arr[i].ship_class_arr[j].ship_parts[k].type == 'c')
                            tempBoard[y][x] = '!';
                        else if (player[id].ship_arr[i].ship_class_arr[j].ship_parts[k].type == 'r')
                            tempBoard[y][x] = '@';
                        else
                            tempBoard[y][x] = '+';
                    }
                }
            }
    PrintColumnsAndRows(tempBoard, game);

}
void Print_state(char* board[], Player* player, GameState* game) {
    int num;
    cin >> num;
    if (num == 0)
    {
        for (int i = 0; i < game->board_size.y; i++)
        {
            for (int j = 0; j < game->board_size.x; j++)
                cout << board[i][j];
            cout << endl;
        }
    }
    if (num == 1)
    {
        char** tempBoard = new char* [game->board_size.y];
        for (int i = 0; i < game->board_size.y; i++)
            tempBoard[i] = new char[game->board_size.x];
        for (int i = 0; i < game->board_size.y; i++)
            for (int j = 0; j < game->board_size.x; j++)
                tempBoard[i][j] = board[i][j];

        ExtendedPrinting(player, tempBoard, game);

        for (int i = 0; i < game->board_size.y; i++)
            delete tempBoard[i];
        delete[] tempBoard;
    }
    int countPlayerA = CheckParts(player, 0),
        countPlayerB = CheckParts(player, 1);
    cout << "PARTS REMAINING:: A : " << countPlayerA << " B : " << countPlayerB << endl;



}
char** InitializeBoard(GameState* game) {
    char** mainBoard = new char* [game->board_size.y];
    for (int i = 0; i < game->board_size.y; i++)
        mainBoard[i] = new char[game->board_size.x];


    for (int i = 0; i < game->board_size.y; i++)
        for (int j = 0; j < game->board_size.x; j++)
            mainBoard[i][j] = ' ';

    return mainBoard;
}
ShipClass* InitializeShipArrays(int amounts[]) {
    ShipClass* tab = new ShipClass[5];
    int default_length = 5;
    for (int i = 1; i < 5; i++)
    {
        tab[i].maxAmount = amounts[i]; tab[i].amount = amounts[i]; tab[i].length = default_length; tab[i].max_radar_range = default_length;
        if (i == 1)
            tab[i].maxMoves = 2;
        else
            tab[i].maxMoves = 3;
        if (tab[i].maxAmount > 0)
            tab[i].ship_class_arr = new Ship[amounts[i]];
        tab[i].maxShoots = default_length;

        for (int j = 0; j < amounts[i]; j++)
        {
            tab[i].ship_class_arr[j] = { new ShipParts[default_length] ,0,'t',NO,tab[i].maxMoves,tab[i].maxShoots,tab[i].max_radar_range,5,0,new Coordinates[5] };
            if (i != 1)
                tab[i].ship_class_arr[j].spy_amount = 0;
            for (int k = 0; k < tab[i].length; k++)
            {
                if (k == 0)
                    tab[i].ship_class_arr[j].ship_parts[k].type = 'r';
                else if (k == 1)
                    tab[i].ship_class_arr[j].ship_parts[k].type = 'c';
                else
                    tab[i].ship_class_arr[j].ship_parts[k].type = 'n';
                if (k == default_length - 1)
                {
                    if (tab[i].ship_class_arr[j].ship_parts[k].type == 'c')
                        tab[i].ship_class_arr[j].ship_parts[k].type = 'b';
                    else
                        tab[i].ship_class_arr[j].ship_parts[k].type = 'e';
                }


            }


        }
        if (default_length == 5)
        {
            tab[i].cannon_range = 0;

        }
        else
        {
            tab[i].cannon_range = default_length;
        }
        default_length--;
    }
    return tab;
}
void InitializePlayers(Player* player, GameState* game, int id, int* flag, int playerid) {
    if (id == 0)
    {
        int shipAmount[5] = { 0,1,2,3,4 };
        if (playerid == A || playerid == -1)
        {
            flag[A] = YES;

            player[A] = {
                    'A',
                    InitializeShipArrays(shipAmount),
                    YES,
                    {0,(game->board_size.y / 2) - 1},
                    {0,game->board_size.x - 1}
            };
        }
        if (playerid == B || playerid == -1)
        {
            flag[B] = YES;
            player[B] = {
                    'B',
                    InitializeShipArrays(shipAmount),
                    NO,
                    { (game->board_size.y / 2) + 1,game->board_size.y - 1 },
                    {0,game->board_size.x - 1}
            };
        }
    }
    else {
        char playerX;
        int shipAmount[5], playerID;
        shipAmount[0] = 0;
        cin >> playerX;
        playerID = playerX - 65;
        for (int i = 1; i < 5; i++)
            cin >> shipAmount[i];

        if (playerID == A)
        {
            if (flag[A] == NO)
            {
                flag[A] = YES;
                player[A] = { playerX,InitializeShipArrays(shipAmount),YES,{0,(game->board_size.y / 2) - 1},{0,game->board_size.x - 1} };
            }
            else
                player[A].ship_arr = InitializeShipArrays(shipAmount);
        }
        else
        {
            if (flag[B] == NO)
            {
                player[B] = { playerX,InitializeShipArrays(shipAmount),NO,{ (game->board_size.y / 2) + 1,game->board_size.y - 1 },{0,game->board_size.x - 1} };
                flag[B] = YES;
            }
            else
                player[B].ship_arr = InitializeShipArrays(shipAmount);
        }
    }
}
int DirectionConversion(char dir) {
    if (dir == 'N')
        return N;
    if (dir == 'S')
        return S;
    if (dir == 'E')
        return E;
    if (dir == 'W')
        return W;
    return 0;
}
int ShipClassConversion(char* klasa) {
    if (strcmp(klasa, "CAR") == 0)
        return 1;
    if (strcmp(klasa, "BAT") == 0)
        return 2;
    if (strcmp(klasa, "CRU") == 0)
        return 3;
    if (strcmp(klasa, "DES") == 0)
        return 4;
    return 0;
}
bool ShipInGoodCoords(Player* player, Arguments* komenda, int shipLength, GameState* game)
{
    int testCoord, minY, maxY, minX, maxX, y = komenda->y, x = komenda->x;;
    if (!strcmp(komenda->action, "SHIP")) {

        minY = 0; maxY = game->board_size.y - 1; minX = 0; maxX = game->board_size.x - 1;
    }
    else {
        minY = player[komenda->playerID].rangeCordsY.y; maxY = player[komenda->playerID].rangeCordsY.x; minX = player[komenda->playerID].rangeCordsX.y; maxX = player[komenda->playerID].rangeCordsX.x;
    }

    if (komenda->dirByNum == N)
    {
        testCoord = komenda->y + shipLength - 1;
        if (y >= minY && y <= maxY && testCoord >= minY && testCoord <= maxY)
            return 1;
    }
    if (komenda->dirByNum == S) {
        testCoord = komenda->y - shipLength + 1;
        if (y >= minY && y <= maxY && testCoord >= minY && testCoord <= maxY)
            return 1;
    }
    if (komenda->dirByNum == W) {
        testCoord = komenda->x + shipLength - 1;
        if (x >= minX && x <= maxX && testCoord >= minX && testCoord <= maxX)
            return 1;
    }
    if (komenda->dirByNum == E) {
        testCoord = komenda->x - shipLength + 1;
        if (x >= minX && x <= maxX && testCoord >= minX && testCoord <= maxX)
            return 1;
    }

    return 0;
}
void error_output(Player* player, Arguments* komenda, int id) {
    cout << "INVALID OPERATION \"";
    if (!strcmp(komenda->action, "REEF"))
    {
        cout << komenda->action << " " << komenda->y << " " << komenda->x << "\":" << " REEF IS NOT PLACED ON BOARD";
    }

    if (!strcmp(komenda->action, "PLACE_SHIP"))
    {
        cout << komenda->action << " " << komenda->y << " " << komenda->x << " " << komenda->dirByString << " " << komenda->shipIndex << " " << komenda->classByString << "\":";
        if (id == TOO_CLOSE)
            cout << " PLACING SHIP TOO CLOSE TO OTHER SHIP";
        if (id == REEF)
            cout << " PLACING SHIP ON REEF";
        if (id == SHIP_PRESENT)
            cout << " SHIP ALREADY PRESENT";
        if (id == ALL_SET)
            cout << " ALL SHIPS OF THE CLASS ALREADY SET";
        if (id == NOT_IN_POSITION)
            cout << " NOT IN STARTING POSITION";
        return;
    }
    if (!strcmp(komenda->action, "SHIP"))
    {
        int j = 0;
        cout << komenda->action << " " << komenda->playerIDbychar << " " << komenda->y << " " << komenda->x << " " << komenda->dirByString << " " << komenda->shipIndex << " " << komenda->classByString << " ";
        while (j != 5 && (player[komenda->playerID].ship_arr[komenda->classByNum].ship_class_arr[komenda->shipIndex].ship_parts[j].current_state == '+' || player[komenda->playerID].ship_arr[komenda->classByNum].ship_class_arr[komenda->shipIndex].ship_parts[j].current_state == 'x'))
        {
            if (player[komenda->playerID].ship_arr[komenda->classByNum].ship_class_arr[komenda->shipIndex].ship_parts[j].current_state == '+')
                cout << 1; else cout << 0; j++;
        }
        if (id == TOO_CLOSE)
            cout << "\":" << " PLACING SHIP TOO CLOSE TO OTHER SHIP";
        else if (id == REEF)
            cout << "\":" << " PLACING SHIP ON REEF";
        else if (id == SHIP_PRESENT)
            cout << "\":" << " SHIP ALREADY PRESENT";
        else if (id == ALL_SET)
            cout << "\":" << " ALL SHIPS OF THE CLASS ALREADY SET";
        else if (id == NOT_IN_POSITION)
            cout << "\":" << " NOT IN STARTING POSITION";
        return;
    }
    if (!strcmp(komenda->action, "MOVE"))
    {
        cout << komenda->action << " " << komenda->shipIndex << " " << komenda->classByString << " " << komenda->move_action << "\":";
        if (id == TOO_CLOSE)
            cout << " PLACING SHIP TOO CLOSE TO OTHER SHIP";
        else if (id == REEF)
            cout << " PLACING SHIP ON REEF";
        else if (id == OUT_OF_BOARD)
            cout << " SHIP WENT FROM BOARD";
        else if (id == NO_MOVES)
            cout << " SHIP MOVED ALREADY";
        else if (id == ENGINE_DESTROYED)
            cout << " SHIP CANNOT MOVE";
        return;
    }
    if (!strcmp(komenda->action, "SHOOT"))
    {
        if (komenda->is_extended)
            cout << komenda->action << " " << komenda->shipIndex << " " << komenda->classByString << " " << komenda->y << " " << komenda->x << "\":";
        else
            cout << komenda->action << " " << komenda->y << " " << komenda->x << "\":";

        if (id == FIELD_NOT_EXIST)
            cout << " FIELD DOES NOT EXIST";
        else if (id == CANNON_DESTROYED)
            cout << " SHIP CANNOT SHOOT";
        else if (id == NOT_ALL_SHIPS_PLACED)
            cout << " NOT ALL SHIPS PLACED";
        else if (id == TOO_MANY_SHOOTS)
            cout << " TOO MANY SHOOTS";
        else if (id == SHOOT_TOO_FAR)
            cout << " SHOOTING TOO FAR";
        return;
    }
    if (!strcmp(komenda->action, "SPY"))
    {
        cout << komenda->action << " " << komenda->shipIndex << " " << komenda->y << " " << komenda->x << "\":";
        if (id == TOO_MANY_SHOOTS)
            cout << " TOO MANY SHOOTS";
        else if (id == ALL_PLANES_SENT)
            cout << " ALL PLANES SENT";
        else if (id == CANNON_DESTROYED)
            cout << " CANNOT SEND PLANE";
    }
}
void DestroyRadar(Player* player, Arguments* komenda, int j)
{
    if (player[komenda->playerID].ship_arr[komenda->classByNum].ship_class_arr[komenda->shipIndex].ship_parts[j].current_state == 'x')
        if (player[komenda->playerID].ship_arr[komenda->classByNum].ship_class_arr[komenda->shipIndex].ship_parts[j].type == 'r')
            player[komenda->playerID].ship_arr[komenda->classByNum].ship_class_arr[j].radar_range = 1;
}
void PlaceShipOnBoard(Player* player, Arguments* komenda, char* board[], GameState* game) {

    int length = player[komenda->playerID].ship_arr[komenda->classByNum].length;
    if (!strcmp(komenda->action, "MOVE") || ShipInGoodCoords(player, komenda, length, game))
    {
        player[komenda->playerID].ship_arr[komenda->classByNum].ship_class_arr[komenda->shipIndex].dir = komenda->dirByNum;
        player[komenda->playerID].ship_arr[komenda->classByNum].ship_class_arr[komenda->shipIndex].dirString = komenda->dirByString;
        if (komenda->dirByNum == N)
        {

            {
                for (int k = komenda->y, j = 0; k < length + komenda->y; k++, j++)
                {

                    if (k != 0 && k == komenda->y)
                        if (board[k - 1][komenda->x] == '+')
                        {
                            error_output(player, komenda, TOO_CLOSE);
                            EXIT_PROGRAM;
                        }
                    if (komenda->x != 0)
                        if (board[k][komenda->x - 1] == '+')
                        {
                            error_output(player, komenda, TOO_CLOSE);
                            EXIT_PROGRAM;
                        }
                    if (komenda->x != game->board_size.x - 1)
                        if (board[k][komenda->x + 1] == '+')
                        {
                            error_output(player, komenda, TOO_CLOSE);
                            EXIT_PROGRAM;
                        }
                    if (k == length + komenda->y - 1)
                        if (komenda->x != game->board_size.x - 1)
                            if (board[k + 1][komenda->x] == '+')
                            {
                                error_output(player, komenda, TOO_CLOSE);
                                EXIT_PROGRAM;
                            }
                    if (board[k][komenda->x] == '#')
                    {
                        error_output(player, komenda, REEF);
                        EXIT_PROGRAM;
                    }
                    board[k][komenda->x] = player[komenda->playerID].ship_arr[komenda->classByNum].ship_class_arr[komenda->shipIndex].ship_parts[j].current_state;
                    player[komenda->playerID].ship_arr[komenda->classByNum].ship_class_arr[komenda->shipIndex].ship_parts[j].coords = { k,komenda->x };
                    DestroyRadar(player, komenda, j);
                }
            }
            return;
        }
        if (komenda->dirByNum == S)
        {
            for (int k = komenda->y, j = 0; k > komenda->y - length; k--, j++)
            {

                if (k != game->board_size.x - 1 && k == komenda->y)
                    if (board[k + 1][komenda->x] == '+')
                    {
                        error_output(player, komenda, TOO_CLOSE);
                        EXIT_PROGRAM;
                    }
                if (komenda->x != 0)
                    if (board[k][komenda->x - 1] == '+')
                    {
                        error_output(player, komenda, TOO_CLOSE);
                        EXIT_PROGRAM;
                    }
                if (komenda->x != game->board_size.x - 1)
                    if (board[k][komenda->x + 1] == '+')
                    {
                        error_output(player, komenda, TOO_CLOSE);
                        EXIT_PROGRAM;
                    }
                if (k == komenda->y - length + 1)
                    if (komenda->y < 0)
                        if (board[k - 1][komenda->x] == '+')
                        {
                            error_output(player, komenda, TOO_CLOSE);
                            EXIT_PROGRAM;
                        }
                if (board[k][komenda->x] == '#')
                {
                    error_output(player, komenda, REEF);
                    EXIT_PROGRAM;
                }
                board[k][komenda->x] = player[komenda->playerID].ship_arr[komenda->classByNum].ship_class_arr[komenda->shipIndex].ship_parts[j].current_state;
                player[komenda->playerID].ship_arr[komenda->classByNum].ship_class_arr[komenda->shipIndex].ship_parts[j].coords = { k,komenda->x };
                DestroyRadar(player, komenda, j);
            }
            return;
        }
        if (komenda->dirByNum == W)
        {
            for (int k = komenda->x, j = 0; k < komenda->x + length; k++, j++)
            {

                if (komenda->y != 0)
                    if (board[komenda->y - 1][k] == '+')
                    {
                        error_output(player, komenda, TOO_CLOSE);
                        EXIT_PROGRAM;
                    }
                if (komenda->y != game->board_size.y - 1)

                    if (board[komenda->y + 1][k] == '+') {
                        error_output(player, komenda, TOO_CLOSE);
                        EXIT_PROGRAM;
                    }
                if (k != 0 && k == komenda->x)
                    if (board[komenda->y][k - 1] == '+')
                    {
                        error_output(player, komenda, TOO_CLOSE);
                        EXIT_PROGRAM;
                    }
                if (k == komenda->x + length - 1)
                    if (komenda->x + length != game->board_size.x - 1)
                        if (board[komenda->y][k + 1] == '+')
                        {
                            error_output(player, komenda, TOO_CLOSE);
                            EXIT_PROGRAM;
                        }
                if (board[komenda->y][k] == '#')
                {
                    error_output(player, komenda, REEF);
                    EXIT_PROGRAM;
                }

                board[komenda->y][k] = player[komenda->playerID].ship_arr[komenda->classByNum].ship_class_arr[komenda->shipIndex].ship_parts[j].current_state;
                player[komenda->playerID].ship_arr[komenda->classByNum].ship_class_arr[komenda->shipIndex].ship_parts[j].coords = { komenda->y,k };
                DestroyRadar(player, komenda, j);
            }

            return;
        }
        if (komenda->dirByNum == E)
        {
            for (int k = komenda->x, j = 0; k > komenda->x - length; k--, j++)
            {

                if (komenda->y != 0)
                    if (board[komenda->y - 1][k] == '+')
                    {
                        error_output(player, komenda, TOO_CLOSE);
                        EXIT_PROGRAM;
                    }
                if (komenda->y != game->board_size.y - 1)
                    if (board[komenda->y + 1][k] == '+') {
                        error_output(player, komenda, TOO_CLOSE);
                        EXIT_PROGRAM;
                    }
                if (k != game->board_size.x - 1 && k == komenda->x)
                    if (board[komenda->y][k - 1] == '+')
                    {
                        error_output(player, komenda, TOO_CLOSE);
                        EXIT_PROGRAM;
                    }
                if (k == komenda->x - length + 1)
                    if (komenda->x - length != 0)
                        if (board[komenda->y][k - 1] == '+')
                        {
                            error_output(player, komenda, TOO_CLOSE);
                            EXIT_PROGRAM;
                        }
                if (board[komenda->y][k] == '#')
                {
                    error_output(player, komenda, REEF);
                    EXIT_PROGRAM;
                }

                board[komenda->y][k] = player[komenda->playerID].ship_arr[komenda->classByNum].ship_class_arr[komenda->shipIndex].ship_parts[j].current_state;
                player[komenda->playerID].ship_arr[komenda->classByNum].ship_class_arr[komenda->shipIndex].ship_parts[j].coords = { komenda->y,k };
                DestroyRadar(player, komenda, j);
            }
        }
    }
    return;
}
void InitializeShipCondition(Player* player, Arguments* komenda) {
    int length = player[komenda[0].playerID].ship_arr[komenda[0].classByNum].length;
    if (!strcmp(komenda[0].action, "PLACE_SHIP"))
    {
        for (int i = 0; i < length; i++)
        {
            player[komenda[0].playerID].ship_arr[komenda[0].classByNum].ship_class_arr[komenda[0].shipIndex].ship_parts[i].current_state = '+';
        }
    }
    if (!strcmp(komenda[0].action, "SHIP"))
    {

        char input;
        for (int i = 0; i < length; i++)
        {
            cin >> input;
            if (input == '1')
                player[komenda[0].playerID].ship_arr[komenda[0].classByNum].ship_class_arr[komenda[0].shipIndex].ship_parts[i].current_state = '+';
            else
                player[komenda[0].playerID].ship_arr[komenda[0].classByNum].ship_class_arr[komenda[0].shipIndex].ship_parts[i].current_state = 'x';
        }
    }
}
void PlaceShipCheck(Player* player, Arguments* komenda, char* board[], GameState* game) {

    if (player[komenda->playerID].ship_arr[komenda->classByNum].maxAmount != 0)
        if (player[komenda->playerID].ship_arr[komenda->classByNum].ship_class_arr[komenda->shipIndex].is_placed == YES)
        {
            error_output(player, komenda, SHIP_PRESENT);
            EXIT_PROGRAM;
        }
    if (player[komenda->playerID].ship_arr[komenda->classByNum].amount == 0)
    {
        error_output(player, komenda, ALL_SET);
        EXIT_PROGRAM;
    }
    InitializeShipCondition(player, komenda);

    if (!ShipInGoodCoords(player, komenda, player[komenda->playerID].ship_arr[komenda->classByNum].length, game) && strcmp(komenda->action, "MOVE"))
    {
        error_output(player, komenda, NOT_IN_POSITION);
        EXIT_PROGRAM;
    }

    else
    {

        PlaceShipOnBoard(player, komenda, board, game);
        player[komenda->playerID].ship_arr[komenda->classByNum].amount--;
        player[komenda->playerID].ship_arr[komenda->classByNum].ship_class_arr[komenda->shipIndex].is_placed = YES;
    }
}
bool CheckIfAllShipsPlaced(Player* player) {
    for (int i = 1; i < 5; i++)
    {
        if (player[A].ship_arr[i].amount != 0)
            return 0;
        if (player[B].ship_arr[i].amount != 0)
            return 0;
    }
    return 1;

}
void ClearShip(Player* player, int playerID, char* board[], int shipIndex, int classID) //usuwanie statku z planszy, aby by³o mo¿na go przestawiæ
{
    int y = player[playerID].ship_arr[classID].ship_class_arr[shipIndex].ship_parts[0].coords.y;
    int x = player[playerID].ship_arr[classID].ship_class_arr[shipIndex].ship_parts[0].coords.x;
    int length = player[playerID].ship_arr[classID].length;


    if (player[playerID].ship_arr[classID].ship_class_arr[shipIndex].dir == N)
        for (int i = y; i < y + length; i++)
            board[i][x] = ' ';
    if (player[playerID].ship_arr[classID].ship_class_arr[shipIndex].dir == S)
        for (int i = y; i > y - length; i--)
            board[i][x] = ' ';
    if (player[playerID].ship_arr[classID].ship_class_arr[shipIndex].dir == W)
        for (int i = x; i < x + length; i++)
            board[y][i] = ' ';
    if (player[playerID].ship_arr[classID].ship_class_arr[shipIndex].dir == E)
        for (int i = x; i > x - length; i--)
            board[y][i] = ' ';
}
void MoveShip(Player* player, int playerID, char* board[], GameState* game) {

    int index; char klasa[4], action;
    cin >> index >> klasa >> action;
    int classID = ShipClassConversion(klasa);
    int dir = player[playerID].ship_arr[classID].ship_class_arr[index].dir;
    char dirChar = player[playerID].ship_arr[classID].ship_class_arr[index].dirString;
    int y = player[playerID].ship_arr[classID].ship_class_arr[index].ship_parts[0].coords.y, x = player[playerID].ship_arr[classID].ship_class_arr[index].ship_parts[0].coords.x;
    char subaction[5] = "MOVE";
    Arguments komenda;
    Arguments* argptr = &komenda;
    argptr->action = subaction; argptr->shipIndex = index; argptr->move_action = action; argptr->classByString = klasa;
    if (game->extended_ships == YES)
        if (player[playerID].ship_arr[classID].ship_class_arr[index].ship_parts[player[playerID].ship_arr[classID].length - 1].current_state == 'x')
        {
            error_output(player, argptr, ENGINE_DESTROYED);
            EXIT_PROGRAM;
        }

    if (dir == N)
        y -= 1;
    if (dir == S)
        y += 1;
    if (dir == W)
        x -= 1;
    if (dir == E)
        x += 1;
    ClearShip(player, playerID, board, index, classID);

    if (player[playerID].ship_arr[classID].ship_class_arr[index].moves > 0)
    {
        player[playerID].ship_arr[classID].ship_class_arr[index].moves--;
        if (action == 'F')
        {
            argptr[0] = { subaction,y,x,dirChar,dir,index,klasa,classID,playerID,player[playerID].id,action };

        }

        else if (action == 'R') {
            if (dir == N)
            {
                dir = E;
                dirChar = 'E';
                x = x + player[playerID].ship_arr[classID].length - 1;
                argptr[0] = { subaction,y,x,dirChar,dir,index,klasa,classID,playerID,player[playerID].id,action };
            }
            else if (dir == S)
            {
                dir = W;
                x = x - player[playerID].ship_arr[classID].length + 1;
                argptr[0] = { subaction,y,x,dirChar,dir,index,klasa,classID,playerID,player[playerID].id,action };
            }
            else if (dir == W)
            {
                dir = N;
                y = y - player[playerID].ship_arr[classID].length + 1;
                argptr[0] = { subaction,y,x,dirChar,dir,index,klasa,classID,playerID,player[playerID].id,action };
            }
            else if (dir == E)
            {
                dir = S;
                y = y + player[playerID].ship_arr[classID].length - 1;
                argptr[0] = { subaction,y,x,dirChar,dir,index,klasa,classID,playerID,player[playerID].id,action };
            }
        }
        else if (action == 'L') {
            if (dir == N)
            {
                dir = W;
                dirChar = 'W';
                x = x - player[playerID].ship_arr[classID].length + 1;
                argptr[0] = { subaction,y,x,dirChar,dir,index,klasa,classID,playerID,player[playerID].id,action };
            }
            else if (dir == S)
            {
                dir = E;
                dirChar = 'E';
                x = x + player[playerID].ship_arr[classID].length - 1;
                argptr[0] = { subaction,y,x,dirChar,dir,index,klasa,classID,playerID,player[playerID].id,action };
            }
            else if (dir == W)
            {
                dir = S;
                dirChar = 'S';
                y = y + player[playerID].ship_arr[classID].length - 1;
                argptr[0] = { subaction,y,x,dirChar,dir,index,klasa,classID,playerID,player[playerID].id,action };
            }
            else if (dir == E)
            {
                dir = N;
                dirChar = 'E';
                y = y - player[playerID].ship_arr[classID].length + 1;
                argptr[0] = { subaction,y,x,dirChar,dir,index,klasa,classID,playerID,player[playerID].id,action };
            }
        }
        if (y < 0 || y >= game->board_size.y || x < 0 || x >= game->board_size.x)
        {
            error_output(player, argptr, OUT_OF_BOARD);
            EXIT_PROGRAM;
        }

        player[playerID].ship_arr[classID].amount++;
        player[playerID].ship_arr[classID].ship_class_arr[index].is_placed = NO;

        PlaceShipCheck(player, argptr, board, game);
        if (player[playerID].ship_arr[classID].ship_class_arr[index].moves == 0)
            return;
    }

    else {
        error_output(player, argptr, NO_MOVES);
        EXIT_PROGRAM;
    };

}
void ShootBasic(char* board[], Player* player, GameState* game, int playerID)
{
    int x, y;
    cin >> y >> x;
    if (playerID == 0) playerID = 1;
    else playerID = 0;
    char action[6] = "SHOOT";
    Arguments komenda;
    Arguments* argptr = &komenda;
    argptr->action = action; argptr->y = y; argptr->x = x;
    if (!CheckIfAllShipsPlaced(player))
    {
        error_output(player, argptr, NOT_ALL_SHIPS_PLACED);
        EXIT_PROGRAM;
    }
    if (y >= game->board_size.y || y < 0 || x >= game->board_size.x || x < 0)
    {
        error_output(player, argptr, FIELD_NOT_EXIST);
        EXIT_PROGRAM;
    }

    else {
        if (board[y][x] == '+')
        {
            board[y][x] = 'x';
            for (int i = 1; i < 5; i++)
                for (int j = 0; j < player[playerID].ship_arr[i].maxAmount; j++)
                    for (int k = 0; k < player[playerID].ship_arr[i].length; k++)
                        if (player[playerID].ship_arr[i].ship_class_arr[j].ship_parts[k].coords.y == y && player[playerID].ship_arr[i].ship_class_arr[j].ship_parts[k].coords.x == x)
                        {
                            player[playerID].ship_arr[i].ship_class_arr[j].ship_parts[k].current_state = 'x';
                        }
        }
    }
    return;
}
bool CalcDist(int y1, int x1, int y2, int x2, int range)
{
    if (range == 0)
        return 1;
    if (double(sqrt(((y1 - y2) * (y1 - y2)) + ((x1 - x2) * (x1 - x2)))) <= double(range))
        return 1;
    return 0;
}
void ShootExtended(Player* player, int playerID, char* board[], GameState* game) {
    int index = 0, y, x; char klasa[4], action[6] = "SHOOT";
    Arguments arguments;
    Arguments* argptr = &arguments;
    cin >> index >> klasa >> y >> x;
    argptr->shipIndex = index; argptr->classByString = klasa; argptr->y = y; argptr->x = x; argptr->action = action; argptr->is_extended = YES;
    int classID = ShipClassConversion(klasa);
    if (!CheckIfAllShipsPlaced(player))
    {
        error_output(player, argptr, NOT_ALL_SHIPS_PLACED);
        EXIT_PROGRAM;
    }

    if (y >= game->board_size.y || y < 0 || x >= game->board_size.x || x < 0)
    {
        error_output(player, argptr, FIELD_NOT_EXIST);
        EXIT_PROGRAM;
    }

    if (player[playerID].ship_arr[classID].ship_class_arr[index].ship_parts[1].current_state == '+')
    {
        int y_cannon = player[playerID].ship_arr[classID].ship_class_arr[index].ship_parts[1].coords.y;
        int x_cannon = player[playerID].ship_arr[classID].ship_class_arr[index].ship_parts[1].coords.x;
        int cannon_range = player[playerID].ship_arr[classID].cannon_range;
        if (player[playerID].ship_arr[classID].ship_class_arr[index].shoots == 0)
        {

            error_output(player, argptr, TOO_MANY_SHOOTS);
            EXIT_PROGRAM;
        }
        if (CalcDist(y_cannon, x_cannon, y, x, cannon_range))
        {
            player[playerID].ship_arr[classID].ship_class_arr[index].shoots--;
            if (board[y][x] == '+')
            {


                playerID = !playerID;

                board[y][x] = 'x';
                for (int i = 1; i < 5; i++)
                    for (int j = 0; j < player[playerID].ship_arr[i].maxAmount; j++)
                        for (int k = 0; k < player[playerID].ship_arr[i].length; k++)
                            if (player[playerID].ship_arr[i].ship_class_arr[j].ship_parts[k].coords.y == y && player[playerID].ship_arr[i].ship_class_arr[j].ship_parts[k].coords.x == x)
                            {
                                player[playerID].ship_arr[i].ship_class_arr[j].ship_parts[k].current_state = 'x';
                                if (player[playerID].ship_arr[i].ship_class_arr[j].ship_parts[k].type == 'r')
                                    player[playerID].ship_arr[i].ship_class_arr[j].radar_range = 1;
                            }
                playerID = !playerID;
            }


        }
        else
        {
            error_output(player, argptr, SHOOT_TOO_FAR);
            EXIT_PROGRAM;
        }

    }
    else if (player[playerID].ship_arr[classID].ship_class_arr[index].ship_parts[1].current_state == 'x')
    {
        error_output(player, argptr, CANNON_DESTROYED);
        EXIT_PROGRAM;
    }
}
void PrintPlayerInformation(Player* player, int id) //PRINT z poziomu gracza
{
    char playerID, dir, classByString[4]; int y, x;
    if (id == 1)
        playerID = 'B';
    else
        playerID = 'A';
    cout << "SET_FLEET " << playerID << " ";
    for (int i = CAR; i <= DES; i++)
        cout << player[id].ship_arr[i].maxAmount << " ";

    for (int i = CAR; i <= DES; i++)
        for (int j = 0; j < player[id].ship_arr[i].maxAmount; j++)
        {
            if (i == CAR)
                strcpy(classByString, "CAR");
            if (i == DES)
                strcpy(classByString, "DES");
            if (i == BAT)
                strcpy(classByString, "BAT");
            if (i == CRU)
                strcpy(classByString, "CRU");
            y = player[id].ship_arr[i].ship_class_arr[j].ship_parts[0].coords.y;
            x = player[id].ship_arr[i].ship_class_arr[j].ship_parts[0].coords.x;
            dir = player[id].ship_arr[i].ship_class_arr[j].dirString;
            cout << "\nSHIP " << playerID << " " << y << " " << x << " " << dir << " " << j << " " << classByString << " ";
            for (int k = 0; k < player[id].ship_arr[i].length; k++)
            {
                if (player[id].ship_arr[i].ship_class_arr[j].ship_parts[k].current_state == '+')
                    cout << 1;
                else
                    cout << 0;
            }

        }
    cout << "\n";
}
void Save(Player* player, GameState* game, vector* reefs)
{
    cout << "[state]\n"
        << "BOARD_SIZE " << game->board_size.y << " " << game->board_size.x << "\n" <<
        "NEXT_PLAYER "; if (game->firstPlayer == A) cout << "A\n"; else cout << "B\n";
    cout << "INIT_POSITION A " << player[A].rangeCordsY.y << " " << player[A].rangeCordsX.y << " " << player[A].rangeCordsY.x << " " << player[A].rangeCordsX.x << "\n";
    PrintPlayerInformation(player, A);
    cout << "INIT_POSITION B " << player[B].rangeCordsY.y << " " << player[B].rangeCordsX.y << " " << player[B].rangeCordsY.x << " " << player[B].rangeCordsX.x << "\n";
    PrintPlayerInformation(player, B);
    for (int i = 0; i < reefs->i; i++)
        cout << "REEF" << " " << reefs->ptr[i].y << " " << reefs->ptr[i].x << "\n";
    if (game->extended_ships)
        cout << "EXTENDED_SHIPS\n";
    cout << "[state]\n";



}
void SendSpy(Player* player, int playerID, char* action)
{
    int index, y, x;
    Arguments argArr;
    Arguments* argptr = &argArr;

    cin >> index >> y >> x;
    argptr->shipIndex = index; argptr->y = y; argptr->x = x; argptr->action = action;
    if (player[playerID].ship_arr[CAR].ship_class_arr[index].shoots == 0)
    {
        error_output(player, argptr, TOO_MANY_SHOOTS);
        EXIT_PROGRAM;
    }
    if (player[playerID].ship_arr[CAR].ship_class_arr[index].spy_amount == 0)
    {
        error_output(player, argptr, ALL_PLANES_SENT);
        EXIT_PROGRAM;
    }
    else if (player[playerID].ship_arr[CAR].ship_class_arr[index].ship_parts[1].current_state == 'x')
    {
        error_output(player, argptr, CANNON_DESTROYED);
        EXIT_PROGRAM;
    }
    int spyindex = player[playerID].ship_arr[CAR].ship_class_arr[index].spy_index;
    player[playerID].ship_arr[CAR].ship_class_arr[index].shoots--;
    player[playerID].ship_arr[CAR].ship_class_arr[index].spy_amount--;
    player[playerID].ship_arr[CAR].ship_class_arr[index].planes[spyindex].y = y; player[playerID].ship_arr[CAR].ship_class_arr[index].planes[spyindex].x = x;
    player[playerID].ship_arr[CAR].ship_class_arr[index].spy_index++;

}
void NextTurn(Player* player) {
    player[A].isTurn = !player[A].isTurn;
    player[B].isTurn = !player[B].isTurn;
    if (CheckIfAllShipsPlaced(player))
    {
        for (int i = 1; i < 5; i++)
        {
            for (int j = 0; j < player[A].ship_arr[i].maxAmount; j++)
            {
                player[A].ship_arr[i].ship_class_arr[j].moves = player[A].ship_arr[i].maxMoves;
                player[A].ship_arr[i].ship_class_arr[j].shoots = player[A].ship_arr[i].maxShoots;
                if (i == 1)
                {
                    player[A].ship_arr[i].ship_class_arr[j].spy_amount = 5;
                    player[A].ship_arr[i].ship_class_arr[j].spy_index = 0;
                }
            }
            for (int j = 0; j < player[B].ship_arr[i].maxAmount; j++)
            {
                player[B].ship_arr[i].ship_class_arr[j].moves = player[B].ship_arr[i].maxMoves;
                player[B].ship_arr[i].ship_class_arr[j].shoots = player[B].ship_arr[i].maxShoots;
                if (i == 1)
                {
                    player[B].ship_arr[i].ship_class_arr[j].spy_amount = 5;
                    player[B].ship_arr[i].ship_class_arr[j].spy_index = 0;
                }
            }
        }


    }
}
void CheckWin(Player* player)
{
    if (CheckIfAllShipsPlaced(player)) {
        if (!CheckParts(player, 0))
        {
            cout << "B won";
            EXIT_PROGRAM;
        }
        if (!CheckParts(player, 1))
        {
            cout << "A won";
            EXIT_PROGRAM;
        }
    }
}
void PrintPlanes(Player* player, int playerID, char* board[], GameState* game)
{
    int  yMin, yMax, xMin, xMax;
    for (int g = 0; g < player[playerID].ship_arr[CAR].maxAmount; g++)
    {
        int index = player[playerID].ship_arr[CAR].ship_class_arr[g].spy_index;
        for (int s = 0; s < index; s++)
        {
            yMin = player[playerID].ship_arr[CAR].ship_class_arr[g].planes[s].y - 1;
            xMin = player[playerID].ship_arr[CAR].ship_class_arr[g].planes[s].x - 1;
            yMax = player[playerID].ship_arr[CAR].ship_class_arr[g].planes[s].y + 1;
            xMax = player[playerID].ship_arr[CAR].ship_class_arr[g].planes[s].x + 1;
            if (yMin < 0)
                yMin = 0;
            if (xMin < 0)
                xMin = 0;
            if (yMax >= game->board_size.y)
                yMax = game->board_size.y - 1;
            if (xMax >= game->board_size.x)
                xMax = game->board_size.x - 1;
            for (int v = yMin; v <= yMax; v++)
                for (int az = xMin; az <= xMax; az++)
                    board[v][az] = ' ';

        }
    }
}
void PrintPlayer(Player* player, int playerID, char* board[], GameState* game, int num)
{
    char** playerBoard = new char* [game->board_size.y];
    for (int i = 0; i < game->board_size.y; i++)
        playerBoard[i] = new char[game->board_size.x];

    for (int i = 0; i < game->board_size.y; i++)
        for (int j = 0; j < game->board_size.x; j++)
            playerBoard[i][j] = '?';

    for (int i = 1; i < 5; i++)
        for (int j = 0; j < player[playerID].ship_arr[i].maxAmount; j++)
            for (int k = 0; k < player[playerID].ship_arr[i].length; k++)
            {
                if (player[playerID].ship_arr[i].ship_class_arr[j].is_placed == YES)
                {
                    int y = player[playerID].ship_arr[i].ship_class_arr[j].ship_parts[k].coords.y;
                    int x = player[playerID].ship_arr[i].ship_class_arr[j].ship_parts[k].coords.x;
                    if (num == 0)
                        playerBoard[y][x] = player[playerID].ship_arr[i].ship_class_arr[j].ship_parts[k].current_state;
                    else if (num == 1)//jezeli wystepuje PRINT 1, to zamiast wypisywania '+' i 'x' wypisuje wartoœci zale¿ne od extended printing
                    {
                        if (player[playerID].ship_arr[i].ship_class_arr[j].ship_parts[k].current_state == 'x')
                        {
                            playerBoard[y][x] = 'x';
                        }
                        else if (player[playerID].ship_arr[i].ship_class_arr[j].ship_parts[k].current_state == '+')
                        {

                            if (player[playerID].ship_arr[i].ship_class_arr[j].ship_parts[k].type == 'e' || player[playerID].ship_arr[i].ship_class_arr[j].ship_parts[k].type == 'b')
                            {
                                playerBoard[y][x] = '%';
                            }
                            else if (player[playerID].ship_arr[i].ship_class_arr[j].ship_parts[k].type == 'c')
                                playerBoard[y][x] = '!';
                            else if (player[playerID].ship_arr[i].ship_class_arr[j].ship_parts[k].type == 'r')
                                playerBoard[y][x] = '@';
                            else
                                playerBoard[y][x] = '+';

                        }


                    }
                    if (player[playerID].ship_arr[i].ship_class_arr[j].ship_parts[k].type == 'r')
                    {
                        for (int h = 0; h < game->board_size.y; h++)
                        {
                            for (int z = 0; z < game->board_size.x; z++)
                            {
                                if (CalcDist(y, x, h, z, player[playerID].ship_arr[i].ship_class_arr[j].radar_range) && (playerBoard[h][z] != '+' && playerBoard[h][z] != 'x' && playerBoard[h][z] != '@'))
                                    playerBoard[h][z] = ' ';
                                PrintPlanes(player, playerID, playerBoard, game);
                                if (board[h][z] == '#')
                                    playerBoard[h][z] = '#';
                            }
                        }
                    }
                }
            }
    playerID = !playerID;
    for (int i = 1; i < 5; i++)
        for (int j = 0; j < player[playerID].ship_arr[i].maxAmount; j++)
            for (int k = 0; k < player[playerID].ship_arr[i].length; k++)
            {
                if (player[playerID].ship_arr[i].ship_class_arr[j].is_placed == YES)
                {
                    int y = player[playerID].ship_arr[i].ship_class_arr[j].ship_parts[k].coords.y;
                    int x = player[playerID].ship_arr[i].ship_class_arr[j].ship_parts[k].coords.x;
                    if (playerBoard[y][x] == ' ')
                    {
                        if (num == 0)
                            playerBoard[y][x] = player[playerID].ship_arr[i].ship_class_arr[j].ship_parts[k].current_state;
                        else if (num == 1)
                        {
                            if (player[playerID].ship_arr[i].ship_class_arr[j].ship_parts[k].current_state == 'x')
                            {
                                playerBoard[y][x] = 'x';
                            }
                            else if (player[playerID].ship_arr[i].ship_class_arr[j].ship_parts[k].current_state == '+')
                            {

                                if (player[playerID].ship_arr[i].ship_class_arr[j].ship_parts[k].type == 'e' || player[playerID].ship_arr[i].ship_class_arr[j].ship_parts[k].type == 'b')
                                {
                                    playerBoard[y][x] = '%';
                                }
                                else if (player[playerID].ship_arr[i].ship_class_arr[j].ship_parts[k].type == 'c')
                                    playerBoard[y][x] = '!';
                                else if (player[playerID].ship_arr[i].ship_class_arr[j].ship_parts[k].type == 'r')
                                    playerBoard[y][x] = '@';
                                else
                                    playerBoard[y][x] = '+';

                            }
                        }
                    }
                }



            }
    if (num == 0)
        for (int i = 0; i < game->board_size.y; i++)
        {
            for (int j = 0; j < game->board_size.x; j++)
                cout << playerBoard[i][j];
            cout << endl;
        }
    if (num == 1)
        PrintColumnsAndRows(playerBoard, game);
    delete[] playerBoard;
}
void InitializePosition(Player* player) {
    int tab[4];  char playerX;
    cin >> playerX;
    for (int i = 0; i < 4; i++)
        cin >> tab[i];
    int playerID = playerX - 65;
    player[playerID].rangeCordsY = { tab[0],tab[2] };
    player[playerID].rangeCordsX = { tab[1],tab[3] };
}
void PlaceReef(char* board[], GameState* game, int y, int x) {

    board[y][x] = '#';
}
char** SetBoardSize(GameState* game) {
    int x, y;
    cin >> y >> x;
    game->board_size.y = y;
    game->board_size.x = x;
    return InitializeBoard(game);
}
void InitializeGameState(GameState* game) {
    game->board_size = { DEFAULT_BOARD_SIZE_Y,DEFAULT_BOARD_SIZE_X };
    game->extended_ships = NO;
    game->firstPlayer = A;
}
void ClearMemory(Player* player, char* board[], vector** reefs, GameState* game)
{
    for (int i = 0; i < game->board_size.y; i++)
        delete[] board[i];
    delete[] board;

    for (int i = A; i <= B; i++)
        for (int j = CAR; j <= DES; j++)
            for (int k = 0; k < player[i].ship_arr[j].maxAmount; k++)
                delete[] player[i].ship_arr[j].ship_class_arr[k].ship_parts;

    for (int i = A; i <= B; i++)
        for (int j = CAR; j <= DES; j++)
            if (player[i].ship_arr[j].maxAmount != 0)
                delete[] player[i].ship_arr[j].ship_class_arr;

    for (int i = A; i <= B; i++)
        delete[] player[i].ship_arr;

    delete[] player;

    for (int i = 0; i < (*reefs)->allocated_size; i++)
        delete[](*reefs)->ptr;

    delete (*reefs);



}
int main()
{

    GameState game;
    GameState* gameptr = &game;
    InitializeGameState(gameptr);
    char** mainBoard = InitializeBoard(gameptr);
    Player* player = new Player[2];
    Arguments arg;
    Arguments* argptr = &arg;
    int flag[2] = { NO,NO };//sprawdza którzy gracze s¹ zaincjalizowani
    vector* reefsCoordinates = new vector[1];
    init(&reefsCoordinates);
    char action[15], subaction[25], playerX;
    char dir; char klasa[4]; int shipIndex; int playerID;
    int classByNum, dirByNum, y, x;
    while (cin >> action) {

        if (!strcmp(action, "[state]"))
        {
            while (cin >> subaction) {
                if (!strcmp(subaction, "PRINT"))
                    Print_state(mainBoard, player, gameptr);
                if (!strcmp(subaction, "SET_FLEET"))
                {
                    InitializePlayers(player, gameptr, 1, flag, SKIP);
                }
                if (!strcmp(subaction, "SHIP"))
                {
                    cin >> playerX >> y >> x >> dir >> shipIndex >> klasa;
                    playerID = playerX - 65;
                    if (flag[playerID] == NO)
                        InitializePlayers(player, gameptr, 0, flag, playerID);
                    classByNum = ShipClassConversion(klasa);
                    dirByNum = DirectionConversion(dir);
                    arg = { subaction,y,x,dir,dirByNum,shipIndex,klasa,classByNum,playerID,player[playerID].id,'t',0 };
                    PlaceShipCheck(player, argptr, mainBoard, gameptr);
                }
                if (!strcmp(subaction, "INIT_POSITION"))
                {
                    InitializePosition(player);
                }
                if (!strcmp(subaction, "EXTENDED_SHIPS"))
                    game.extended_ships = YES;
                if (!strcmp(subaction, "REEF"))
                {
                    cin >> y >> x;
                    PlaceReef(mainBoard, gameptr, y, x);
                    push_back(&reefsCoordinates, { y,x });

                }
                if (!strcmp(subaction, "NEXT_PLAYER")) {
                    cin >> playerX;
                    if (playerX == 'A')
                    {
                        player[A].isTurn = YES; player[B].isTurn = NO;

                    }
                    else
                    {
                        player[A].isTurn = NO; player[B].isTurn = YES;
                        game.firstPlayer = B;
                    }

                }
                if (!strcmp(subaction, "BOARD_SIZE"))
                {
                    mainBoard = SetBoardSize(gameptr);
                    InitializePlayers(player, gameptr, 0, flag, SKIP);
                }
                if (!strcmp(subaction, "SAVE"))
                {
                    Save(player, gameptr, reefsCoordinates);
                }
                if (!strcmp(subaction, "[state]"))
                    break;
            }
        }
        if (!strcmp(action, "[playerA]") || !strcmp(action, "[playerB]")) {


            playerX = action[7];
            playerID = int(playerX - 65);
            if (flag[playerID] == NO)
            {
                InitializePlayers(player, gameptr, 0, flag, playerID);
            }

            if (flag[!playerID] == NO)
            {
                InitializePlayers(player, gameptr, 0, flag, !playerID);
            }

            if (player[playerID].isTurn)
            {
                while (cin >> subaction)
                {

                    if (!strcmp(subaction, "PLACE_SHIP"))
                    {

                        cin >> y >> x >> dir >> shipIndex >> klasa;
                        classByNum = ShipClassConversion(klasa);
                        dirByNum = DirectionConversion(dir);
                        arg = { subaction,y,x,dir,dirByNum,shipIndex,klasa,classByNum,playerID,player[playerID].id,'t',0 };

                        PlaceShipCheck(player, argptr, mainBoard, gameptr);
                    }
                    if (!strcmp(subaction, "SHOOT"))
                    {
                        if (game.extended_ships)
                            ShootExtended(player, playerID, mainBoard, gameptr);
                        else
                            ShootBasic(mainBoard, player, gameptr, playerID);

                    }
                    if (!strcmp(subaction, "MOVE"))
                        MoveShip(player, playerID, mainBoard, gameptr);
                    if (!strcmp(subaction, "PRINT"))
                    {
                        int num;
                        cin >> num;
                        PrintPlayer(player, playerID, mainBoard, gameptr, num);

                    }
                    if (!strcmp(subaction, "SPY"))
                        SendSpy(player, playerID, subaction);
                    if ((!strcmp(subaction, "[playerA]") && !strcmp(action, "[playerA]")) || (!strcmp(subaction, "[playerB]") && !strcmp(action, "[playerB]")))
                        break;
                }
            }
            else
            {
                cout << "INVALID OPERATION \"" << action << " \":" << " THE OTHER PLAYER EXPECTED";
                EXIT_PROGRAM;
            }
            CheckWin(player);
            NextTurn(player);
        }
    }

    ClearMemory(player, mainBoard, &reefsCoordinates, gameptr);
}



