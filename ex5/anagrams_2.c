#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define WORDLEN 20
#define FILENAME "anagrams.txt"

typedef struct word {
    char word[100];
    char anagram_group[100];
    bool anagram_found;
    struct word* next;
    int anagrams;
} Word;

bool is_anagram(const char input[], const char word[]);
void sort(char str[WORDLEN]);
void add_word_to_list(Word** start, char word[WORDLEN]);
void print_list(Word* w);
void find_anagrams(Word* start);
void print_anagrams(Word* start);
bool next_anagram(Word *w);

int main(void)
{

    char buffer[WORDLEN];
    Word* start = NULL;

    FILE* f = fopen(FILENAME, "r");
    if (!f)
    {
        fprintf(stderr, "file not opened\n");
    }

    while(fscanf(f, "%s", buffer) == 1)
    {
        add_word_to_list(&start, buffer);
    }

    find_anagrams(start);
    print_anagrams(start);
    return 0;
}

void find_anagrams(Word* w)
{
    if(w == NULL)
    {
        fprintf(stderr, "find_anagrams() received a NULL pointer\n");
        exit(EXIT_FAILURE);
    }

    // for each word in the linked list, compare it all of the other words in the list and check if they are anagrams
    // updating the list if they are
    Word* word_to_compare = w;
    Word* comparison = w;
    do
    {
        word_to_compare->anagrams = 0;
        do
        {
            if((is_anagram(comparison->word, word_to_compare->word) || strcmp(comparison->word, word_to_compare->word) == 0) && !comparison->anagram_found)
            {
                comparison->anagram_found = true;
                strcpy(comparison->anagram_group,word_to_compare->word);
                word_to_compare->anagrams++;                
            }
            comparison = comparison->next;
        } while(comparison != NULL);

        comparison = w;
        word_to_compare = word_to_compare->next;

    } while(word_to_compare != NULL);
}

void add_word_to_list(Word** start, char word[WORDLEN])
{
    Word* new = (Word*) calloc(1, sizeof(Word));
    strcpy(new->word, word);
    new->anagram_found = false;

    new->next = *start;
    *start = new;
}

bool is_anagram(const char input[], const char word[])
{
    if (strcmp(input, word) == 0)
    {
        return false;
    }
    char sorted_word[WORDLEN], sorted_input[WORDLEN];
    strcpy(sorted_word, word);
    strcpy(sorted_input, input);
    sort(sorted_word);
    sort(sorted_input);

    for (int i = 0; i < (int) strlen(sorted_input); i++)
    {
        if (sorted_input[i] != sorted_word[i])
        {
            return false;
        }
    }
    return true;
}


void sort(char str[WORDLEN])
{
    char tmp;
    int penultimate = (int) strlen(str) - 1;
    for (int i = 0; i < penultimate; i++)
    {
        for (int j = i + 1; j < (int) strlen(str); j++)
        {
            if (str[i] > str[j])
            {
                tmp = str[i];
                str[i] = str[j];
                str[j] = tmp;
            }
        }
    }
}


void print_list(Word* w)
{
    if(w == NULL)
    {
        return;
    }
    printf("word: %s is anagram? %d parent: %s\n", w->word, w->anagram_found, w->anagram_group);
    print_list(w->next);
}

void print_anagrams(Word* start)
{
    Word *word_one = start;
    Word *comparison = start;
    do
    {
        if(next_anagram(word_one))
        {
            printf("%d - ",word_one->anagrams);
        }

        do
        {
            if (strcmp(word_one->anagram_group, comparison->anagram_group) == 0 && word_one->anagrams > 0)
            {
                printf("%s ", comparison->word);
            }
            comparison = comparison->next;
        } while (comparison != NULL);

        if(next_anagram(word_one))
        {
            printf("\n");
        }

        comparison = start;
        word_one = word_one->next;
    } while (word_one != NULL);
    
}

bool next_anagram(Word *w)
{
    if(w->anagrams > 0)
    {
        return true;
    }
    return false;
}
