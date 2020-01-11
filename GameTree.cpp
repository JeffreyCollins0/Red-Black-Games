#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <fstream>

using namespace std;

/*
Name: Jeffrey Collins
Course: CSCI 2275 - Programming and Data Structures
Fall 2019
Final Project
*/

// List node
struct Game{
    Game* next;
    Game* prev;
    string name;
    int year;
    string genre;
    string platform;
    int count;
    Game(string name){
        next = NULL;
        prev = NULL;
        this->name = name;
        count = 1;
    };
    ~Game(){
        if(next != NULL){
            delete next;
            next = NULL;
        }
    };
};

// Tree node
struct TreeNode{
    static TreeNode* nullNode;

    TreeNode* left;
    TreeNode* right;
    TreeNode* parent;
    bool color; // F -> red, T -> black
    vector<Game*> games;
    int year;
    TreeNode(bool isBlack = false){
        left = nullNode;
        right = nullNode;
        parent = NULL;
        vector<Game*> tem;
        games = tem;
        color = isBlack;
    };
};

TreeNode* TreeNode::nullNode = new TreeNode(true);

// Tree class
class GameTree{
    public:
        // constructor
        GameTree(){
            nullNode = TreeNode::nullNode;
            root = nullNode;
        };
        // destructor
        ~GameTree(){
            if(root != NULL){
                // postorder traversal to delete
                deleteTreeR(root);
            }
        };

        // Game tree functionality

        // add game
        void addGame(string name, int year, string genre, string platform){
            // search for the game
            TreeNode* node = search(year);
            if(node == NULL){
                // add to the tree
                redBlackInsert(name,year,genre,platform);
                // add the game node
                Game* temmie = new Game(name);
                temmie->year = year;
                temmie->genre = genre;
                temmie->platform = platform;
                // add to vector
                node = search(year);
                node->games.push_back(temmie);
            }else{
                // find game in vector
                Game* temmie = search(name,year);
                if(temmie == NULL){
                    // set up the game node
                    temmie = new Game(name);
                    temmie->year = year;
                    temmie->genre = genre;
                    temmie->platform = platform;
                    // add to vector
                    node->games.push_back(temmie);
                }else{
                    // increment count
                    temmie->count += 1;
                }
            }
            cout << "Added " << name << " to the tree" << endl;
            cout << endl;
        }

        // display a game's info
        void findGame(string name, int year){
            Game* tem = search(name, year);
            if(tem != NULL){
                cout << "==== Game info ====" << endl;
                cout << "Title: " << tem->name << endl;
                cout << "Year: " << tem->year << endl;
                cout << "Genre: " << tem->genre << endl;
                cout << "System: " << tem->platform << endl;
                cout << "Quantity: " << tem->count << endl;
            }else{
                cout << "Could not find game \'" << name << "\'" << endl;
            }
            cout << endl;
        };

        // find a game within the tree
        Game* search(string name, int year){
            TreeNode* tem = search(year);
            if(tem != NULL){
                for(int i=0; i<tem->games.size(); i++){
                    Game* temmie = tem->games.at(i);
                    if(temmie->name == name){
                        return temmie;
                    }
                }
            }
            return NULL;
        };

        // remove one copy of a game from stock
        void buyGame(string name, int year){
            // search for game
            TreeNode* node = search(year);
            if(node != NULL){
                // get game
                Game* tem = NULL;
                int ind = -1;
                for(int i=0; i<node->games.size(); i++){
                    if(node->games.at(i)->name == name){
                        tem = node->games.at(i);
                        ind = i;
                        break;
                    }
                }
                if(tem != NULL){
                    tem->count -= 1;
                    cout << "Bought " << name << ", " << tem->count << " left in stock" << endl;
                    if(tem->count <= 0){
                        // remove from vector
                        node->games.erase(node->games.begin()+ind);
                        if(node->games.size() == 0){
                            // remove from tree
                            cout << name << " is now out of stock" << endl;
                            redBlackDelete(name,year);
                        }
                    }
                }else{
                    cout << "Could not find game \'" << name << "\'" << endl;
                }
            }else{
                cout << "No games for year " << year << endl;
            }
            cout << endl;
        };

        // delete a game
        void deleteGame(string name, int year){
            // find the node
            TreeNode* node = search(year);
            if(node != NULL){
                // get game
                Game* tem = NULL;
                int ind = -1;
                for(int i=0; i<node->games.size(); i++){
                    if(node->games.at(i)->name == name){
                        tem = node->games.at(i);
                        ind = i;
                        break;
                    }
                }
                if(tem != NULL){
                    // remove from vector
                    node->games.erase(node->games.begin()+ind);
                    if(node->games.size() == 0){
                        // remove from tree
                        redBlackDelete(name,year);
                        cout << "Deleted " << name << " from the tree" << endl;
                    }
                }else{
                    cout << "Could not find game \'" << name << "\'" << endl;
                }
            }else{
                cout << "No games for year " << year << endl;
            }
            cout << endl;
        };

