#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

struct GamesPlayedNode
{
    unsigned long long gameID;
    double hoursPlayed;
    int achievements;

    GamesPlayedNode *left;
    GamesPlayedNode *right;

    GamesPlayedNode(unsigned long long id, double hrs, int ach)
    {
        gameID = id;
        hoursPlayed = hrs;
        achievements = ach;
        left = right = nullptr;
    }
};

class GamesPlayedTree
{
    GamesPlayedNode *root;

  public:
    GamesPlayedTree() { root = nullptr; }
    ~GamesPlayedTree() {}

    void insertGamesPlayed(unsigned long long id, double hrsPlayed,
                           int achievements)
    {
        GamesPlayedNode *newNode =
            new GamesPlayedNode(id, hrsPlayed, achievements);
        insertNode(root, newNode);
    }

    GamesPlayedNode *insertNode(GamesPlayedNode *root, GamesPlayedNode *newNode)
    {
        if (root == nullptr)
            return newNode;

        // left subtree
        if (newNode->gameID < root->gameID)
            root->left = insertNode(root->left, newNode);

        // right subtree
        else if (newNode->gameID > root->gameID)
            root->right = insertNode(root->right, newNode);

        // if equal
        else
        {
            cout << "\nthis GamesPlayed record already exists!";
            cout << "\ncannot be re-entered!";
        }
        return root;
    }
};

struct PlayerNode
{
    unsigned long long playerID;
    string name;
    string phone_no;
    string email;
    string password;

    PlayerNode *left;
    PlayerNode *right;

    // each node will have its own games played tree structure
    GamesPlayedTree *gamesPlayed;

    PlayerNode(unsigned long long id, string nm, string phn, string em,
               string pw, GamesPlayedTree *&GamesPlayed_byPlayer)
    {
        playerID = id;
        name = nm;
        phone_no = phn;
        email = em;
        password = pw;
        left = right = nullptr;

        // reference to the original tree
        gamesPlayed = GamesPlayed_byPlayer;
    }
};

class PlayerTree
{
    PlayerNode *root;

  public:
    PlayerTree() { root = nullptr; }
    ~PlayerTree() {}

    void insertNewPlayer(unsigned long long id, string name, string phone,
                         string email, string password,
                         GamesPlayedTree *&GamesPlayed_byPlayer)
    {
        PlayerNode *newNode = new PlayerNode(id, name, phone, email, password,
                                             GamesPlayed_byPlayer);
        insertNode(root, newNode);
    }

    PlayerNode *insertNode(PlayerNode *root, PlayerNode *newNode)
    {
        if (root == nullptr)
            return newNode;

        // left subtree
        if (newNode->playerID < root->playerID)
            root->left = insertNode(root->left, newNode);

        // right subtree
        else if (newNode->playerID > root->playerID)
            root->right = insertNode(root->right, newNode);

        // if equal
        else
        {
            cout << "\nthis player ID already exists!";
            cout << "\ncannot be re-entered!";
        }
        return root;
    }
};

struct GameNode
{
    unsigned long long gameID;
    string name;
    string developer;
    string publisher;
    float fileSize;
    int downloads;

    GameNode *left;
    GameNode *right;

    GameNode(unsigned long long id, string nm, string dev, string pub,
             float size, int dwnlds)
    {
        gameID = id;
        name = nm;
        developer = dev;
        publisher = pub;
        fileSize = size;
        downloads = dwnlds;
        left = right = nullptr;
    }
};

class GameTree
{
    GameNode *root;

  public:
    GameTree() { root = nullptr; }
    ~GameTree() {}

    void insertNewGame(unsigned long long id, string name, string developer,
                       string publisher, float size, int downloads)
    {
        GameNode *newNode =
            new GameNode(id, name, developer, publisher, size, downloads);
        insertNode(root, newNode);
    }

    GameNode *insertNode(GameNode *root, GameNode *newNode)
    {
        if (root == nullptr)
            return newNode;

        // left subtree
        if (newNode->gameID < root->gameID)
            root->left = insertNode(root->left, newNode);

        // right subtree
        else if (newNode->gameID > root->gameID)
            root->right = insertNode(root->right, newNode);

        // if equal
        else
        {
            cout << "\nthis Game ID already exists!";
            cout << "\ncannot be re-entered!";
        }
        return root;
    }
};

