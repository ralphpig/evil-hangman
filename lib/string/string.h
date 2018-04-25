#ifndef STRING_H
  #define STRING_H
  #define STRING_DEFAULT_SIZE 7
  
  #include <stdlib.h>
  #include <stdio.h>
  #include "../generic.h"

  typedef void *String;  

  // Init
  String string_init_default(void);
  String string_init(char* str);

  // Getters
  int string_get_capacity(String hString);
  int string_get_size(String hString);
  char *string_get_data(String hString);

  // String functions
  int string_compare(String hLeft, String hRight);
  Status string_extraction(String hString, FILE* fp);
  Status string_insertion(String hString, FILE* fp);
  void string_overwrite(String hTo, String hFrom);
  void string_empty(String hString);
  Boolean string_isEmpty(String hString);
  Boolean string_contains(String hString, char item);
  Status string_push(String hString, char item);
  Status string_pop(String hString);
  Status string_concat(String hBase, String hAppend);
  void string_assign(Item *pLeft, Item Right);
  char *string_charat(String hString, int index);
  char *string_c(String hString);


  // Util functions
  int cstring_get_size(char *str);
  void cstring_cpy(char *from, char *to);
  void string_free(String *pItem);
#endif