        // print all games
        void printGames(){
            // call recursive helper method
            if(root == nullNode){
                cout << "No games currently in tree" << endl;
                return;
            }
            cout << "==== Current Inventory ====" << endl;
            printGamesR(root);
            cout << endl;
        };

        // Red-black functionality

        // search
        TreeNode* search(int year){
            // find tree node
            TreeNode* tem = root;
            while(tem != nullNode){
                if(tem->year < year){
                    // right
                    tem = tem->right;
                }else if(tem->year > year){
                    // left
                    tem = tem->left;
                }else{
                    return tem;
                }
            }
            return NULL;
        };

        // print out the tree
        void printTree(){
            if(root == nullNode){
                cout << "Empty Tree." << endl;
                return;
            }
            queue<TreeNode*> que;
            int levelSize = 1;
            que.push(root);
            while(1){
                levelSize = que.size();
                if(levelSize == 0){
                    break;
                }
                while(levelSize > 0){
                    TreeNode* tem = que.front();
                    que.pop();
                    if(tem->left != nullNode){
                        que.push(tem->left);
                    }
                    if(tem->right != nullNode){
                        que.push(tem->right);
                    }
                    levelSize -= 1;
                    // indicate if it is a left or right child
                    string str = "-";
                    if(tem->parent != NULL){
                        if(tem == tem->parent->left){
                            str = "L";
                        }else{
                            str = "R";
                        }
                    }
                    cout << tem->year << "[" << tem->color << "][" << str << "] ";
                }
                cout << endl;
            }
        };

        // deleteTree
        void deleteTree(){
            if(root == nullNode){
                return;
            }else{
                deleteTreeR(root);
                root = nullNode;
            }
        };
    private:
        TreeNode* root;
        TreeNode* nullNode;

        // Game tree functionality

        // deleteTreeR
        void deleteTreeR(TreeNode* node){
            if(node == nullNode){
                // leaf
                return;
            }else{
                // delete left
                deleteTreeR(node->left);    
                // delete right
                deleteTreeR(node->right);
                // delete node
                delete node;
            }
        };

        // printGamesR
        void printGamesR(TreeNode* node){
            if(node == NULL || node == nullNode){
                return;
            }
            // in order traversal
            // left
            if(node->left != nullNode){
                printGamesR(node->left);
            }
            // print vector of games
            for(int i=0; i<node->games.size(); i++){
                cout << node->games.at(i)->name << " " << node->games.at(i)->count << endl;
            }
            // right
            if(node->right != nullNode){
                printGamesR(node->right);
            }
        };

        // Red-black functionality

        TreeNode* insertRB(string name, int year, string genre, string platform);
        void redBlackInsert(string name, int year, string genre, string platform);
        void redBlackDelete(string name, int year);
        void balanceRB(TreeNode* node);
        void leftRotate(TreeNode* node);
        void rightRotate(TreeNode* node);

        // other helper functions
        TreeNode* treeMinimum(TreeNode* root){
            TreeNode* tem = root;
            TreeNode* parent = NULL;
            while(tem != nullNode){
                parent = tem;
                tem = tem->left;
            }
            return parent;
        };

        TreeNode* uncle(TreeNode* node){
            if(node->parent == node->parent->parent->left){
                // left child
                return node->parent->parent->right;
            }else{
                // right child
                return node->parent->parent->left;
            }
        };

        // gets the color of a node
        bool color(TreeNode* node){
            if(node == NULL){
                return true;
            }else{
                return node->color;
            }
        };
};

void GameTree::redBlackInsert(string name, int year, string genre, string platform){
    TreeNode* node = insertRB(name,year,genre,platform);
    // adjust tree
    while (node != root && node->parent->color == false){
        // find uncle
        TreeNode* nodeUncle = uncle(node);
        if(node->parent == node->parent->parent->left){
            if(nodeUncle != NULL && nodeUncle->color == false){
                // case 1
                node->parent->color = true;
                nodeUncle->color = true;
                node->parent->parent->color = false;
                node = node->parent->parent;
            }else{
                if(node == node->parent->right){
                    // Case 2 - node is right child and uncle is black
                    node = node->parent;
                    leftRotate(node);
                }
                // Case 3 - x is left child
                node->parent->color = true;
                node->parent->parent->color = false;
                rightRotate(node->parent->parent);
            }
        }else{
            if(nodeUncle != NULL && nodeUncle->color == false){
                // case 1
                node->parent->color = true;
                nodeUncle->color = true;
                node->parent->parent->color = false;
                node = node->parent->parent;
            }else{
                if(node == node->parent->right){
                    // Case 2 - node is right child and uncle is black
                    node = node->parent;
                    leftRotate(node);
                }
                // Case 3 - x is left child
                node->parent->color = true;
                node->parent->parent->color = false;
                leftRotate(node->parent->parent);
            }
        }
    }
    root->color = true;
};

