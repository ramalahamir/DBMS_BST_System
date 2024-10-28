#include <iostream>
using namespace std;

struct GamesPlayedNode
{
    int gameID;
    float hoursPlayed;
    int achievements;
};

class GamesPlayedTree
{
};

struct PlayerNode
{
    int playerID;
    string name;
    string phone_no;
    string email;
    string password;

    // each node will have its own games played tree structure
    GamesPlayedTree gamesPlayed;
};

class PlayerTree
{
};

struct GameNode
{
    int gameID;
    string name;
    string developer;
    string publisher;
    float fileSize;
    int downloads;
};

class GameTree
{
};

int main() { return 0; }