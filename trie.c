#include "include/trie.h"

// prefix tree
TrieNode* createNode() {
    TrieNode* node = (TrieNode*)malloc(sizeof(TrieNode));
    if (node == NULL) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    node->isEndOfWord = false;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        node->children[i] = NULL;
    }
    return node;
}

void insert(TrieNode* root, const char* word) {
    TrieNode* currentNode = root;

    for (int i = 0; word[i] != '\0'; i++) {
        char ch = tolower(word[i]);
        if (ch < 'a' || ch > 'z') {
            continue;
        }
        int index = ch - 'a';
        if (currentNode->children[index] == NULL) {
            currentNode->children[index] = createNode();
        }
        currentNode = currentNode->children[index];
    }
    currentNode->isEndOfWord = true;
}

bool search(TrieNode* root, const char* word) {
    TrieNode* currentNode = root;
    for (int i = 0; word[i] != '\0'; i++) {
        char ch = tolower(word[i]);
        if (ch < 'a' || ch > 'z') {
            continue;
        }
        int index = ch - 'a'; 
        if (currentNode->children[index] == NULL) {
            return false;
        }
        currentNode = currentNode->children[index];
    }
    return currentNode != NULL && currentNode->isEndOfWord;
}

void loadDictionary(TrieNode* root, const char* dictionaryFile) {
    FILE* file = fopen(dictionaryFile, "r");
    if (file == NULL) {
        printf("Error opening dictionary file.\n");
        return;
    }
    char word[100];
    while (fscanf(file, "%s", word) != EOF) {
        insert(root, word);
    }
    fclose(file);
}

// int main() {
//     TrieNode* trieRoot = createNode();
//     loadDictionary(trieRoot, "programDictionary.txt");

//     printf("Searching for 'hello': %s\n", search(trieRoot, "hello") ? "Found" : "Not Found");
//     printf("Searching for 'world': %s\n", search(trieRoot, "world") ? "Found" : "Not Found");
//     printf("Searching for 'nece': %s\n", search(trieRoot, "trie") ? "Found" : "Not Found");
//     printf("Searching for 'test': %s\n", search(trieRoot, "test") ? "Found" : "Not Found");
//     printf("Searching for 'notfound': %s\n", search(trieRoot, "notfound") ? "Found" : "Not Found");

//     return 0;
// }