// random function
int random(int seed = 232644)
{
    int a = 1664525;
    unsigned long long int c = 1013904223;
    unsigned long long int m = 4294967296;

    // Update the seed
    seed = (a * seed + c) % m;
    return seed % 1000;
}

// checks if valid input or not for numeric dtypes
bool isNumeric_check(string str)
{
    // if string is empty
    if (str.empty())
        return false;

    int i = 0;
    bool decimal_found = false;
    while (str[i] != '\0')
    {
        if (str[i] == '.')
        {
            if (decimal_found == false)
                decimal_found = true;
            else
                return false;
        }

        if (str[i] < '0' || str[i] > '9')
            return false;
        i++;
    }
    return true;
}

void ReadCSVs(PlayerTree *&playerTree, GameTree *&gameTree)
{
    int seed = random();

    // READING THE PLAYER FILE
    cout << "\n\nREADING THE PLAYER FILE: ";
    ifstream file("Players.csv");
    string line;

    if (file.is_open())
    {
        while (getline(file, line))
        {

            // making the line a stream
            stringstream stream(line);
            string playerID, playerName, phone, email, password;

            // extracting info from the stream
            getline(stream, playerID, ',');
            getline(stream, playerName, ',');
            getline(stream, phone, ',');
            getline(stream, email, ',');
            getline(stream, password, ',');

            // setting default values to prevent conversion errors if cells
            // are empty
            unsigned long long plrID = 000;
            if (isNumeric_check(playerID))
                plrID = stoull(playerID);

            seed = random(seed);
            // 44 * 10 + 100 = 540
            if (seed > 540)
            {
                // if valid row to be read then read further columns reated to
                // the games played

                // make the tree instance to be nested in player tree
                GamesPlayedTree *GamesPlayed_byPlayer = new GamesPlayedTree();

                string gamePlayedID, hrsPlayed, achievements;

                // setting default values to prevent conversion errors if cells
                // are empty
                unsigned long long id = 000;
                double hrs = 0.00;
                int ach = 0;

                while (getline(stream, gamePlayedID, ','))
                {
                    getline(stream, hrsPlayed, ',');
                    getline(stream, achievements, ',');

                    // checking if the read string is valid or not
                    if (isNumeric_check(gamePlayedID))
                        id = stoull(gamePlayedID);
                    if (isNumeric_check(hrsPlayed))
                        hrs = stod(hrsPlayed);
                    if (isNumeric_check(achievements))
                        ach = stoi(achievements);

                    // insert the gamePlayed info to the tree
                    GamesPlayed_byPlayer->insertGamesPlayed(id, hrs, ach);
                }

                // insert it to the player tree
                playerTree->insertNewPlayer(plrID, playerName, phone, email,
                                            password, GamesPlayed_byPlayer);
            }
            // else skip the line
        }
    }
    else
        cout << "file not opened" << endl;

    file.close(); // Close the file

    cout << "\nPlayers tree Data successfully made!";

    // READING THE GAME FILE
    cout << "\n\nREADING THE GAME FILE: ";
    ifstream file2("Games.csv");

    if (file2.is_open())
    {
        while (getline(file2, line))
        {

            // making the line a stream
            stringstream stream(line);
            string gameID, gameName, developer, publisher, size, downloads;

            // extracting info from the stream
            getline(stream, gameID, ',');
            getline(stream, gameName, ',');
            getline(stream, developer, ',');
            getline(stream, publisher, ',');
            getline(stream, size, ',');
            getline(stream, downloads, ',');

            // setting default values to prevent conversion errors if cells
            // are empty
            unsigned long long gmeID = 000;
            float sz = 0.00;
            int dwnlds = 0;

            if (isNumeric_check(gameID))
                gmeID = stoull(gameID);
            if (isNumeric_check(size))
                sz = stof(size);
            if (isNumeric_check(downloads))
                dwnlds = stoi(downloads);

            // insert it to the game tree
            gameTree->insertNewGame(gmeID, gameName, developer, publisher, sz,
                                    dwnlds);
        }
    }
    else
        cout << "file not opened" << endl;

    file.close(); // Close the file

    cout << "\nGames tree Data successfully made!";
}

int main()
{
    PlayerTree *playerTree = new PlayerTree();
    GameTree *gameTree = new GameTree();

    // reading the data and setting up the trees
    ReadCSVs(playerTree, gameTree);

    return 0;
}