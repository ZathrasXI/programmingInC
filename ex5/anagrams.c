#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define FILESIZE 25
#define FILENAME "anagrams.txt"
#define WORDSIZE 15

bool is_anagram(char input[WORDSIZE], char word[WORDSIZE]);

int main(int argc, char* argv[])
{
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
        fprintf(stderr,"issue closing %s\n", FILENAME);
        exit(EXIT_FAILURE);
    }
    return 0;
}

bool is_anagram(char input[WORDSIZE], char word[WORDSIZE])
{
    if (strlen(input) != strlen(word) - 1)
    {
        printf("%lu - %s\n", strlen(word), word);
        return false;
    }
    return true;
}

// void remove_new_line(char str)
// {
//     int length = (int) strlen(str);
//     if (str[length-1] == '\n')
//     {
//         str[length-1] = '\0';
//     }
// }
