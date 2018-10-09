// Implements a dictionary's functionality

#include "dictionary.h"

#define NUM_NODES 27


typedef struct node
{
    bool is_word;
    struct node *children[NUM_NODES];
}
Node;

Node *root;

int n_words = 0;

// Returns true if word is in dictionary else false
bool check(const char *word)
{
    Node* current = root;
    int i = 0;

    while(word[i] != '\0')
    {
        int letter = tolower(word[i]);

        if (letter == '\'')
        {
            letter = 123;
        }

        if (current->children[letter-'a'] != NULL)
        {
            current = current->children[letter-'a'];
            i++;
        }

        else
        {
           return false;
        }
    }

    if (current->is_word == true)
    {
       return true;

    }
    return false;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char* dictionary)
{
    // Open the dictionary file
    FILE* dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        return false;
    }

    // Allocate space for the first root node
    root = calloc(1, sizeof(Node));

    // Points to current node as the program descends through the trie
    Node* current = root;

    // Read the dictionary one character at a time
    for (char c = tolower(fgetc(dict)); c != EOF; c = fgetc(dict))
    {
        // If the word is finished
        if (c == '\n')
        {
            // Check that the word is at least one letter long
            if (current != root)
            {
                n_words++;

                current->is_word = true;

                // Reset the pointer back to the top of the trie
                current = root;
            }
        }
        // If the word is still in progress
        else
        {
            // Check if character is apostrophe
            if (c == '\'')
            {
                // 123 - 97 = 26 (apostrophes go at nodes[26])
                c = 123;
            }

            // Try to get the address of the next node in the trie
            struct node* nextNode = current->children[c-97];
            if (nextNode == NULL)
            {
                // Allocate space if it is not yet there
                current->children[c-97] = calloc(1, sizeof(Node));

                current = current->children[c-97];
            }
            else
            {
                current = nextNode;
            }

        }
    }

    fclose(dict);

    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    return n_words;
}

void freeNode(Node* current)
{
    for (int i = 0; i < NUM_NODES; i++)
    {
        if(current->children[i] != NULL)
        {
            freeNode(current->children[i]);
        }

    }
    current = NULL;
    free(current);

}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    freeNode(root);
    return true;
}
