#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

class Node
{
  public:
    int data;
    Node *next;

    Node(int d)
    {
        data = d;
        next = nullptr;
    }
};

class Queue
{
  private:
    Node *front;
    Node *rear;

  public:
    Queue() { front = rear = nullptr; }
    void enqueue(int val)
    {
        Node *newNode = new Node(val);
        if (rear == nullptr)
        {
            front = rear = newNode;
        }
        else
        {
            rear->next = newNode;
            rear = newNode;
        }
    }
    void dequeue()
    {
        if (front == nullptr)
            return;

        Node *temp = front;
        front = front->next;

        // if queue becomes empty while dequeueing
        if (front == nullptr)
            rear = nullptr;

        delete temp;
    }
    bool isEmpty() { return front == nullptr; }
};

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

    GamesPlayedNode *FindMax(GamesPlayedNode *root)
    {
        while (root->right != nullptr)
            root = root->right;
        return root;
    }

    GamesPlayedNode *deleteNode(GamesPlayedNode *root,
                                unsigned long long gameID)
    {
        if (root == nullptr)
            return nullptr;

        if (gameID < root->gameID)
            root->left = deleteNode(root->left, gameID);
        else if (gameID > root->gameID)
            root->right = deleteNode(root->right, gameID);

        else
        {
            if (root->left == nullptr)
            {
                GamesPlayedNode *temp = root->right;
                delete root;
                return temp;
            }
            if (root->right == nullptr)
            {
                GamesPlayedNode *temp = root->left;
                delete root;
                return temp;
            }

            // predecessor case
            GamesPlayedNode *temp = FindMax(root->left);
            root->gameID = temp->gameID;
            root->left = deleteNode(root->left, temp->gameID);
        }
        return root;
    }

    void saveData(GamesPlayedNode *root, ofstream &file)
    {
        if (root == nullptr)
            return;

        // writing the data
        file << root->gameID << ',' << root->hoursPlayed << ','
             << root->achievements << ',';

        // traversing the nodes in predorder (root, left, right)
        saveData(root->left, file);
        saveData(root->right, file);
    }

    void print_N_layers(int N) {}

    // functions for calling from main()
    ////////////////////////////////////

    // insert data
    void insertGamesPlayed(unsigned long long id, double hrsPlayed,
                           int achievements)
    {
        GamesPlayedNode *newNode =
            new GamesPlayedNode(id, hrsPlayed, achievements);
        root = insertNode(root, newNode);
    }

    // delete data
    void deleteGamesPlayed(unsigned long long gameID)
    {
        root = deleteNode(root, gameID);
    }

    // save data
    void saveGamesPlayedByPlayerData(ofstream &file) { saveData(root, file); }
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

    PlayerNode *FindMax(PlayerNode *root)
    {
        while (root->right != nullptr)
            root = root->right;
        return root;
    }

    PlayerNode *deleteNode(PlayerNode *root, unsigned long long playerID)
    {
        if (root == nullptr)
            return nullptr;

        if (playerID < root->playerID)
            root->left = deleteNode(root->left, playerID);
        else if (playerID > root->playerID)
            root->right = deleteNode(root->right, playerID);

        else
        {
            if (root->left == nullptr)
            {
                PlayerNode *temp = root->right;
                delete root;
                return temp;
            }
            if (root->right == nullptr)
            {
                PlayerNode *temp = root->left;
                delete root;
                return temp;
            }

            // predecessor case
            PlayerNode *temp = FindMax(root->left);
            root->playerID = temp->playerID;
            root->left = deleteNode(root->left, temp->playerID);
        }
        return root;
    }

    PlayerNode *RetrieveNode(PlayerNode *root, unsigned long long ID)
    {
        if (root == nullptr)
            return nullptr;
        if (root->playerID == ID)
            return root;
        else if (ID < root->playerID)
            return RetrieveNode(root->left, ID);
        else
            return RetrieveNode(root->right, ID);
        return nullptr;
    }

    // saves data to a new csv file
    void saveData(PlayerNode *root, ofstream &file)
    {
        if (root == nullptr)
            return;

        // else write the player data
        file << endl
             << root->playerID << ',' << root->name << ',' << root->phone_no
             << ',' << root->email << ',' << root->password << ',';

        // write the games played by the player info
        root->gamesPlayed->saveGamesPlayedByPlayerData(file);

        // save data in preorder traversal (root, left, right)
        saveData(root->left, file);
        saveData(root->right, file);
    }

    // functions for calling from main()
    ////////////////////////////////////

    // inserts data
    void insertNewPlayer(unsigned long long id, string name, string phone,
                         string email, string password,
                         GamesPlayedTree *&GamesPlayed_byPlayer)
    {
        PlayerNode *newNode = new PlayerNode(id, name, phone, email, password,
                                             GamesPlayed_byPlayer);
        root = insertNode(root, newNode);
    }

    // deletes data
    void deletePlayer(unsigned long long playerID)
    {
        root = deleteNode(root, playerID);
    }

    // saves data
    void savePlayerData(ofstream &file) { saveData(root, file); }

    // searches data
    PlayerNode *RetrievePlayer(unsigned long long playerID)
    {
        return RetrieveNode(root, playerID);
    }

    // display information related to Game
    void displayPlayerInfo(PlayerNode *root)
    {
        cout << "\nPlayer Info: ";
        cout << "\nPlayer ID: " << root->playerID;
        cout << "\nPlayer Name: " << root->name;
        cout << "\nplayer phone no: " << root->phone_no;
        cout << "\nPlayer email: " << root->email;
        cout << "\nPlayer password: " << root->password;
        // cout << "\nGames played: " << root->gamesPlayed;
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

    GameNode *FindMax(GameNode *root)
    {
        while (root->right != nullptr)
            root = root->right;
        return root;
    }

    GameNode *deleteNode(GameNode *root, unsigned long long gameID)
    {
        if (root == nullptr)
            return nullptr;

        if (gameID < root->gameID)
            root->left = deleteNode(root->left, gameID);
        else if (gameID > root->gameID)
            root->right = deleteNode(root->right, gameID);

        else
        {
            if (root->left == nullptr)
            {
                GameNode *temp = root->right;
                delete root;
                return temp;
            }
            if (root->right == nullptr)
            {
                GameNode *temp = root->left;
                delete root;
                return temp;
            }

            // predecessor case
            GameNode *temp = FindMax(root->left);
            root->gameID = temp->gameID;
            root->left = deleteNode(root->left, temp->gameID);
        }
        return root;
    }

    GameNode *RetrieveNode(GameNode *root, unsigned long long ID)
    {
        if (root == nullptr)
            return nullptr;
        if (root->gameID == ID)
            return root;
        else if (ID < root->gameID)
            return RetrieveNode(root->left, ID);
        else
            return RetrieveNode(root->right, ID);
        return nullptr;
    }

    // saves data to a new csv file
    void saveData(GameNode *root, ofstream &file)
    {
        if (root == nullptr)
            return;

        // else write the player data
        file << endl
             << root->gameID << ',' << root->name << ',' << root->developer
             << ',' << root->publisher << ',' << root->fileSize,
            ',' << root->downloads;

        // save data in preorder traversal (root, left, right)
        saveData(root->left, file);
        saveData(root->right, file);
    }

    // functions for calling from main()
    ////////////////////////////////////

    // inserts data
    void insertNewGame(unsigned long long id, string name, string developer,
                       string publisher, float size, int downloads)
    {
        GameNode *newNode =
            new GameNode(id, name, developer, publisher, size, downloads);
        root = insertNode(root, newNode);
    }

    // deletes data
    void deleteGame(unsigned long long gameID)
    {
        root = deleteNode(root, gameID);
    }

    // searches data
    GameNode *RetrieveGame(unsigned long long gameID)
    {
        return RetrieveNode(root, gameID);
    }

    void saveGameData(ofstream &file) { saveData(root, file); }

    // display information related to Game
    void displayGameInfo(GameNode *root)
    {
        cout << "\nGame Info: ";
        cout << "\nGame ID: " << root->gameID;
        cout << "\nGame Name: " << root->name;
        cout << "\nGame developer: " << root->developer;
        cout << "\nGame Publisher: " << root->publisher;
        cout << "\nFile size (in GB): " << root->fileSize;
        cout << "\nNumber of downloads: " << root->downloads;
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

// all the reading work
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
                // if valid row to be read then read further columns reated
                // to the games played

                // make the tree instance to be nested in player tree
                GamesPlayedTree *GamesPlayed_byPlayer = new GamesPlayedTree();
                string gamePlayedID, hrsPlayed, achievements;

                // setting default values to prevent conversion errors if
                // cells are empty
                unsigned long long id = 000;
                double hrs = 0.00;
                int ach = 0;

                while (getline(stream, gamePlayedID, ','))
                {
                    // if the read cell is empty break the loop
                    if (gamePlayedID.empty())
                        break;

                    getline(stream, hrsPlayed, ',');
                    getline(stream, achievements, ',');

                    // checking if the read string is valid or not
                    if (isNumeric_check(gamePlayedID))
                        id = stoull(gamePlayedID);

                    hrs = stod(hrsPlayed);
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
    cout << "\ngames played tree for each player node also successfully "
            "made!";

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
            // if (isNumeric_check(size))
            sz = stof(size);
            // if (isNumeric_check(downloads))
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
    int input;

    do
    {
        cout << "\n------------------------------------";
        cout << "\nQuery Menu: ";
        cout << "\nwhich tree do you wish to explore?";
        cout << "\n1. Player Tree"
             << "\n2. Game Tree";
        cout << "\n------------------------------------";

        cout << "\nenter: (enter '0' to quit) ";
        cin >> input;

        switch (input)
        {
            // for quit
            case 0:
                break;
            case 1:
            {
                cout << "\n------------------------------------";
                cout << "\nchoose the following operations: ";
                cout << "\n1. insertion"
                     << "\n2. search / retrieval"
                     << "\n3. deletion"
                     << "\n4. save data"
                     << "\n5. show N layers"
                     << "\n6. show layer number"
                     << "\n7. show path"
                     << "\n8. edit entry"
                     << "\n9. top N players"
                     << "\n10. show details"
                     << "\n11. has played";
                cout << "\n------------------------------------";

                int choice;
                cout << "\nenter: ";
                cin >> choice;

                switch (choice)
                {
                    case 0:
                        break;
                    case 1:
                    {
                        cout << "\n------------------------------------";
                        cout << "\n Player Insertion: ";
                        cout << "\n------------------------------------";
                        cout << "\nenter player info: ";

                        unsigned long long playerID;
                        string name;
                        string phone;
                        string email;
                        string password;

                        cout << "\nenter player ID: ";
                        cin >> playerID;
                        cout << "Enter player name: ";
                        cin.ignore();
                        getline(cin, name);
                        cout << "Enter player phone number: ";
                        getline(cin, phone);
                        cout << "Enter player email: ";
                        getline(cin, email);
                        cout << "Enter player password: ";
                        getline(cin, password);

                        // record of played games of the player
                        GamesPlayedTree *GamesPlayed_byPlayer =
                            new GamesPlayedTree();
                        int n;
                        cout << "\nhow many games has the user played?";
                        cout << "\nenter number: ";
                        cin >> n;
                        cout << "\nenter game record one by one: ";

                        for (int i = 0; i < n; i++)
                        {
                            // get the info for single game played node
                            unsigned long long gameID;
                            cout << "\nenter game Id: ";
                            cin >> gameID;
                            double hrsPlayed;
                            cout << "enter hours played: ";
                            cin >> hrsPlayed;
                            int achievements;
                            cout << "enter no of achievements unlocked: ";
                            cin >> achievements;

                            // insert it to the tree
                            GamesPlayed_byPlayer->insertGamesPlayed(
                                gameID, hrsPlayed, achievements);
                        }

                        // insert the new player into the dataset
                        playerTree->insertNewPlayer(playerID, name, phone,
                                                    email, password,
                                                    GamesPlayed_byPlayer);
                        cout << "insertion of player node successfull!";
                        break;
                    }
                    case 2:
                    {
                        cout << "\n------------------------------------";
                        cout << "\n Player Retrieval: ";
                        cout << "\n------------------------------------";
                        unsigned long long playerID;
                        cout << "\nenter playerID: ";
                        cin >> playerID;

                        // show the node to the user
                        PlayerNode *Node = playerTree->RetrievePlayer(playerID);
                        if (Node == nullptr)
                            cout << "no such player exists!";
                        else
                            playerTree->displayPlayerInfo(Node);
                        break;
                    }
                    case 3:
                    {
                        cout << "\n------------------------------------";
                        cout << "\n Player Deletion: ";
                        cout << "\n------------------------------------";
                        unsigned long long playerID;
                        cout << "\nenter player ID: ";
                        cin >> playerID;
                        // delete the player
                        playerTree->deletePlayer(playerID);
                        cout << "deletion of player node successfull!";
                        break;
                    }
                    case 4:
                    {
                        ofstream file("savePlayerData.csv");
                        playerTree->savePlayerData(file);
                        cout << "\n------------------------------------";
                        cout << "\n Saving Player data to "
                                "'savePlayerData.csv' .... completed!";
                        cout << "\n------------------------------------";
                        break;
                    }
                    default:
                        cout << "\ninvalid option entered!";
                        break;
                }
                break;
            }
            case 2:
            {
                cout << "\n------------------------------------";
                cout << "\nchoose the following operations: ";
                cout << "\n1. insertion"
                     << "\n2. search / retrieval"
                     << "\n3. deletion"
                     << "\n4. save data"
                     << "\n5. show N layers"
                     << "\n6. show layer number"
                     << "\n7. show path"
                     << "\n8. edit entry";
                cout << "\n------------------------------------";

                int choice;
                cout << "\nenter: ";
                cin >> choice;

                switch (choice)
                {
                    case 1:
                    {
                        cout << "\n------------------------------------";
                        cout << "\n Game Insertion: ";
                        cout << "\n------------------------------------";
                        cout << "\nenter game info: ";

                        unsigned long long gameID;
                        string name;
                        string developer;
                        string publisher;
                        float size;
                        int downloads;

                        cout << "\nEnter game ID: ";
                        cin >> gameID;
                        cin.ignore();
                        cout << "Enter game name: ";
                        getline(cin, name);
                        cout << "Enter game developer: ";
                        getline(cin, developer);
                        cout << "Enter game publisher: ";
                        getline(cin, publisher);
                        cout << "Enter game size (in GB): ";
                        cin >> size;
                        cout << "Enter number of downloads: ";
                        cin >> downloads;

                        // insert the new game into the tree
                        gameTree->insertNewGame(gameID, name, developer,
                                                publisher, size, downloads);
                        cout << "insertion of game node successfull!";
                        break;
                    }
                    case 2:
                    {
                        cout << "\n------------------------------------";
                        cout << "\n Game Retrieval: ";
                        cout << "\n------------------------------------";
                        unsigned long long gameID;
                        cout << "\nenter gameID: ";
                        cin >> gameID;
                        // show the node to the user
                        GameNode *Node = gameTree->RetrieveGame(gameID);
                        if (Node == nullptr)
                            cout << "Node doesn't exist!";
                        else
                            gameTree->displayGameInfo(Node);
                        break;
                    }
                    case 3:
                    {
                        cout << "\n------------------------------------";
                        cout << "\n Game Deletion: ";
                        cout << "\n------------------------------------";
                        unsigned long long gameID;
                        cout << "\nenter game ID: ";
                        cin >> gameID;
                        // delete the game
                        gameTree->deleteGame(gameID);
                        cout << "deletion of game node successfull!";
                        break;
                    }
                    case 4:
                    {
                        ofstream file("saveGameData.csv");
                        gameTree->saveGameData(file);
                        cout << "\n------------------------------------";
                        cout << "\n Saving Game data to "
                                "'saveGameData.csv' .... completed!";
                        cout << "\n------------------------------------";
                        break;
                    }
                    default:
                        cout << "\ninvalid option entered!";
                        break;
                }
                break;
            }
            default:
                cout << "\ninvalid input!";
                return 0;
        }
    } while (input != 0);
    return 0;
}