void GameTree::redBlackDelete(string name, int year){
    // find tree node
    TreeNode* node = search(year);
    if(node == NULL){
        cout << "Failed to find games from " << year << endl;
        return;
    }
    bool color = node->color;
    TreeNode* x = NULL;
    if(node != root){
        if(node->left == nullNode && node->right == nullNode){
            // no children
            if(node == node->parent->left){
                // left child
                node->parent->left = nullNode;
                x = node->parent->left;
            }else{
                // right child
                node->parent->right = nullNode;
                x = node->parent->right;
            }
        }else if(node->left != nullNode && node->right != nullNode){
            // two children
            // find successor
            TreeNode* min = treeMinimum(node->right);
            color = min->color;
            x = min->right;
            if(min == node->right){
                // replace node
                if(node == node->parent->left){
                    // left child
                    node->parent->left = min;
                }else{
                    // right child
                    node->parent->right = min;
                }
                min->parent = node->parent;
                node->left->parent = min;
                min->left = node->left;
                x->parent = min;
            }else{
                // remove successor
                min->parent->left = min->right;
                min->right->parent = min->parent;
                // replace node
                min->parent = node->parent;
                if(node == node->parent->left){
                    node->parent->left = min;
                }else{
                    node->parent->right = min;
                }
                min->left = node->left;
                min->right = node->right;
                node->left->parent = min;
                node->right->parent = min;
            }
            min->color = node->color;
        }else{
            // one child
            if(node->left != nullNode){
                // left child
                node->left->parent = node->parent;
                if(node == node->parent->left){
                    node->parent->left = node->left;
                }else{
                    node->parent->right = node->left;
                }
                x = node->left;
            }else{
                // right child
                node->right->parent = node->parent;
                if(node == node->parent->left){
                    node->parent->left = node->right;
                }else{
                    node->parent->right = node->right;
                }
                x = node->right;
            }
        }
    }else{
        // root
        if(node->left == nullNode && node->right == nullNode){
            // no children
            root = nullNode;
            x = root;
        }else if(node->left != nullNode && node->right != nullNode){
            // two children
            // find successor
            TreeNode* min = treeMinimum(node->right);
            color = min->color;
            x = min->right;
            if(min == node->right){
                // replace node
                min->parent = node->parent;
                node->left->parent = min;
                min->left = node->left;
                x->parent = min;
            }else{
                // remove successor
                min->parent->left = min->right;
                min->right->parent = min->parent;
                // replace node
                min->parent = node->parent;
                min->left = node->left;
                min->right = node->right;
                node->left->parent = min;
                node->right->parent = min;
            }
            min->color = node->color;
            root = min;
        }else{
            // one child
            if(node->left != nullNode){
                // left child
                node->left->parent = NULL;
                root = node->left;
                x = node->left;
            }else{
                // right child
                node->right->parent = NULL;
                root = node->right;
                x = node->right;
            }
        }
    }
    // balance color
    if(color == true){
        balanceRB(x);
    }
    delete node;
};

TreeNode* GameTree::insertRB(string name, int year, string genre, string platform){
    // set up the tree node
    TreeNode* node = new TreeNode();
    node->year = year;

    // insert in tree
    TreeNode* tem = root;
    TreeNode* parent = NULL;
    while(tem != nullNode){
        parent = tem;
        if(year > tem->year){
            tem = tem->right;
        }else if(year < tem->year){
            tem = tem->left;
        }else{
            break;
        }
    }
    if(parent == NULL){
        root = node;
    }else if(node->year < parent->year){
        parent->left = node;
        node->parent = parent;
    }else{
        parent->right = node;
        node->parent = parent;
    }
    return node;
}

