#ifndef FamilyTree_H
  #define FamilyTree_H
  #include "../string/string.h"
  #include "../vector/vector.h"
  
  typedef void *FTNode;

  // Getters
  Vector tree_getWords(FTNode hNode);
  String tree_getFamily(FTNode hNode);
  FTNode tree_getMaxWords(FTNode hNode);

  // Avl functions
  FTNode tree_insert(FTNode hNode, int key, String family, String word);  
  void tree_inorder(FTNode hNode, void (*forEach)(int, String, Vector));  

  // Util functions 
  void tree_free(FTNode *hNode);
  void tree_free_(FTNode hNode);
#endif