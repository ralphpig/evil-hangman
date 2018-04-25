#ifndef D_VECTOR
#define D_VECTOR
  #include <stdlib.h>
  #include "../generic.h"

  typedef void* Vector;  
 
  Vector vector_init();
  Status vector_push(Vector hVector, Item item);
  Status vector_pop(Vector hVector);
  void vector_forEach(Vector hVector, void (*forEach)(Item));
  void vector_overwrite(Vector hTo, Vector hFrom);
  void vector_empty(Vector hVector);
  Item vector_getData(Vector hVector, unsigned int index);
  int vector_getCapacity(Vector hVector);
  int vector_getSize(Vector hVector);  
  void vector_free(Vector *phVector);

#endif