void GameTree::balanceRB(TreeNode* node){
    while(node->parent != NULL && node->color == true){
        if(node == node->parent->left){
            // left child
            TreeNode* s = node->parent->right;
            if(color(s) == false){
                // case 1
                s->color = true;
                node->parent->color = false;
                leftRotate(node->parent);
                s = node->parent->right;
            }
            if(s != nullNode){
                if(color(s->left) == true && color(s->right) == true){
                    // case 2
                    s->color = false;
                    node = node->parent;
                }else if(color(s->left) == false && color(s->right) == true){
                    // case 3
                    s->left->color = true;
                    s->color = false;
                    rightRotate(s);
                }else{
                    // case 4
                    s->color = node->parent->color;
                    node->parent->color = true;
                    s->right->color = true;
                    leftRotate(node->parent);
                    node = root;
                }
            }else{
                // sibling is null
                node = node->parent;
            }
        }else{
            // right child
            TreeNode* s = node->parent->left;
            if(color(s) == false){
                // case 1
                s->color = true;
                node->parent->color = false;
                rightRotate(node->parent);
                s = node->parent->left;
            }
            if(s != nullNode){
                if(color(s->left) == true && color(s->right) == true){
                    // case 2
                    s->color = false;
                    node = node->parent;
                }else if(color(s->left) == false && color(s->right) == true){
                    // case 3
                    s->left->color = true;
                    s->color = false;
                    leftRotate(s);
                }else{
                    // case 4
                    s->color = node->parent->color;
                    node->parent->color = true;
                    if(s->right != NULL){
                        s->right->color = true;
                    }
                    rightRotate(node->parent);
                    node = root;
                }
            }else{
                // sibling is null
                node = node->parent;
            }
        }
    }
    node->color = true;
}

void GameTree::leftRotate(TreeNode* x){
    TreeNode* y = x->right;
    x->right = y->left;
    if(y->left != nullNode){
        y->left->parent = x;
    }
    y->parent = x->parent;
    if(y->parent == NULL){
        root = y;
    }else{
        if(x == x->parent->left){
            x->parent->left = y;
        }else{
            x->parent->right = y;
        }
    }
    y->left = x;
    x->parent = y;
}

void GameTree::rightRotate(TreeNode* y){
    TreeNode* x = y->left;
    y->left = x->right;
    if(x->right != nullNode){
        x->right->parent = y;
    }
    x->parent = y->parent;
    if(y->parent == NULL){
        root = x;
    }else{
        if(y == y->parent->left){
            y->parent->left = x;
        }else{
            y->parent->right = x;
        }
    }
    x->right = y;
    y->parent = x;
}

// Main function
int main(int argc, char* argv[]){
    // create the gameTree
    GameTree* gt = new GameTree();

    // read in the file
    ifstream input;
    if(argc < 2){
        input.open("debug.txt");
    }else{
        input.open(argv[1]);
    }
    if(!input.is_open()){
        cout << "Error opening file \'" << argv[1] << "\'" << endl;
        return 1;
    }
    while(input.good()){
        string title;
        string year;
        string genre;
        string system;
        getline(input,title,',');
        if (input.eof()) {
            break;
        }
        getline(input,year,',');
        getline(input,genre,',');
        getline(input,system);
        try {
            gt->addGame(title,stoi(year),genre,system);
        } catch (invalid_argument e) {
            cerr << "invalid year " << year << endl;
        }
    }
    input.close();

    string in = "";
    while(in != "7"){
        // display a menu
        cout << "====== MAIN MENU ======" << endl;
        cout << "1. Add a game" << endl;
        cout << "2. Buy a game" << endl;
        cout << "3. Find a game" << endl;
        cout << "4. Delete a game" << endl;
        cout << "5. Delete all games" << endl;
        cout << "6. Print inventory" << endl;
        cout << "7. Exit" << endl;

        // take user input
        cin >> in;
        if(in == "1"){
            // add game
            cout << "Enter the game title:" << endl;
            string title = "";
            cin.clear();
            cin.ignore();
            getline(cin,title);
            cout << "Enter the release year:" << endl;
            string year = "";
            cin >> year;
            cout << "Enter the genre:" << endl;
            string genre = "";
            cin >> genre;
            cout << "Enter the game system:" << endl;
            string system = "";
            cin.clear();
            cin.ignore();
            getline(cin,system);
            // perform the operation
            gt->addGame(title,stoi(year),genre,system);
        }else if(in == "2"){
            // buy game
            cout << "Enter the game title:" << endl;
            string title = "";
            cin.clear();
            cin.ignore();
            getline(cin,title);
            cout << "Enter the release year:" << endl;
            string year = "";
            cin >> year;
            // perform the operation
            gt->buyGame(title,stoi(year));
        }else if(in == "3"){
            // find game
            cout << "Enter the game title:" << endl;
            string title = "";
            cin.clear();
            cin.ignore();
            getline(cin,title);
            cout << "Enter the release year:" << endl;
            string year = "";
            cin >> year;
            // perform the operation
            gt->findGame(title,stoi(year));
        }else if(in == "4"){
            // delete game
            cout << "Enter the game title:" << endl;
            string title = "";
            cin.clear();
            cin.ignore();
            getline(cin,title);
            cout << "Enter the release year:" << endl;
            string year = "";
            cin >> year;
            // perform the operation
            gt->deleteGame(title,stoi(year));
        }else if(in == "5"){
            // delete all
            gt->deleteTree();
        }else if(in == "6"){
            // print all
            gt->printGames();
        }
    }
    cout << "Goodbye." << endl;
    return 0;
};
