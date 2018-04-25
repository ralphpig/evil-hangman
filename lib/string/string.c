#include "string.h"

typedef struct {
  char* data;
  int size;
  int capacity;
} string;

/**    Init    **/
String string_init_default(void) {
  int capacity = STRING_DEFAULT_SIZE + 1;
  string *str = (string *) malloc(sizeof(string));

  str->data = (char *) calloc(capacity, sizeof(char));
  str->size = 0;
  str->capacity = capacity;

  if(str->data == NULL) return NULL;

  return (String) str;
}

String string_init(char* init) {
  int size = cstring_get_size(init);
  int capacity = ((STRING_DEFAULT_SIZE > size) ? STRING_DEFAULT_SIZE : size) + 1;
  
  string *str = (string *) malloc(sizeof(string));

  str->data = (char *) calloc(capacity, sizeof(char));
  str->size = size;
  str->capacity = capacity;

  if(str->data == NULL) return NULL;
  
  cstring_cpy(init, str->data);

  return (String) str;
}


/**    Getters    **/
int string_get_capacity(String hString) {
  string *str = (string *) hString;
  return str->capacity;
}

int string_get_size(String hString) {
  string *str = (string *) hString;
  return str->size;
}

char *string_get_data(String hString) {
  string *str = (string *) hString;
  return str->data;
}


/**    String functions    **/
int string_compare(String hLeft, String hRight) {
  char *sLeft = ((string *) hLeft)->data;
  char *sRight = ((string *) hRight)->data;  

  for(; *sLeft; sLeft++, sRight++) {
    if(*sLeft < *sRight) return -1;
    if(*sLeft > *sRight) return 1;
  }

  return 0;
}

Status string_extraction(String hString, FILE* fp) {
  string *str = (string *) hString;
  char *data = str->data;
  int tmp, tmpSize = 0;
  
  // Clear whitespace
  while((tmp = fgetc(fp)) == ' ') {
    printf("-WS-\n");
  }

  while(tmp != ' ' && tmp != '\n' && tmp != EOF) {    
    if(++tmpSize == str->capacity) {
      str->capacity *= 2;      
      data = str->data = realloc(str->data, str->capacity * sizeof(char));
      if(str->data == NULL) return FAILURE;
    }

    *data = (char) tmp;
    *(++data) = '\0';
    str->size = tmpSize;

    tmp = fgetc(fp);
  }

  // No data was changed
  if(data == str->data) return FAILURE;
  
  return SUCCESS;
}

Status string_insertion(String hString, FILE* fp) {
  string *str = (string *) hString;
  fprintf(fp, "%s", str->data);
  return SUCCESS;
}

void string_overwrite(String hTo, String hFrom) {
  string *pFrom = (string *) hFrom;
  char *data = pFrom->data;
  string_empty(hTo);

  for(; *data; data++) {
    string_push(hTo, *data);
  }
}

void string_empty(String hString) {
  string *pString = (string *) hString;
  for(int i = 0; i < pString->size; i++) {
    pString->data[i] = 0;
  }

  pString->size = 0;
}

Boolean string_isEmpty(String hString) {
  string *pString = (string *) hString;
  return (pString->size > 0) ? FALSE : TRUE;
}

Boolean string_contains(String hString, char item) {
  string *pString = (string *) hString;
  char *data = pString->data;
  for(; *data; data++) {
    if(*data == item) return TRUE;
  }

  return FALSE;
}

Status string_push(String hString, char item) {
  string *str = (string *) hString;
  
  if(str->size + 1 == str->capacity) {
    str->capacity *= 2;
    str->data = realloc(str->data, str->capacity * sizeof(char));      

    if(str->data == NULL) return FAILURE;
  }

  (str->data)[str->size] = item;
  (str->data)[++(str->size)] = '\0';
  
  return SUCCESS;
}

Status string_pop(String hString) {
  string *str = (string *) hString;

  if(str->size == 0) return FAILURE;
  str->size--;
  str->data[str->size] = '\0';  
  
  // printf("str:%s, size:%d, capacity:%d\n", str->data, str->size, str->capacity);  
  return SUCCESS;
}

Status string_concat(String hBase, String hAppend) {
  string *a = (string *) hBase;  
  string *b = (string *) hAppend;

  char *aData = a->data;
  char *bData = b->data;
  int newSize = a->size + b->size;

  if(newSize + 1 >= a->capacity) {    
    a->capacity *= (newSize / a->size) + 1;     
    a->data = realloc(a->data, a->capacity * sizeof(char));    

    for(int i = a->size + 1; i < a->capacity; i++) {
      a->data[i] = 0;
    }

    aData = a->data;
  }

  for(aData += a->size; *bData; aData++, bData++) {
    *aData = *bData;
  }
  a->size = newSize;
  return SUCCESS;
}

 //Precondition: pLeft is the address of a MY_STRING handle
// containing a valid MY_STRING object address OR NULL.
// The value of Right must be the handle of a valid MY_STRING object
//Postcondition: On Success pLeft will contain the address of a handle
// to a valid MY_STRING object that is a deep copy of the object indicated
// by Right. If the value of the handle at the address indicated by
// pLeft is originally NULL then the function will attempt to initialize
// a new object that is a deep copy of the object indicated by Right,
// otherwise the object indicated by the handle at the address pLeft will
// attempt to resize to hold the data in Right. On failure pLeft will be
// left as NULL and any memory that may have been used by a potential
// object indicated by pLeft will be returned to the freestore.
void string_assign(Item *pLeft, Item Right) {  
  string *right = (string *) Right;
  string *left;

  if(*pLeft == NULL) {
    left = string_init(right->data);
    *pLeft = (Item *) left;
    return;
  }

  left = (string *) pLeft;

  if(left->capacity < right->capacity) {
    left->capacity = right->capacity;
    left->data = realloc(left->data, left->capacity * sizeof(char));

    if(left->data == NULL) {
      string_free((String *)&left);
    }
  }

  cstring_cpy(right->data, left->data);
  left->size = right->size;
}

char *string_charat(String hString, int index) {
  string *str = (string *) hString;

  if(index >= str->size || index < 0) return NULL;

  return &(str->data[index]);
}

char *string_c(String hString) {
  string *str = (string *) hString;
  return str->data;
}

/**    Util functions    **/
int cstring_get_size(char *str) {
  int s = 0;
  for(; *str; str++, s++) {};  
  return s;
}

void cstring_cpy(char *from, char *to) {
  for(; *from; from++, to++) {
    *to = *from;
  }

  *to = '\0';
}

void string_free(String *pItem) {
  string *str = (string *) *pItem;
  free(str->data);
  free(str);
  *pItem = NULL;
}