#ifndef WAGNER_FISCHER_H
#define WAGNER_FISCHER_H
#define MAX_SUGGESTIONS 3
#define MAX_LENGTH 100

typedef struct Suggestion {
    char suggestions[MAX_SUGGESTIONS][MAX_LENGTH + 1];
    int num_suggestions;
} Suggestion;

int wagner_fischer(const char* str1, const char* str2);
Suggestion suggest(const char*);

#endif
