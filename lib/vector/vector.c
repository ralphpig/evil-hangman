#include "vector.h"

//KNOWN TYPE
typedef struct {
  int size;
  int capacity;
  Item *data;
} vector;

Vector vector_init() {
  vector *pVector = (vector *) malloc(sizeof(vector));
  
  pVector->size = 0;
  pVector->capacity = 1;
  pVector->data = (Item *) malloc(sizeof(Item) * pVector->capacity);
  
  if (pVector->data == NULL) {
    free(pVector);
    return NULL;
  }
  
  return (Vector) pVector;
}

Status vector_push(Vector hVector, Item item) {
  vector *pVector = (vector *) hVector;
  // printf("Push to vector: %u\n", hVector);
  
  if (pVector->size >= pVector->capacity) {
    pVector->capacity *= 2;
    pVector->data = realloc(pVector->data, sizeof(Item) * pVector->capacity);
    if (pVector->data == NULL) {
      return FAILURE;
    }    
  } 
  pVector->data[pVector->size++] = item;
  return SUCCESS;
}

Status vector_pop(Vector hVector) {
  vector *pVector = (vector *) hVector;
  if (pVector->size == 0) {
    return FAILURE;
  }

  pVector->size--;
  return SUCCESS;
}

void vector_forEach(Vector hVector, void (*forEach)(Item)) {
  vector *pVector = (vector *) hVector;
  for(int i = 0; i < pVector->size; i++) {
    forEach(pVector->data[i]);
  }
}

void vector_overwrite(Vector hTo, Vector hFrom) {  
  vector *pFrom = (vector *) hFrom;  
  vector_empty(hTo);  

  for(int i = 0; i < pFrom->size; i++) {
    vector_push(hTo, pFrom->data[i]);
  }
}

void vector_empty(Vector hVector) {
  vector *pVector = (vector *) hVector;
  Item *data = pVector->data;
  for(int i = pVector->size; i > 0; i--) {
    data[i - 1] = 0;
  }

  pVector->size = 0;
}

Item vector_getData(Vector hVector, unsigned int index) {
  vector *pVector = (vector *) hVector;
  if (index >= pVector->size) return NULL;

  return pVector->data[index];
}

int vector_getSize(Vector hVector) {
  vector *pVector = (vector *) hVector;  
  return pVector->size; 
}

// int vector_getCapacity(Vector hVector) {}
void vector_free(Vector *phVector) {
  vector *pVector = (vector *) *phVector;  
  free(pVector->data);
  free(pVector);
  *phVector = NULL;
  return;
}
