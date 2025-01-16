#include <ncurses.h>
#include <string.h>
#include <stdlib.h>
#include "include/wagner_fischer.h"
#include "include/trie.h"

#define RESET "\x1b[0m"
#define BOLD  "\x1b[1m"
#define RED   "\x1b[31m"
#define MAGENTA  "\x1b[35m"
#define GREEN "\x1b[32m"

#define NORMAL_MODE 1
#define SPELLCHECK_MODE 0

int currentMode = NORMAL_MODE;

typedef struct Vec2 {
    int x;
    int y;
} Vec2;

typedef struct Word {
    Vec2 startPointer;
    Vec2 endPointer;
    struct Word* nextWord;
    int colourPair;   // each word can have either the green colour or red colour attribute
    int colourPairSet;
} Word;

Word* createWord() {
    Word* newWord = (Word*)malloc(sizeof(Word));
    newWord->startPointer.x = 0;
    newWord->startPointer.y = 0;
    newWord->endPointer.x = 0;
    newWord->endPointer.y = 0;
    newWord->nextWord = NULL;
    newWord->colourPairSet = 0;
    return newWord;
}

void addWord(Word** head, Word* newWord) {
    if (*head == NULL) {
        *head = newWord;
    } 
    else {
        Word* temp = *head;
        while (temp->nextWord != NULL) {
            temp = temp->nextWord;
        }
        temp->nextWord = newWord;
    }
}

void printList(Word* wordList) {
    Word* temp = wordList;
    while (temp != NULL) {
        printf("Word from (%d, %d) to (%d, %d)\n", 
               temp->startPointer.x, temp->startPointer.y, 
               temp->endPointer.x, temp->endPointer.y);
        temp = temp->nextWord;
    }
}

int setWordsAttribute(Word* wordList, TrieNode* trieRoot, int height, int width) {
    Word* temp = wordList;
    while (temp != NULL) {
        if (!temp->colourPairSet) {
            char word[100];
            int index = 0;
            for (int x = temp->startPointer.x; x < temp->endPointer.x; x++) {
                word[index++] = mvinch(temp->startPointer.y, x) & A_CHARTEXT; 
            }
            word[index] = '\0';
            int found = search(trieRoot, word);
            if (found) {
                temp->colourPair = 3;  // Green
                temp->colourPairSet = true;
            } else {
                temp->colourPair = 1;  // Red
                temp->colourPairSet = true;
                Suggestion check = suggest(word);
                for(int i = 0; i < check.num_suggestions; i++){
                    mvprintw(height - 2, 20 + i * ((width - 20) / check.num_suggestions), "%s ", check.suggestions[i]);
                }
            }
        }
        temp = temp->nextWord;
    }
    return 0;
}

void spellCheckPrint(Word* wordList) {
    Word* temp = wordList;
    while (temp != NULL) {
        attron(COLOR_PAIR(temp->colourPair) | A_BOLD);
        int startX = temp->startPointer.x;
        int startY = temp->startPointer.y;
        int endX = temp->endPointer.x;
        move(startY, startX);
        for (int x = startX; x < endX; x++) {
            int ch = mvinch(startY, x) & A_CHARTEXT; 
            mvprintw(startY, x, "%c", ch);
        }
        attroff(COLOR_PAIR(temp->colourPair) | A_BOLD);
        temp = temp->nextWord;
    }
}

int main(int argc, char** argv) {
    if (argc > 1 && strcmp(argv[1], "--help") == 0) {
        printf(BOLD MAGENTA "\nThis is a spellchecker.\n\n" RESET BOLD "Run without any flags to use it.\n\n" RESET);
        printf(BOLD "Type normally. To spellcheck, press the " GREEN "F9 " RESET BOLD "key. Press F9 again to return to normal.\n\n" RESET);
        printf(BOLD "To go up after you have backspace-ed the entire line, press the " GREEN "UP " BOLD "arrow key.\n\n" RESET);
        return 0;
    }

    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();

    int ch, x = 0, y = 0, modeChanged = 0, wordFound = 0;
    int height, width;

    getmaxyx(stdscr, height, width);

    if (has_colors()) {
        start_color();
        init_pair(0, COLOR_YELLOW, COLOR_BLACK);
        init_pair(1, COLOR_RED, COLOR_BLACK);
        init_pair(2, COLOR_MAGENTA, COLOR_BLACK);
        init_pair(3, COLOR_BLUE, COLOR_BLACK);
    }

    Word* wordList = NULL;
    Word* currentWord = createWord();

    TrieNode* trieRoot = createNode();
    loadDictionary(trieRoot, "words.txt");

    while ((ch = getch()) != KEY_F(4)) {
        if (wordFound) {
            //printw("x:%d and y:%d", x, y);
            currentWord->startPointer.x = x;
            currentWord->startPointer.y = y;
            wordFound = 0;
        }
        
        if (ch == KEY_F(9)) {
            currentMode = currentMode == NORMAL_MODE ? SPELLCHECK_MODE : NORMAL_MODE;
            modeChanged = 1;
        }

        if (currentMode == NORMAL_MODE) {
            attron(COLOR_PAIR(2) | A_BOLD);
            mvprintw(height - 2, 2, "NORMAL");
            clrtoeol();
            attroff(COLOR_PAIR(2) | A_BOLD);
            
            if (modeChanged) {
                modeChanged = 0;
                continue;
            }

            move(y, x);
            if (ch == KEY_BACKSPACE) {
                if (x > 0) {
                    x--;
                    move(y, x);
                    delch();
                }
            } else if (ch == '\n' || ch == '\r') {
                y++;
                x = 0;
                printw("\n");
            } else if (ch == KEY_UP && y > 0) {
                y--;
                x = width;
            }  else if (ch == KEY_DOWN && y > 0) {
                y++;
                x = width;
            } else {
                printw("%c", ch);
                if (ch == ' ') {
                    //printw("x:%d and y:%d", x, y);
                    currentWord->endPointer.x = x;
                    currentWord->endPointer.y = y;
                    addWord(&wordList, currentWord);
                    currentWord = createWord();
                    wordFound = 1;
                }
                x++;
            }
            move(y, x);
            refresh();
        } 
        else {
            Word* temp = wordList;
            while (temp != NULL) {
                temp->colourPairSet = false;
                temp = temp->nextWord;
            }
            attron(COLOR_PAIR(1) | A_BOLD);
            mvprintw(height - 2, 2, "SPELLCHECK");
            clrtoeol();
            attroff(COLOR_PAIR(1) | A_BOLD);
            int result = setWordsAttribute(wordList, trieRoot, height, width);
            spellCheckPrint(wordList);
            refresh();
        }
    }

    refresh();
    endwin();

    printList(wordList);

    // just some clean up - following good practices ;)
    Word* temp = wordList;
    while (temp != NULL) {
        Word* next = temp->nextWord;
        free(temp);
        temp = next;
    }
    return 0;
}
