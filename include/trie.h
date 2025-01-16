#ifndef TRIE_H
#define TRIE_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h> 
#define ALPHABET_SIZE 26

typedef struct TrieNode {
    struct TrieNode* children[ALPHABET_SIZE];
    bool isEndOfWord;
} TrieNode;

TrieNode* createNode();
void insert(TrieNode*, const char*);
bool search(TrieNode*, const char*);
void loadDictionary(TrieNode*, const char*);

#endif