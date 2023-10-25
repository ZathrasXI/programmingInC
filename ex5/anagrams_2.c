#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define WORDLEN 20

#define FILENAME "anagrams.txt"
// for each word in file get all anagrams and store them, doesn't choose a word if it has already been selected as an anagram of a previous word


typedef struct word {
    char word[100];
    char anagram_of[100];
    struct word* next;
} Word;

Word* addWord(const char str[]);

int main(int argc, char* argv)
{
    char buffer[WORDLEN];
    strcpy(buffer, argv[1]);

    Word* start;
    start = NULL;

    FILE* f = fopen(FILENAME, "r");
    if (!f)
    {
        fprintf(stderr, "file not opened\n");
    }

    while(fscanf(f, "%s", buffer) == 1)
    {
        start = addWord(buffer);
    }
    return 0;
}


Word* addWord(const char str[])
{
    Word* w;
    w = (Word *) calloc(1, sizeof(Word));
    strcpy(w->word, str);
    w->next = NULL;
    return w;
}


void print_list(Word* w)
{
    printf("\n");
    do
    {
        printf("word: %s\n", w->word);
        w = w->next;
    } while(w != NULL);
    printf("END\n");
}