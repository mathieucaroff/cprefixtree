#ifndef LIB_TREETABLE_H_INCLUDED
#define LIB_TREETABLE_H_INCLUDED

typedef unsigned long count_t;
typedef unsigned char arraySize_t;
typedef char character_t; // Can be signed or not.
typedef unsigned char ucharacter_t; // must be the unsigned version of character_t

typedef enum {
  pft_mode_NODE, pft_mode_LEAF
} prefixTreeMode;

typedef union PrefixTree PrefixTree;
union PrefixTree {
  struct {
    prefixTreeMode mode;
    count_t count;
    arraySize_t arraySize;
    character_t** wordarray;
  };
  struct {
    prefixTreeMode ALTmode;
    count_t ALTcount; // I think the ALTnames are useless but don't know the correct substitute.
    arraySize_t ALTarraySize;
    PrefixTree** children;
  };
};

/// PUBLIC Methods
PrefixTree* pft_create (arraySize_t arraySize);

void pft_free (PrefixTree* prefixTree);

void pft_add (PrefixTree* prefixTree, character_t* word);

void pft_remove (PrefixTree* prefixTree, character_t* word);

bool pft_present (PrefixTree* prefixTree, character_t* word);

bool pft_prefix_present (PrefixTree* prefixTree, character_t* word);

PrefixTree* pft_get_prefix_element (PrefixTree* prefixTree, character_t* word);


/// PRIVATE Methods
bool pft_util_str_is_prefix (const character_t * prefix, const character_t * string);

void pft_convert_leaf_to_node (PrefixTree* prefixTree);

void pft_convert_node_to_leaf (PrefixTree* prefixTree);

//// LEAF
void pft_leaf_add (PrefixTree* prefixTree, character_t* word);

void pft_leaf_remove (PrefixTree* prefixTree, character_t* word);

bool pft_leaf_present (PrefixTree* prefixTree, character_t* word);

bool pft_leaf_prefix_present (PrefixTree* prefixTree, character_t* word);

PrefixTree* pft_leaf_get_prefix_element (PrefixTree* prefixTree, character_t* word);

//// NODE
void pft_node_add (PrefixTree* prefixTree, character_t* word);

void pft_node_remove (PrefixTree* prefixTree, character_t* word);

bool pft_node_present (PrefixTree* prefixTree, character_t* word);

bool pft_node_prefix_present (PrefixTree* prefixTree, character_t* word);

PrefixTree* pft_node_get_prefix_element (PrefixTree* prefixTree, character_t* word);

#endif
