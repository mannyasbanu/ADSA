#include <iostream>
#include <vector>
#include <stack>
#include <sstream>
using namespace std;



// AVL TREE IMPLEMENTATION //

// === NODE STRUCTURE ===
struct Node {
  int key;
  Node* left;
  Node* right;
  int height;
  Node (int k) : key(k), height(1) {}
  int getBalance () {
    int leftHeight = left ? left->height : 0;
    int rightHeight = right ? right->height : 0;
    return leftHeight - rightHeight;
  }
};



// HELPER FUNCTIONS //

// === GET HEIGHT OF NODE ===
int getHeight(Node* root) {
  return root ? root->height : 0;
}
// === GET MINIMUM NODE IN SUBTREE ===
Node* getMinNode(Node* root) {
  Node* cur = root;
  while (cur->left != nullptr) {
    // find leftmost node
    cur = cur->left;
  }
  return cur;
}
// === ROTATE TREE RIGHT ===
Node* rotateRight(Node* root) {
  Node* newRoot = root->left;
  root->left = newRoot->right;
  newRoot->right = root;
  // update heights
  root->height = 1 + max(getHeight(root->left), getHeight(root->right));
  newRoot->height = 1 + max(getHeight(newRoot->left), getHeight(newRoot->right));
  return newRoot; 
}
// === ROTATE TREE LEFT ===
Node* rotateLeft(Node* root) {
  Node* newRoot = root->right;
  root->right = newRoot->left;
  newRoot->left = root;
  // update heights
  root->height = 1 + max(getHeight(root->left), getHeight(root->right));
  newRoot->height = 1 + max(getHeight(newRoot->left), getHeight(newRoot->right));
  return newRoot; 
}



// TRAVERSAL FUNCTIONS //

// === PRE-ORDER TRAVERSAL ===
vector<int> preOrder(Node* root) {
  vector<int> res;
  stack<Node*> nodeStack;
  if (root != nullptr) nodeStack.push(root);
  while (!nodeStack.empty()) {
    Node* cur = nodeStack.top();
    nodeStack.pop();
    // visit current
    res.push_back(cur->key);
    // push right then left
    if (cur->right != nullptr) nodeStack.push(cur->right);
    if (cur->left != nullptr) nodeStack.push(cur->left);
  }
  return res;
}
// === IN-ORDER TRAVERSAL ===
vector<int> inOrder(Node* root) {
  vector<int> res;
  stack<Node*> nodeStack;
  Node* cur = root;
  // traverse while nodes exist and stack not empty
  while (cur != nullptr || !nodeStack.empty()) {
    // reach leftmost node
    while (cur != nullptr) {
      nodeStack.push(cur);
      cur = cur->left;
    }
    // visit parent of null
    cur = nodeStack.top();
    nodeStack.pop();
    res.push_back(cur->key);
    // search right subtree
    cur = cur->right;
  }
  return res;
}
// === POST-ORDER TRAVERSAL ===
vector<int> postOrder(Node* root) {
  vector<int> res;
  stack<Node*> nodeStack;
  Node* cur = root;
  Node* lastVisit = nullptr;
  while (cur != nullptr || !nodeStack.empty()) {
    // reach leftmost subtree
    if (cur != nullptr) {
      nodeStack.push(cur);
      cur = cur->left;
    }
    else {
      Node* topNode = nodeStack.top();
      // search right subtree
      if (topNode->right != nullptr && topNode->right != lastVisit) {
        cur = topNode->right;
      // add leaf node to res
      } else {
        res.push_back(topNode->key);
        lastVisit = topNode;
        nodeStack.pop();
      }
    }
  }
  return res;
}



// INSERTION AND DELETION FUNCTIONS //

