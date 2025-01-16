#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "include/wagner_fischer.h"
#define min(a, b) ((a) < (b) ? (a) : (b))

int wagner_fischer(const char* str1, const char* str2) {
    int str1_len = strlen(str1);
    int str2_len = strlen(str2);
    int matrix[str1_len + 1][str2_len + 1];

    // base cases
    for (int i = 0; i <= str1_len; i++) matrix[i][0] = i;
    for (int j = 0; j <= str2_len; j++) matrix[0][j] = j;

    for (int i = 1; i <= str1_len; i++) {
        for (int j = 1; j <= str2_len; j++) {
            int cost = (str1[i - 1] == str2[j - 1]) ? 0 : 1; 
            matrix[i][j] = min(min(matrix[i - 1][j] + 1,     // Deletion
                               matrix[i][j - 1] + 1),         // Insertion
                               matrix[i - 1][j - 1] + cost);  // Substitution
        }
    }
    return matrix[str1_len][str2_len];
}

Suggestion suggest(const char* word) {
    Suggestion result;
    result.num_suggestions = 0;
    int min_distance = INT_MAX;
    int threshold = 4;
    FILE *file = fopen("programDictionary.txt", "r");
    if (file == NULL) {
        printf("Error opening dictionary file\n");
        return result;
    }

    char dict_word[MAX_LENGTH + 1];
    while (fscanf(file, "%s", dict_word) != EOF) {
        int distance = wagner_fischer(word, dict_word);
        if (distance < min_distance && distance < threshold) {
            result.num_suggestions = 0;
            strcpy(result.suggestions[result.num_suggestions], dict_word);
            result.num_suggestions++;
            min_distance = distance;
        } 
        else if (distance == min_distance && distance < threshold) {
            strcpy(result.suggestions[result.num_suggestions], dict_word);
            result.num_suggestions++;
        }
        if (result.num_suggestions == MAX_SUGGESTIONS) {
            break;
        }
    }

    fclose(file);
    return result;
}

// int main(){
//     Suggestion check = suggest("pease");
//     for(int i = 0; i < check.num_suggestions; i++){
//         printf("%s\n", check.suggestions[i]);
//     }
//     return 0;
// }