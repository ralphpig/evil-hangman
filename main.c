#include <stdio.h>
#include "lib/string/string.h"
#include "lib/vector/vector.h"
#include "lib/tree/tree.h"

int getLength(Vector *dictionary);
int getInt();
int getChar();
void kflush();

FTNode buildFamilyTree(Vector wordChoices, String currentFamily, char guess);

String wordToFamily(String currentFamily, String word, char guess);
int familyToKey(String word);

Boolean isGameWin(String currentFamily);

// Traversal iterator
void treePrint(int key, String family, Vector words) {
  printf("%-4d | %s : %d\n", key, string_c(family), vector_getSize(words));
}



int main(int argc, char const *argv[]) {
  system("clear");
  printf("\tHANGMAN\n\n");

  // Read dictionary and build data structure
  Vector dictionary[30] = { 0 };  
  String tmpStr = string_init_default();  
  FILE *fp = fopen("dictionary.txt", "r"); 

  if(tmpStr == NULL) return 1;
  if(fp == NULL) return 1;
  
  while(string_extraction(tmpStr, fp)) {
    if(dictionary[string_get_size(tmpStr)] == NULL) {
      dictionary[string_get_size(tmpStr)] = vector_init();
    }
    // printf("Read string: %s\n", string_c(tmpStr));
    vector_push(dictionary[string_get_size(tmpStr)], (Item) string_init(string_c(tmpStr)));
  }

  fclose(fp);



  // Game init  
    printf("How long of a word? (2 - 29) : ");
  int wordLength = getLength(dictionary);

    printf("How many guesses? : ");
  int guesses = getInt();

  Vector wordChoices = vector_init();
  String guessList = string_init_default();
  String guessListFull = string_init_default();
  String currentFamily = string_init_default();
  FTNode tree = NULL;

  vector_overwrite(wordChoices, dictionary[wordLength]);

  // Game loop
  while(1) {
    tree_free(&tree);    

      printf("\nEnter guess: ");
    char guess = getChar();    

    if(string_contains(guessList, guess)) {
      printf("\nYou already guessed '%c'.\n", guess);
      continue;
    }
    
    system("clear");

    // Build Family tree    
    tree = buildFamilyTree(wordChoices, currentFamily, guess);

    // Build guess list
    (!string_isEmpty(guessListFull)) ? string_push(guessListFull, ','), string_push(guessListFull, ' ') : 0; // Add comma

    string_push(guessList, guess);    
    string_push(guessListFull, guess);    

    // Print 
    printf("\nPossible word families:\n");
    tree_inorder(tree, &treePrint);

    FTNode max = tree_getMaxWords(tree);

    if(!max) {
      printf("\nWrong.\n");
      guesses--;      
    } else {
      vector_overwrite(wordChoices, tree_getWords(max));
      string_overwrite(currentFamily, tree_getFamily(max));

      if(string_contains(currentFamily, guess)) {
        printf("\nCorrect.\n");
      } else {      
        printf("\nWrong.\n");
        guesses--;
      }
    }    

    if(!guesses) {
      printf("You're out of guesses. Try again.\n");
      break;
    }

    if(isGameWin(currentFamily)) {
      printf("\nCongratulations, you won! The word was '%s'.\n", string_c(currentFamily));
      break;
    }

    printf("\t%s\n[%s]\n%d guesses left\n", string_c(currentFamily), string_c(guessListFull), guesses); 
  }


  // Clean up  
  tree_free(&tree);  
  string_free(&guessList);
  string_free(&guessListFull);
  string_free(&currentFamily);
  string_free(&tmpStr);
  vector_free(&wordChoices);

  // Free dictionary
  for(int l = 0; l < 30; l++) { 
    if(!dictionary[l]) continue;
    for(int s = 0; s < vector_getSize(dictionary[l]); s++) {      
      String str = (String) vector_getData(dictionary[l], s);
      string_free(&str);
    }
    vector_free(&dictionary[l]);
  }

  return 0;
}

FTNode buildFamilyTree(Vector wordChoices, String currentFamily, char guess) {
  FTNode tree = NULL;
  for(int i = 0; i < vector_getSize(wordChoices); i++) {
    String word = vector_getData(wordChoices, i);
    String family = wordToFamily(currentFamily, word, guess);

    if(!family) continue;

    int key = familyToKey(family);

    // printf("Key: %d. Word: %s. Family: %s\n", key, string_c(word), string_c(family));
    tree = tree_insert(tree, key, family, word);
  }

  return tree;
}


int familyToKey(String word) {
  char *string = string_c(word);
  int key = 0;
  for(int i = 1; *string; string++, i++) {
    key += (i * *string);
  }
  return key;
}


String wordToFamily(String hCurrentFamily, String hWord, char guess) {  
  String family = string_init_default();
  char *word = string_c(hWord);
  char *cFamily = string_c(hCurrentFamily);  

  for(; *word; word++, (*cFamily) ? cFamily++ : 0) {
    char next = '-';

    if(guess == *word) next = guess;
    if(*cFamily && *cFamily != '-') next = *cFamily;    

    string_push(family, next);
  }

  // printf("wordToFamily (%s, %s, %c) : %s\n", string_c(hCurrentFamily), string_c(word), guess, string_c(family));
  return family;
}

Boolean isGameWin(String currentFamily) {
  char *str = string_c(currentFamily);
  for(; *str; str++) {
    if(*str == '-') return FALSE;
  }

  return TRUE;
}


// Util
int getLength(Vector *dictionary) {
  int length;
  int valid;
  do {
    length = getInt();
    valid = (length >= 2 && length <= 29 && dictionary[length]);    
    if(!valid) {      
      if(!dictionary[length]) printf("Sorry, I don't know any words that length. Again: ");
      else printf("Length between 2 & 29. Again: ");
      
      continue;
    }

    break;
  } while (!valid);

  return length;
}

int getInt() {
   int input;
   int valid;

   do {      
      valid = scanf("%d", &input);
      kflush();
      valid = (valid && input >= 1);
      if(!valid) {
         printf("\tInvalid input. Again: ");
         continue;
         
      }

      break;    
   } while(!valid);

   return input;
}

int getChar() {
   char input;
   int valid;

   do {      
      valid = scanf("%c", &input);
      kflush();
      valid = (valid && input >= 97 && input <= 122);
      
      if(!valid) {
        printf("\tInvalid character. Again: ");
        continue;
      }

      break;
   } while(!valid);

   return input;
}

void kflush() {
   char c;
   while ((c = getchar()) != '\n' && c != EOF) {}
}