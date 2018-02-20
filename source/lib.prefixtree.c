/*
 * @auther Mathieu CAROFF
 * @license THE-UNLICENSE
 * @description @see .h
*/

#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<stdbool.h>

#include"../header/lib.prefixtree.h"

#define MALLOC(type) malloc(sizeof(type))
#define CALLOC(n, type) calloc(n, sizeof(type))

/// Public Methods
PrefixTree* pft_create (arraySize_t arraySize) {
  PrefixTree* prefixTree = MALLOC(PrefixTree);
  prefixTree->mode = pft_mode_LEAF;
  prefixTree->count = 0;
  prefixTree->arraySize = arraySize;
  prefixTree->wordarray = CALLOC(arraySize, character_t*);
  return prefixTree;
}

void pft_free (PrefixTree* prefixTree) {
  if (prefixTree->count != 0) {
    perror("WARNING: pft_free on non-void prefixtree element. Has been ignored.");
  } else if (prefixTree->mode != pft_mode_LEAF) {
    perror("WARNING: pft_free on non-leaf prefixtree element. This an unexpected internal error of the prefix tree library. Has been ignored.");
  } else {
    free(prefixTree->wordarray);
    free(prefixTree);
  }
}

void pft_add (PrefixTree* prefixTree, character_t* word) {
  if (prefixTree->mode == pft_mode_LEAF)
    pft_leaf_add(prefixTree, word);
  else
    pft_node_add(prefixTree, word);
}

void pft_remove (PrefixTree* prefixTree, character_t* word) {
  if (prefixTree->mode == pft_mode_LEAF)
    pft_leaf_remove(prefixTree, word);
  else
    pft_node_remove(prefixTree, word);
}

bool pft_present (PrefixTree* prefixTree, character_t* word) {
  bool found = 0;
  if (prefixTree->mode == pft_mode_LEAF)
    found = pft_leaf_present(prefixTree, word);
  else
    found = pft_node_present(prefixTree, word);
  return found;
}

bool pft_prefix_present (PrefixTree* prefixTree, character_t* word) {
  bool found = 0;
  if (prefixTree->mode == pft_mode_LEAF)
    found = pft_leaf_prefix_present(prefixTree, word);
  else
    found = pft_node_prefix_present(prefixTree, word);
  return found;
}

PrefixTree* pft_get_prefix_element (PrefixTree* prefixTree, character_t* word) {
  PrefixTree* element;
  if (prefixTree->mode == pft_mode_LEAF)
    element = pft_leaf_get_prefix_element(prefixTree, word);
  else
    element = pft_node_get_prefix_element(prefixTree, word);
  return element;
}

/// Private Methods
bool pft_util_str_is_prefix (const character_t * prefix, const character_t * string) {
    while (*prefix && *string) {
        if (*prefix++ != *string++)
            return false;
    }
    return !*prefix;
}

void pft_convert_leaf_to_node (PrefixTree* prefixTree) {
    count_t count = prefixTree->count;
    character_t** wordarray = prefixTree->wordarray;
    prefixTree->children = CALLOC(sizeof(character_t) << 8, PrefixTree*);
    prefixTree->mode = pft_mode_NODE;
    arraySize_t c = (arraySize_t) prefixTree->count;
  for (arraySize_t i = 0 ; i < c ; i++) {
        pft_add(prefixTree, wordarray[i]);
  }
  prefixTree->count = count;
  free(wordarray);
}


// We are going to copy the content of each non NULL children wordarray into the newly created parent wordarray.
void pft_convert_node_to_leaf (PrefixTree* prefixTree) {
  PrefixTree** children = prefixTree->children;
  character_t** wordarray = CALLOC(prefixTree->arraySize, character_t*);
  
  // Outer loop:
  ucharacter_t old = 0; old = ~old; // Outer loop delimiter
  // od is the maximum value of it's type
  ucharacter_t olc = 0; // Outer loop counter
  // Inner loop:
  arraySize_t gilc = 0; // Global inner loop counter
  arraySize_t lilc; // Local inner loop counter
  arraySize_t ild; // Inner loop delimiter
  character_t** childwordarray; // Inner loop array pointer
  while (1) {
    if (children[olc] != NULL) {
      childwordarray = children[olc]->wordarray;
      ild = children[olc]->count;
      for (lilc = 0; lilc < ild ; lilc++, gilc++){
	wordarray[gilc] = childwordarray[lilc]-1;
      }
      free(childwordarray);
      free(children[olc]);
      children[olc] = NULL;
    }
    if (olc == old)
      break;
    olc++;
  }
  prefixTree->wordarray = wordarray;
  prefixTree->mode = pft_mode_LEAF;
}

