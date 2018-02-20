#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include"../header/lib.prefixtree.h"

#define SHOWI(expr) printf(#expr " -- %i\n", expr);

void test_lib_prefixtree ();

int main (int argc, char** argv) {
  test_lib_prefixtree();
  return 0;
}

void test_lib_prefixtree () {
  arraySize_t max_array_size = 2;
  PrefixTree* prefixTree = pft_create(max_array_size);
  PrefixTree* swap;
  PrefixTree* alternative;
#define ADD(word) {printf("Adding "#word"\n"); pft_add(prefixTree, word);}
#define PRESENT(word) {printf(#word " present: %d\n", pft_present(prefixTree, (word)));}
#define REMOVE(word) {printf("Removing "#word"\n"); pft_remove(prefixTree, word);}
#define SWAP() {printf("Swapping\n"); swap = prefixTree; prefixTree = alternative; alternative = swap;}
  ADD("a")
  ADD("aa")
  ADD("ab")
  ADD("ac")
  ADD("ad")
  ADD("ba")
  ADD("")
  PRESENT("aa")
  PRESENT("ad")
  PRESENT("ba")
  PRESENT("")
  PRESENT("az")
  PRESENT("bz")
  REMOVE("ba")
  PRESENT("aa")
  PRESENT("ad")
  PRESENT("ba")
  PRESENT("az")
  PRESENT("bz")
  
  alternative = pft_get_prefix_element(prefixTree, "a");
  SWAP()
  PRESENT("")
  PRESENT("b")
  PRESENT("d")
  PRESENT("z")
  PRESENT("ab")
  PRESENT("az")
  SWAP()
  
  alternative = pft_get_prefix_element(prefixTree, "ab");
  SWAP()
  PRESENT("")
  PRESENT("z")
  
  REMOVE("ab")
  REMOVE("ac")
  REMOVE("ad")
  REMOVE("ba")
  REMOVE("")
}
