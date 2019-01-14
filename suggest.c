#include <stdlib.h>
#include <string.h>
#include "suggest.h"
#include "spellcheck.h"

struct wordlist {
  struct trie *trie;
  int length;
};


void swap(char *px, char *py) {
  char temp = *px;
  *px = *py;
  *py = temp;
}


char *my_strdup(const char *s) {
  char *newstr = malloc((strlen(s) + 1) * sizeof(char));
  strcpy(newstr, s);
  return newstr;
}


void *single_deletion(const char *word, struct wordlist *wl,
                      bool (*is_word)(const char *)) {
  int len = strlen(word);
  for (int c = 0; c < len; c++) {
    char *str = my_strdup(word);
    int h = 0;
    for (int i = 0; i < len; i++) {
      if (i == c) {
        continue;
      } else {
        str[h++] = word[i];
      }
    }
    str[len - 1] = '\0';
    if (is_word(str) == true && (strcmp(str, word) != 0)) {
      wordlist_add(str,wl);
    }
    free(str);
  }
  return wl;
}


struct wordlist *single_insertion(const char *word, struct wordlist *wl,
                                  bool (*is_word)(const char *)) {  
  int len = strlen(word) + 1;
  for (int c = 0; c < len; c++) {    
    for (char let = 'a'; let <= 'z'; let++) {
      char *str = (char*)malloc(sizeof(char)*len + 2);
      int h = 0;
      int i = 0;
      while (i < len) {
        if (h == c) {
          str[h++] = let;
        } else {
          str[h++] = word[i];
          i++;
        }
      }
      str[len + 1] = '\0';
      if (is_word(str) == true && (strcmp(str, word) != 0)) {
        wordlist_add(str,wl);
      }
      free(str);
    }
  }
  return wl;
}


struct wordlist *single_swaps(const char *word, struct wordlist *wl,
                              bool (*is_word)(const char *)) {
  int len = strlen(word);
  for(int i = 0; i < len - 1; i++) {
    char *str = my_strdup(word);
    swap(&str[i], &str[i + 1]);
    if (is_word(str) == true && (strcmp(str, word) != 0)) {
      wordlist_add(str,wl);
    }
    free(str);
  } 
  return wl;
}


struct wordlist *single_substitutions(const char *word, struct wordlist *wl,
                                      bool (*is_word)(const char *)) {
  int len = strlen(word);
  for(int j = 0; j < len; j++) {
    for(int i = 0; i < 26; i++) {
      char let = i +'a';
      char *str = my_strdup(word);
      str[j] = let;
      if (is_word(str) == true && (strcmp(str, word) != 0)) {
        wordlist_add(str,wl);
      }
      free(str);
    }
  }
  return wl;
}


struct wordlist *suggest(const char *s,
                         bool (*is_word)(const char *)) {
  while (1) {
    struct wordlist *wl = wordlist_create();
    if (is_word) {
      single_deletion(s, wl, is_word);
      single_insertion(s, wl, is_word);
      single_substitutions(s, wl, is_word);
      single_swaps(s, wl, is_word);
      return wl;
    }
    break;
  }
  return NULL;
}
