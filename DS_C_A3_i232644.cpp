#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

struct Node
{
    unsigned long long ID;
    Node *next;

    Node(unsigned long long id)
    {
        ID = id;
        next = nullptr;
    }
};

class Queue
{
    Node *front;
    Node *rear;
    int size;

  public:
    Queue()
    {
        front = rear = nullptr;
        size = 0;
    }
    bool isEmpty() { return front == nullptr; }

    void enqueue(unsigned long long id)
    {
        Node *new_node = new Node(id);
        if (isEmpty())
        {
            front = rear = new_node;
            return;
        }
        rear->next = new_node;
        rear = new_node;
        size++;
    }
    unsigned long long dequeue()
    {
        if (isEmpty())
            return -1;

        Node *temp = front;
        front = front->next;

        if (front == nullptr)
            rear = nullptr;

        size--;
        return temp->ID;
    }
    int Size() { return size; }
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

    void displayGamePlayed()
    {
        cout << "\n------------------------------------";
        cout << "\nGame Played ID: " << gameID;
        cout << "\nhours played: " << hoursPlayed;
        cout << "\nachievements: " << achievements;
    }
};

class GamesPlayedTree
{
    GamesPlayedNode *root;
    int size; // to keep track of how many games the player has played!

  public:
    GamesPlayedTree()
    {
        root = nullptr;
        size = 0;
    }
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
        size++;
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

        size--;
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

    int Size() { return size; }

    // printing in preorder
    void displayAllGames(GamesPlayedNode *root)
    {
        if (root == nullptr)
            return;

        root->displayGamePlayed();
        displayAllGames(root->left);
        displayAllGames(root->right);
    }

    GamesPlayedNode *RetrieveNode(GamesPlayedNode *root, unsigned long long ID)
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

    // searches data's existence
    bool RetrieveGamePlayed(unsigned long long ID)
    {
        return (RetrieveNode(root, ID) != nullptr);
    }

    // save data
    void saveGamesPlayedByPlayerData(ofstream &file) { saveData(root, file); }
    void printAllGames() { displayAllGames(root); }
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
    // display information related to Game
    void displayPlayerInfo()
    {
        cout << "\nPlayer Info: ";
        cout << "\nPlayer ID: " << playerID;
        cout << "\nPlayer Name: " << name;
        cout << "\nplayer phone no: " << phone_no;
        cout << "\nPlayer email: " << email;
        cout << "\nPlayer password: " << password;
    }
};

class PlayerTree
{
    PlayerNode *root;

  public:
    PlayerTree() { root = nullptr; }
    ~PlayerTree() {}

