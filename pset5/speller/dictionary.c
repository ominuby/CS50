/**
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <cs50.h>
#include <ctype.h>

#include "dictionary.h"

// define the structure of basic element of linkedlist
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;//pointer to other node
}
node;

// hash table
node *HashTable[Hash_Size];

// hash function
int hash_func(char *word)
{
    // hash alphabetically
    return tolower(*word) % Hash_Size;
}

// calculate size of dictionary
unsigned int DictionarySize = 0;

/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    // TODO
    int len = strlen(word);
    char word_ed[len + 1];
    for (int i = 0; i < len + 1; i++)//BECAREFUL THAT i should be less than len+1 instead of len
    {
        word_ed[i] = word[i];
    }
    int h = hash_func(word_ed);
    node *cursor = HashTable[h];
    while(cursor != NULL)
    {
        int same = !strcasecmp(word_ed,cursor->word);
        if(same)
        {
            return true;
        }
        else
        {
            cursor = cursor->next;
        }

    }
    return false;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    // TODO
    /*
    for (int i = 0; i < Hash_Size; i++)
    {
        HashTable[i] = NULL;
    }
    */

    FILE *dptr = fopen(dictionary,"r");
    // ensure dictionary is opened successfully
    if (dptr == NULL)
    {
        printf("Can't open the dictionary.\n");
        return false;
    }
    char word[LENGTH];
    while (fscanf(dptr,"%s",word) != EOF)
    {
        node *new_node = malloc(sizeof(node));
        // ensure malloc succeed
        if (new_node == NULL)
        {
            printf("malloc unsuccessfully.\n");
            unload();
            return false;
        }

        strcpy(new_node->word,word);
        new_node->next = NULL;

        int h = hash_func(new_node->word);
        node *head = HashTable[h];
        // check if the bucket is empty
        if (head == NULL)
        {
            HashTable[h] = new_node;
        }
        else
        {
            new_node->next = HashTable[h];
            HashTable[h] = new_node;
        }
        DictionarySize++;
    }
    fclose(dptr);
    return true;

}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    // TODO
    return DictionarySize;
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    for (int i = 0; i < Hash_Size; i++)
    {
        node *cursor = HashTable[i];
        while (cursor != NULL)
        {
            node *tmp = cursor;
            cursor = cursor->next;
            free(tmp);
        }
    }

    return true;
}
