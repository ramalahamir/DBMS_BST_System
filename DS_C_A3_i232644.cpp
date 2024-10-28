#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

struct GamesPlayedNode
{
    int gameID;
    float hoursPlayed;
    int achievements;

    GamesPlayedNode *left;
    GamesPlayedNode *right;
};

class GamesPlayedTree
{
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
    GamesPlayedTree gamesPlayed;

    PlayerNode(unsigned long long id, string nm, string phn, string em,
               string pw)
    {
        playerID = id;
        name = nm;
        phone_no = phn;
        email = em;
        password = pw;
        left = right = nullptr;
    }
};

class PlayerTree
{
    PlayerNode *root;

  public:
    PlayerTree() { root = nullptr; }
    ~PlayerTree() {}

    void insertNewPlayer(unsigned long long id, string name, string phone,
                         string email, string password)
    {
        PlayerNode *newNode = new PlayerNode(id, name, phone, email, password);
        insertNode(root, newNode);
    }

    PlayerNode *insertNode(PlayerNode *root, PlayerNode *newNode)
    {
        if (root == nullptr)
        {
            cout << "\nplayer with id " << newNode->playerID
                 << " successfully added!";
            return newNode;
        }

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
        {
            cout << "\nGame with id " << newNode->gameID
                 << " successfully added!";
            return newNode;
        }

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

int main()
{
    int seed = random();
    PlayerTree playerTree;
    GameTree gameTree;

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

            seed = random(seed);
            // 44 * 10 + 100 = 540
            if (seed > 540)
            {
                // insert it to the player tree
                playerTree.insertNewPlayer(stoull(playerID), playerName, phone,
                                           email, password);
            }
            // else skip the line
        }
    }
    else
    {
        cout << "file not opened" << endl;
        return 1;
    }

    file.close(); // Close the file

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

            // insert it to the game tree
            gameTree.insertNewGame(stoull(gameID), gameName, developer,
                                   publisher, stof(size), stoi(downloads));
        }
    }
    else
    {
        cout << "file not opened" << endl;
        return 1;
    }

    file.close(); // Close the file

    return 0;
}