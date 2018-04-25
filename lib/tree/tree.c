#include "tree.h"

struct node;
typedef struct node Node;

struct node {
  int key;
  int height;
  String family;
  Vector words;
  Node *left;
  Node *right;  
};


Node *tree_balance(Node *node);
Node *tree_rightRotate(Node *node);
Node *tree_leftRotate(Node *node);
Node *tree_newNode(int key, String family, String word);

int tree_height(Node *node);

int max(int a, int b);



FTNode tree_insert(FTNode hNode, int key, String family, String word) {
  Node *node = (Node *) hNode;
  if(!node) {
    return tree_newNode(key, family, word);
  }

  if(key < node->key) {
    node->left = tree_insert(node->left, key, family, word);
  } else if(key > node->key){
    node->right = tree_insert(node->right, key, family, word);
  } else {
    vector_push(node->words, word);
    string_free(&family);
  }

  node->height = 1 + max(tree_height(node->left), tree_height(node->right));

  return tree_balance(node);
}

Vector tree_getWords(FTNode hNode) {
  Node *node = (Node *) hNode;
  return node->words;
}

String tree_getFamily(FTNode hNode) {
  Node *node = (Node *) hNode;
  return node->family;
}

FTNode tree_getMaxWords(FTNode hNode) {
  if(!hNode) return NULL;

  Node *max = (Node *) hNode;
  Node *left = tree_getMaxWords(max->left);
  Node *right = tree_getMaxWords(max->right);    
  int l = left ? vector_getSize(left->words) : -1;
  int r = right ? vector_getSize(right->words) : -1;
  
  if(vector_getSize(max->words) < r) max = right;
  if(vector_getSize(max->words) < l) max = left;

  return max;
}

void tree_inorder(FTNode hNode, void (*forEach)(int, String, Vector)) {  
  Node *root = (Node *) hNode;
  if(root) {
    forEach(root->key, root->family, root->words);
    tree_inorder(root->left, forEach);    
    tree_inorder(root->right, forEach);
  }
}


void tree_free(FTNode *hNode) {
  tree_free_(*hNode);
  *hNode = NULL;
}

void tree_free_(FTNode hNode) {  
  if(!hNode) return;
  Node *root = (Node *) hNode;
  // printf("## %d: %u\n", root->key, root);
  // printf("   Family: %s\n", string_c(root->family));  
  // printf("   Left: %u\n", root->left);
  // printf("   Right: %u\n", root->right);
  tree_free((FTNode *) &root->left);
  tree_free((FTNode *) &root->right);
  string_free(&root->family);
  vector_free(&root->words);
  free(root);
}





Node *tree_balance(Node *node) {
  int balance = tree_height(node->left) - tree_height(node->right);

  // Left
  if(balance > 1) {
    int subBalance = tree_height(node->left->left) - tree_height(node->left->right);
    // Left - Left
    if(subBalance > 0) {
      return tree_rightRotate(node);
    }

    // Left - Right
    if(subBalance < 0) {
      node->left = tree_leftRotate(node->left);
      return tree_rightRotate(node);
    }
  }
  // Right
  if(balance > 1) {
    int subBalance = tree_height(node->right->left) - tree_height(node->right->right);
    // Left - Right
    if(subBalance > 0) {
      node->right = tree_rightRotate(node->right);
      return tree_leftRotate(node);
    }

    // Right - Right
    if(subBalance < 0) { 
      return tree_leftRotate(node);
    }
  }

  return node;
}

Node *tree_rightRotate(Node *y) {  
  Node *x = y->left;
  Node *T2 = x->right;

  x->right = y;
  y->left = T2;

  x->height = max(tree_height(x->left), tree_height(x->right));
  y->height = max(tree_height(y->left), tree_height(y->right));

  return x;
}

Node *tree_leftRotate(Node *x) {
  Node *y = x->right;
  Node *T2 = y->left;

  y->left = x;
  x->right = T2;

  x->height = max(tree_height(x->left), tree_height(x->right));
  y->height = max(tree_height(y->left), tree_height(y->right));

  return y;
}

Node *tree_newNode(int key, String family, String word) {
  Node *node = calloc(1, sizeof(Node));
  node->family = family;
  node->key = key;
  node->words = vector_init();
  vector_push(node->words, word);

  return node;
}

int tree_height(Node *node) {
  if(!node) return 0;
  return node->height;
}

int max(int a, int b) {
  return (a > b) ? a : b;
}