//// LEAF
void pft_leaf_add (PrefixTree* prefixTree, character_t* word) {
  if (pft_leaf_present(prefixTree, word)) {
    // do nothing
    // this avoids to have multiple insertion of the same word
    // which could lead to segmentation error when reaching end of words
  } else if (prefixTree->count >= prefixTree->arraySize) {
    pft_convert_leaf_to_node(prefixTree);
    pft_add(prefixTree, word);
  } else {
    prefixTree->wordarray[prefixTree->count] = word;
    prefixTree->count++;
  }
}

void pft_leaf_remove (PrefixTree* prefixTree, character_t* word) {
  arraySize_t c = (arraySize_t) prefixTree->count;
  character_t** wordarray = prefixTree->wordarray;
  for (arraySize_t i = 0 ; i < c ; i++) {
    if (!strcmp(wordarray[i], word)) {
      wordarray[i] = wordarray[c-1];
      wordarray[c-1] = NULL;
      prefixTree->count = c-1;
      break;
    }
  }
}

bool pft_leaf_present (PrefixTree* prefixTree, character_t* word) {
  bool found = 0;
  arraySize_t c = (arraySize_t) prefixTree->count;
  character_t** wordarray = prefixTree->wordarray;
  for (arraySize_t i = 0 ; i < c  && !found; i++) {
    if (!strcmp(wordarray[i], word))
      found = 1;
  }
  return found;
}

bool pft_leaf_prefix_present (PrefixTree* prefixTree, character_t* word) {
  bool found = false;
  arraySize_t c = (arraySize_t) prefixTree->count;
  character_t** wordarray = prefixTree->wordarray;
  for (arraySize_t i = 0 ; i < c  && !found; i++) {
    if (pft_util_str_is_prefix(word, wordarray[i]))
      found = 1;
  }
  return found;
}

PrefixTree* pft_leaf_get_prefix_element (PrefixTree* prefixTree, character_t* word) {
  PrefixTree* element = pft_create(prefixTree->arraySize);
  unsigned long wordlen = strlen(word);
  arraySize_t ec = 0; // element->count
  arraySize_t tc = prefixTree->count;
  for (arraySize_t i = 0; i < tc; i++) {
    if (pft_util_str_is_prefix(word, prefixTree->wordarray[i])) {
      element->wordarray[ec] = prefixTree->wordarray[i] + wordlen;
      ec++;
    }
  }
  element->count = ec;
  return element;
}

//// NODE
void pft_node_add (PrefixTree* prefixTree, character_t* word) {
  ucharacter_t ch = *word;
  PrefixTree** childPtr = prefixTree->children + ch;
  if (*childPtr == NULL)
    *childPtr = pft_create(prefixTree->arraySize);
  pft_add(*childPtr, word+1);
  prefixTree->count++;
}

void pft_node_remove (PrefixTree* prefixTree, character_t* word) {
  unsigned char ch = *word;
  PrefixTree** childPtr = prefixTree->children + ch;
  if (*childPtr == NULL) {
    // do nothing
  } else {
    pft_remove(*childPtr, word+1);
    if ((*childPtr)->count == 0) {
      pft_free(*childPtr);
    }
    prefixTree->count--;
  }
  if (prefixTree->count <= prefixTree->arraySize) {
    pft_convert_node_to_leaf(prefixTree);
  }
}

bool pft_node_present (PrefixTree* prefixTree, character_t* word) {
  bool found;
  ucharacter_t ch = *word;
  PrefixTree** childPtr = prefixTree->children + ch;
  if (*childPtr == NULL)
    found = 0;
  else
    found = pft_present(*childPtr, word+1);
  return found;
}

bool pft_node_prefix_present (PrefixTree* prefixTree, character_t* word) {
  bool found;
  ucharacter_t ch = *word;
  PrefixTree** childPtr = prefixTree->children + ch;
  if (!*word)
    found = 1;
  else if (*childPtr == NULL)
    found = 0;
  else
    found = pft_prefix_present(*childPtr, word+1);
  return found;
}

PrefixTree* pft_node_get_prefix_element (PrefixTree* prefixTree, character_t* word) {
  PrefixTree* element;
  ucharacter_t ch = *word;
  PrefixTree** childPtr = prefixTree->children + ch;
  if (!*word)
    element = prefixTree;
  else if (*childPtr == NULL)
    element = NULL;
  else
    element = pft_get_prefix_element(*childPtr, word+1);
  return element;
}