    int height(PlayerNode *root)
    {
        if (root == nullptr)
            return -1;

        int left_height = height(root->left);
        int right_height = height(root->right);

        return 1 + max(left_height, right_height);
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

    // printing in breadth first order till N layer
    void print_N_layers(int N)
    {
        if (root == nullptr)
            return;

        Queue Q;
        Q.enqueue(root->playerID); // size of layer one = 1

        int layer = 1;
        while (layer <= N)
        {
            // get the size of each layer
            int layerSize = Q.Size();
            int s = 0;

            cout << "\n------------------------------------";
            cout << "\nlayer no: " << layer;
            cout << "\n------------------------------------";

            while (s <= layerSize)
            {
                unsigned long long ID = Q.dequeue();
                PlayerNode *curr = RetrievePlayer(ID);

                cout << "\n------------------------------------";
                curr->displayPlayerInfo();
                cout << "\n------------------------------------";

                if (curr->left != nullptr)
                {
                    Q.enqueue(curr->left->playerID);
                }
                if (curr->right != nullptr)
                {
                    Q.enqueue(curr->right->playerID);
                }
                s += 1;
            }
            layer++; // move to the next layer
        }
    }

    int layerNumber(unsigned long long ID)
    {
        PlayerNode *temp = RetrievePlayer(ID);
        // subtracting temp's height - root height from
        int layer = -1;
        if (temp != nullptr)
            layer = rootHeight() - height(temp);
        else
            cout << "\nID doesn't exist!";
        return layer;
    }

    void editEntry(unsigned long long ID)
    {
        cout << "\nwhat would you like to edit?";
        cout << "\n1. ID";
        cout << "\n2. Name";
        cout << "\n3. phone no";
        cout << "\n4. email";
        cout << "\n5. password";

        int input;
        cout << "\nenter: ";
        cin >> input;

        PlayerNode *oldNode = RetrievePlayer(ID);
        cin.ignore();

        if (input == 1)
        {
            unsigned long long id;
            cout << "\nenter new id: ";
            cin >> id;

            // deleting the old node
            deletePlayer(ID);
            // making new node having the same data with different ID
            insertNewPlayer(id, oldNode->name, oldNode->phone_no,
                            oldNode->email, oldNode->password,
                            oldNode->gamesPlayed);
        }
        else if (input == 2)
        {
            string name;
            cout << "\nenter new name: ";
            getline(cin, name);

            oldNode->name = name;
        }
        else if (input == 3)
        {
            string phone;
            cout << "\nenter new phone no: ";
            getline(cin, phone);

            oldNode->phone_no = phone;
        }
        else if (input == 4)
        {
            string email;
            cout << "\nenter new email: ";
            getline(cin, email);

            oldNode->email = email;
        }
        else if (input == 5)
        {
            string password;
            cout << "\nenter new password: ";
            getline(cin, password);

            oldNode->password = password;
        }
        else
        {
            cout << "\ninvalid input!";
            return;
        }
        // displaying it to the user
        cout << "\nchanges successfully made!";
        oldNode->displayPlayerInfo();
    }

    // printing the preorder path
    bool showPath(PlayerNode *root, unsigned long long ID)
    {
        if (root == nullptr)
            return false;

        cout << root->playerID << "\t";

        if (root->playerID == ID)
            return true;

        // traverse both and if either finds the target return true
        if (showPath(root->left, ID) || showPath(root->right, ID))
            return true;

        return false;
    }

    void showDetails(unsigned long long ID)
    {
        PlayerNode *temp = RetrievePlayer(ID);
        if (temp == nullptr)
        {
            cout << "\nplayer doesn't exist";
            return;
        }
        temp->displayPlayerInfo();
        temp->gamesPlayed->printAllGames();
    }

    bool hasPlayed(PlayerNode *root, unsigned long long ID)
    {
        return root->gamesPlayed->RetrieveGamePlayed(ID);
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

    // root height
    int rootHeight() { return height(root); }

    // show preorder traversal
    void showPreorderTraversal(unsigned long long ID) { showPath(root, ID); }
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

    // display information related to Game
    void displayGameInfo()
    {
        cout << "\nGame Info: ";
        cout << "\nGame ID: " << gameID;
        cout << "\nGame Name: " << name;
        cout << "\nGame developer: " << developer;
        cout << "\nGame Publisher: " << publisher;
        cout << "\nFile size (in GB): " << fileSize;
        cout << "\nNumber of downloads: " << downloads;
    }
};

class GameTree
{
    GameNode *root;

  public:
    GameTree() { root = nullptr; }
    ~GameTree() {}

    int height(GameNode *root)
    {
        if (root == nullptr)
            return -1;

        int left_height = height(root->left);
        int right_height = height(root->right);

        return 1 + max(left_height, right_height);
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
             << ',' << root->publisher << ',' << root->fileSize << ','
             << root->downloads;

        // save data in preorder traversal (root, left, right)
        saveData(root->left, file);
        saveData(root->right, file);
    }

    // printing in breadth first order till N layer
    void print_N_layers(int N)
    {
        if (root == nullptr)
            return;

        Queue Q;
        Q.enqueue(root->gameID); // size of layer one = 1

        int layer = 1;
        while (layer <= N)
        {
            // get the size of each layer
            int layerSize = Q.Size();
            int s = 0;

            cout << "\n------------------------------------";
            cout << "\nlayer no: " << layer;
            cout << "\n------------------------------------";

            while (s <= layerSize)
            {
                unsigned long long ID = Q.dequeue();
                GameNode *curr = RetrieveGame(ID);

                cout << "\n------------------------------------";
                curr->displayGameInfo();
                cout << "\n------------------------------------";

                if (curr->left != nullptr)
                {
                    Q.enqueue(curr->left->gameID);
                }
                if (curr->right != nullptr)
                {
                    Q.enqueue(curr->right->gameID);
                }
                s += 2; // because 2 childern are being inserted
            }
            layer++; // move to the next layer
        }
    }

    int layerNumber(unsigned long long ID)
    {
        GameNode *temp = RetrieveGame(ID);
        // subtracting temp's height - root height from
        int layer = -1;
        if (temp != nullptr)
            layer = rootHeight() - height(temp);
        else
            cout << "\nID doesn't exist!";
        return layer;
    }

    // printing the preorder path
    bool showPath(GameNode *root, unsigned long long ID)
    {

        if (root == nullptr)
            return false;

        cout << root->gameID << "\t";

        if (root->gameID == ID)
            return true;

        // traverse both and if either finds the target return true
        if (showPath(root->left, ID) || showPath(root->right, ID))
            return true;

        return false;
    }

    void editEntry(unsigned long long ID)
    {
        cout << "\nwhat would you like to edit?";
        cout << "\n1. ID";
        cout << "\n2. Name";
        cout << "\n3. developer";
        cout << "\n4. publisher";
        cout << "\n5. filesize";
        cout << "\n6. downloads";

        int input;
        cout << "\nenter: ";
        cin >> input;

        GameNode *oldNode = RetrieveGame(ID);
        cin.ignore();

        if (input == 1)
        {
            unsigned long long id;
            cout << "\nenter new id: ";
            cin >> id;

            // deleting the old node
            deleteGame(ID);
            // making new node having the same data with different ID
            insertNewGame(id, oldNode->name, oldNode->developer,
                          oldNode->publisher, oldNode->fileSize,
                          oldNode->downloads);
        }
        else if (input == 2)
        {
            string name;
            cout << "\nenter new name: ";
            getline(cin, name);

            oldNode->name = name;
        }
        else if (input == 3)
        {
            string dev;
            cout << "\nenter new phone no: ";
            getline(cin, dev);

            oldNode->developer = dev;
        }
        else if (input == 4)
        {
            string publisher;
            cout << "\nenter new email: ";
            getline(cin, publisher);

            oldNode->publisher = publisher;
        }
        else if (input == 5)
        {
            float size;
            cout << "\nenter new password: ";
            cin >> size;

            oldNode->fileSize = size;
        }
        else if (input == 6)
        {
            int downloads;
            cout << "\nenter new password: ";
            cin >> downloads;

            oldNode->downloads = downloads;
        }
        else
        {
            cout << "\ninvalid input!";
            return;
        }
        // displaying it to the user
        cout << "\nchanges successfully made!";
        oldNode->displayGameInfo();
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

    // root height
    int rootHeight() { return height(root); }

    // show preorder traversal
    void showPreorderTraversal(unsigned long long ID) { showPath(root, ID); }
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
                            Node->displayPlayerInfo();
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
                    case 5:
                    {
                        cout << "\n------------------------------------";
                        cout << "\nPrinting N layers: ";
                        cout << "\n------------------------------------";
                        cout << "\nenter N: ";
                        int N;
                        cin >> N;

                        // if N is negative or greater than layer
                        if (N <= 0)
                            cout << "\ninvalid input! N cannot be negative";
                        else if (N > playerTree->rootHeight())
                            cout << "\nLayer Limit was Reached, can’t go "
                                    "further";
                        else
                            playerTree->print_N_layers(N);
                        break;
                    }
                    case 6:
                    {
                        cout << "\n------------------------------------";
                        cout << "\nPrinting corresponding layer: ";
                        cout << "\n------------------------------------";
                        cout << "\nenter player ID: ";

                        unsigned long long ID;
                        cin >> ID;
                        cout << "\nlayer of " << ID << " is "
                             << playerTree->layerNumber(ID);
                        break;
                    }
                    case 7:
                    {
                        cout << "\n------------------------------------";
                        cout << "\nPrinting Preorder traversal path: ";
                        cout << "\n------------------------------------";
                        cout << "\nenter player ID: ";

                        unsigned long long ID;
                        cin >> ID;

                        // if the player exists
                        if (playerTree->RetrievePlayer(ID) != nullptr)
                        {
                            cout << "\npath of " << ID << " is :\n";
                            playerTree->showPreorderTraversal(ID);
                        }
                        else
                            cout << "\nplayer doesn't exist!";
                        break;
                    }
                    case 8:
                    {
                        cout << "\n------------------------------------";
                        cout << "\nEditing an entry: ";
                        cout << "\n------------------------------------";
                        cout << "\nenter player ID: ";

                        unsigned long long ID;
                        cin >> ID;
                        if (playerTree->RetrievePlayer(ID) != nullptr)
                            playerTree->editEntry(ID);
                        else
                            cout << "\nID doesn't exist!";
                        break;
                    }
                    case 10:
                    {
                        cout << "\n------------------------------------";
                        cout << "\nShow details: ";
                        cout << "\n------------------------------------";
                        cout << "\nenter player ID: ";

                        unsigned long long ID;
                        cin >> ID;
                        if (playerTree->RetrievePlayer(ID) != nullptr)
                            playerTree->showDetails(ID);
                        else
                            cout << "\nID doesn't exist!";
                        break;
                    }
                    case 11:
                    {
                        cout << "\n------------------------------------";
                        cout << "\nHas played: ";
                        cout << "\n------------------------------------";

                        cout << "\nenter player ID: ";
                        unsigned long long ID;
                        cin >> ID;
                        cout << "\nenter game ID: ";
                        unsigned long long gameID;
                        cin >> gameID;

                        PlayerNode *temp = playerTree->RetrievePlayer(ID);
                        if (temp != nullptr)
                            cout << (playerTree->hasPlayed(temp, gameID)
                                         ? "true"
                                         : "false");
                        else
                            cout << "\nID doesn't exist!";
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
                            Node->displayGameInfo();
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
                    case 5:
                    {
                        cout << "\n------------------------------------";
                        cout << "\nPrinting N layers: ";
                        cout << "\n------------------------------------";
                        cout << "\nenter N: ";
                        int N;
                        cin >> N;

                        // if N is negative or greater than layer
                        if (N <= 0)
                            cout << "\ninvalid input! N cannot be negative";
                        else if (N > gameTree->rootHeight())
                            cout << "\nLayer Limit was Reached, can’t go "
                                    "further";
                        else
                            gameTree->print_N_layers(N);
                        break;
                    }
                    case 6:
                    {
                        cout << "\n------------------------------------";
                        cout << "\nPrinting corresponding layer: ";
                        cout << "\n------------------------------------";
                        cout << "\nenter game ID: ";

                        unsigned long long ID;
                        cin >> ID;
                        cout << "\nlayer of " << ID << " is "
                             << gameTree->layerNumber(ID);
                        break;
                    }
                    case 7:
                    {
                        cout << "\n------------------------------------";
                        cout << "\nPrinting Preorder traversal path: ";
                        cout << "\n------------------------------------";
                        cout << "\nenter game ID: ";

                        unsigned long long ID;
                        cin >> ID;

                        // if only the game exists
                        if (gameTree->RetrieveGame(ID) != nullptr)
                        {
                            cout << "\npath of " << ID << " is :\n";
                            gameTree->showPreorderTraversal(ID);
                        }
                        else
                            cout << "\ngame doesn't exist!";
                        break;
                    }
                    case 8:
                    {
                        cout << "\n------------------------------------";
                        cout << "\nEditing an entry: ";
                        cout << "\n------------------------------------";
                        cout << "\nenter game ID: ";

                        unsigned long long ID;
                        cin >> ID;
                        if (gameTree->RetrieveGame(ID) != nullptr)
                            gameTree->editEntry(ID);
                        else
                            cout << "\nID doesn't exist!";
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