// === AVL TREE INSERTION ===
Node* insert(Node* root, int key) {
  // insert pos reached
  if (root == nullptr) return new Node(key);
  // search left
  else if (key < root->key) root->left = insert(root->left, key);
  // search right
  else root->right = insert(root->right, key);
  // update height
  root->height = 1 + max(getHeight(root->left), getHeight(root->right));
  // get balance factor
  int balance = root->getBalance();
  // rebalance tree
  if (balance > 1 && key < root->left->key) {
    return rotateRight(root);
  }
  if (balance < -1 && key > root->right->key) {
    return rotateLeft(root);
  }
  if (balance > 1 && key > root->left->key) {
    root->left = rotateLeft(root->left);
    return rotateRight(root);
  }
  if (balance < -1 && key < root->right->key) {
    root->right = rotateRight(root->right);
    return rotateLeft(root);
  }
  return root;
}
// === AVL TREE DELETION ===
Node* deleteNode(Node* root, int key) {
  // key not found
  if (root == nullptr) return root;
  // search left
  else if (key < root->key) root->left = deleteNode(root->left, key);
  // search right
  else if (key > root->key) root->right = deleteNode(root->right, key);
  // key found
  else {
    // replace right child
    if (root->left == nullptr) {
      Node* temp = root->right;
      delete(root);
      return temp;
    }
    // replace left child
    else if (root->right == nullptr) {
      Node* temp = root->left;
      delete(root);
      return temp;
    }
    // replace in order successor
    Node* temp = getMinNode(root->right);
    root->key = temp->key;
    root->right = deleteNode(root->right, temp->key);
  }
  // update height
  root->height = 1 + max(getHeight(root->left), getHeight(root->right));
  // get balance factor
  int balance = root->getBalance();
  // rebalance tree
  if (balance > 1 && root->left->getBalance() >= 0) {
    return rotateRight(root);
  }
  if (balance < -1 && root->right->getBalance() <= 0) {
    return rotateLeft(root);
  }
  if (balance > 1 && root->left->getBalance() < 0) {
    root->left = rotateLeft(root->left);
    return rotateRight(root);
  }
  if (balance < -1 && root->right->getBalance() > 0) {
    root->right = rotateRight(root->right);
    return rotateLeft(root);
  }
  return root;
}



// TREE STRUCTURE //

// === TREE CLASS ===
class Tree {
private:
  Node* root;

public:
  Tree(Node* r = nullptr);
  Node* getRoot() { return root; }
  void insertValue(int key);
  void deleteValue(int key);
};
// === TREE CONSTRUCTOR ===
Tree::Tree(Node* r) : root(r) {}
// === TREE INSERTION AND DELETION ===
void Tree::insertValue(int key) {
  root = insert(root, key);
}
void Tree::deleteValue(int key) {
  root = deleteNode(root, key);
}



// INPUT OUTPUT FUNCTIONS //

// === PRINT VECTOR ===
void printVector(const vector<int>& vec) {
  for (size_t i = 0; i < vec.size(); i++) {
    cout << vec[i];
    if (i != vec.size() - 1) cout << " ";
  }
  cout << endl;
}
// === PROCESS MOVE ===
void processMove(Tree& tree, const string& move) {
  // insert value
  if (move[0] == 'A') {
    tree.insertValue(stoi(move.substr(1)));
  }
  // delete value
  else if (move[0] == 'D') {
    tree.deleteValue(stoi(move.substr(1)));
  }
  // print traversal
  else {
    vector<int> traversal;
    if (move == "PRE") {
      traversal = preOrder(tree.getRoot());
    }
    else if (move == "IN") {
      traversal = inOrder(tree.getRoot());
    }
    else if (move == "POST") {
      traversal = postOrder(tree.getRoot());
    }
    if (traversal.empty()) {
      cout << "EMPTY" << endl;
    }
    else {
      printVector(traversal);
    }
  }
}



// MAIN FUNCTION //

int main() {
  Tree tree;
  string move; 
  while (cin >> move) {
    processMove(tree, move);
  }
  return 0;
}