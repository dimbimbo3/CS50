// Implements a dictionary's functionality

#include <ctype.h> //toupper
#include <stdbool.h> //bool data type
#include <stdio.h> //file io
#include <string.h> //strcpy
#include <strings.h> //strcasecmp
#include <stdlib.h> //malloc

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// Number of buckets in hash table
#define N 676 //all first two letter combinations

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    int index = hash(word); //retrieves the index at which to check for the word
    node *cursor = table[index]; //cursor for stepping through each node at the index
    
    //loops as long as there is nodes
    while(cursor != NULL)
    {
        //checks if the current node holds the given word
        if(strcasecmp(word, cursor->word) == 0)
        {
            return true;
        }
        else
        {
            cursor = cursor->next; //if not, moves cursor to the next node
        }
    }

    return false; //word was not found
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    int incrementer = 0; //initilized to 0 for single letter words (like "I" or "A") and words with apostrophes (like "I'm")
    int multiplier = toupper(word[0]) - 'A'; //determines where to begin indexing for the first letter (example: 0 -> Aa, 1 -> Ba, 2 -> Ca, ...)

    //verifies that the given word has more than one character
    if(word[1] != '\0')
    {
        //checks if the second character is a letter in order to determine how far to go beyond the index of that first letter (example: 0 -> Aa, 1 -> Ab, 2 -> Ac, ...)
        if(isalpha(word[1]))
        {
            incrementer = toupper(word[1]) - 'A';
        }
    }

    //calculates the index for these first two letters (example: Aa->Az = 0->25, Ba-Bz = 26->51, Ca-Cz = 52->77, ...)
    return (26 * multiplier) + incrementer;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    FILE *input = fopen(dictionary, "r"); //opens the given dictionary file for reading
    //checks that the file could be loaded
    if(input == NULL){
        return false;
    }
    
    char buffer[LENGTH+1]; //buffer to store each read-in word from dictionary

    //loops through each string in the input file
    while(fscanf(input, "%s", buffer) != EOF)
    {
        unsigned int index = hash(buffer); //get the appropriate index for the word in the hash table
        updateDictionary(buffer, index);
    }

    fclose(input); //close dictionary source file
    return true;
}

// Adds the given word to the loaded dictionary.
void updateDictionary(const char *word, unsigned int index){
    node *cursor = table[index]; //cursor for stepping through each node at the index
    
    //checks if there are any words at this index
    if(cursor == NULL)
    {
        table[index] = malloc(sizeof(node)); //allocate memory for first dictionary word
        cursor = table[index]; //set cursor to newly created first node at index
        cursor->next = NULL; //initialize next pointer to NULL
    }
    else
    {
        //iterates through until the next node will be null
        while(cursor->next != NULL)
        {
            cursor = cursor->next;
        }

        cursor->next = malloc(sizeof(node)); //allocate memory for next dictionary word
        cursor = cursor->next; //set cursor to next open node at index
        cursor->next = NULL; //initialize next pointer to NULL
    }

    strcpy(cursor->word, word); //stores word in its position (according to first two letters)
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    node *cursor; //cursor for stepping through each node at each index
    int words = 0;

    //loops through the entire hash table
    for(int i = 0; i < N; i++)
    {
        cursor = table[i]; //sets cursor to the current index
        //loops through each node
        while(cursor != NULL)
        {
            words++; //increment word count
            cursor = cursor->next; //move cursor to the next node
        }
    }

    return words;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    node *cursor; //cursor for stepping through each node at each index
    node *temp; //temporary point for node deletion

    //loops through the entire hash table
    for(int i = 0; i < N; i++)
    {
        cursor = table[i]; //sets cursor to the current index
        //loops through each node
        while(cursor != NULL)
        {
            temp = cursor; //set temp to the current cursor
            cursor = cursor->next; //move cursor to the next node
            free(temp); //frees the memory from the previous node
        }
    }

    return true;
}
