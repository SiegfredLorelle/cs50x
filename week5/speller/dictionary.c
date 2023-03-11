// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>

// Included by the student
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 26;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    // Get the the index in the hash to access the correct linked list
    unsigned int index = hash(word);
    // Point cursor to the start of the linked list
    node *cursor = table[index];

    // Loop through each node in the linked list
    while (cursor != NULL)
    {
        // Check if the word is in the dictionary (linked list)
        if (strcasecmp(word, cursor->word) == 0)
        {
            return true;
        }
        // Go to next node
        cursor = cursor->next;
    }
    // Word is not in the dictionary
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    // Return the index of the word
    return toupper(word[0]) - 'A';
}

// Loads dictionary into memory, returning true if successful, else false
int counter = 0;
bool load(const char *dictionary)
{
    // TODO
    // Open the dictionary
    FILE *dictionary_file = fopen(dictionary, "r");
    // Check if the dictionary is openable
    if (dictionary_file == NULL)
    {
        printf("The file could not be opened.\n");
        return false;
    }

    // Get every word in the dictionary
    char current_word[LENGTH];
    while (fscanf(dictionary_file, "%s", current_word) != EOF)
    {
        // Allocate memory
        node *tmp = malloc(sizeof(node));

        // Check if extra memory exists
        if (tmp == NULL)
        {
            return false;
        }

        // Copy current word to tmp
        strcpy(tmp->word, current_word);

        // insert to hash
        unsigned int index = hash(tmp->word);

        // Index the the linked list in the hash then include the word to linked list
        tmp->next = table[index];
        table[index] = tmp;

        // Count how many times a word is loaded in the linked list (needed for size function)
        counter++;
    }
    fclose(dictionary_file);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    // TODO
    // Return the counter from load function
    return (unsigned int)counter;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    node *cursor;
    node *tmp;

    // Loop through each linked list in the hash
    for (int i = 0; i < N; i++)
    {
        // Point cursor to the start of the linked list
        cursor = table[i];
        // Loop through each node of the linked list
        while (cursor != NULL)
        {
            // Point tmp to cursor then point cursor to the next node then free tmp
            tmp = cursor;
            cursor = cursor->next;
            free(tmp);
        }
    }
    return true;
}
