#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARRAY_SIZE(a) sizeof(a)/sizeof(a[0])

// Trie structure
struct Trie
{
    struct Trie *children[26];
    int isEndOfWord;
    int count;
};

struct Trie *createTrie()
{
    struct Trie *pNode = (struct Trie *)malloc(sizeof(struct Trie));
    pNode->isEndOfWord = 0;
    pNode->count = 0;

    for (int i = 0; i < 26; i++)
        pNode->children[i] = NULL;

    return pNode;
}

// Inserts the word to the trie structure
void insert(struct Trie *root, char *word)
{
    struct Trie *pNode = root;

    for (int i = 0; i < strlen(word); i++)
    {
        int index = word[i] - 'a';
        if (!pNode->children[index])
            pNode->children[index] = createTrie();

        pNode = pNode->children[index];
    }

    pNode->isEndOfWord = 1;
    pNode->count += 1;
}

// Computes the number of occurrences of the word
int numberOfOccurances(struct Trie *root, char *word)
{
    struct Trie *pNode = root;

    for (int i = 0; i < strlen(word); i++)
    {
        int index = word[i] - 'a';
        if (!pNode->children[index])
            return 0;

        pNode = pNode->children[index];
    }

    return pNode->count;
}

// Deallocate the trie structure
struct Trie *deallocateTrie(struct Trie *root)
{
    if (root == NULL) return NULL;

    for (int i = 0; i < 26; i++)
    {
        if (root->children[i])
            deallocateTrie(root->children[i]);
    }

    free(root);
    return NULL;
}

// This function will return number of words in the dictionary,
// and read all the words in the dictionary to the structure words
int readDictionary(char *filename, char **pInWords)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        printf("Could not open file %s", filename);
        return 0;
    }
    
    int numWords;
    fscanf(file, "%d\n", &numWords);

    for (int i = 0; i < numWords; ++i)
    {
        pInWords[i] = malloc(256);
        fgets(pInWords[i], 256, file);
        
        pInWords[i][strcspn(pInWords[i], "\n")] = 0;
    }

    fclose(file);
    return numWords;
}

int main(void)
{
    char *inWords[256];

    int numWords = readDictionary("dictionary.txt", inWords);
    for (int i = 0; i < numWords; ++i)
    {
        printf("%s\n", inWords[i]);
    }

    struct Trie *pTrie = createTrie();
    for (int i = 0; i < numWords; i++)
    {
        insert(pTrie, inWords[i]);
    }

    char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < ARRAY_SIZE(pWords); i++)
    {
        printf("\t%s : %d\n", pWords[i], numberOfOccurances(pTrie, pWords[i]));
    }

    pTrie = deallocateTrie(pTrie);
    if (pTrie != NULL)
    {
        printf("There is an error in this program\n");
    }

    for (int i = 0; i < numWords; i++)
    {
        free(inWords[i]);
    }

    return 0;
}
