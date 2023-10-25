#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#define FILESIZE 25
#define FILENAME "anagrams.txt"
#define WORDSIZE 15

//TODO should it warn me if I've imported a library and I'm not using anything from it? e.g. not using assert() from assert.h
//TODO ask which is more efficient: function returns ul or I do more type casting?
//TODO swap() using pointers
//TODO don't print word if it is equal to input
unsigned long len_without_newline_char(char str[WORDSIZE]);
void test(void);
bool is_anagram(char input[WORDSIZE], char word[WORDSIZE]);
void sort(char str[WORDSIZE]);

int main(int argc, char* argv[])
{
    test();
    if (argc != 2)
    {
        fprintf(stderr, "usage: ./anagrams <anagram string>\n");
        exit(EXIT_FAILURE);
    }

    if (strlen(argv[1]) > WORDSIZE)
    {
        fprintf(stderr, "the longest word is %d characters\n", WORDSIZE);
        exit(EXIT_FAILURE);
    }
    char input[WORDSIZE];
    strcpy(input, argv[1]);
    sort(input);

    FILE* anagram_file = fopen(FILENAME, "r");
    if (!anagram_file)
    {
        fprintf(stderr, "Unable to open %s\n", FILENAME);
    }

    char buffer[WORDSIZE];
    while (fgets(buffer, WORDSIZE, anagram_file) != NULL)
    {
        
        if (is_anagram(input, buffer))
        {
            printf("%s\n", buffer);
        }
    }

    int closed = fclose(anagram_file);
    if (closed != 0)
    {
        fprintf(stderr,"issue closing file: %s\n", FILENAME);
        exit(EXIT_FAILURE);
    }
    return 0;
}

bool is_anagram(char input[WORDSIZE], char word[WORDSIZE])
{
    if (strlen(input) != len_without_newline_char(word))
    {
        return false;
    }

    char sorted_word[WORDSIZE];
    strcpy(sorted_word, word);
    sort(sorted_word);
    for (int i = 0; i < (int) strlen(input); i++)
    {
        if (input[i] != sorted_word[i])
        {
            return false;
        }
    }
    return true;
}

unsigned long len_without_newline_char(char str[WORDSIZE])
{
    int length = strlen(str);
    if (str[length-1] == '\n')
    {
        str[length-1] = '\0';
        return length - 1;
    }
    return length;
}

void sort(char str[WORDSIZE])
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

void test(void)
{
    char str[WORDSIZE] = "elephants\n";
    assert(len_without_newline_char(str) == (unsigned long) 9);
    strcpy(str, "elephants");
    assert(len_without_newline_char(str) == (unsigned long) 9);

    sort(str);
    char sorted[WORDSIZE] = "aeehlnpst";
    assert(strcmp(str, sorted) == 0);

    assert(is_anagram(sorted, str) == true);
}
