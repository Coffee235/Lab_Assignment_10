#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define ALPHABET_SIZE 26

// Trie node structure
struct TrieNode {
    struct TrieNode *children[ALPHABET_SIZE];
    bool isEndOfWord;
    int count;
};

// Initialize
struct TrieNode *createTrieNode() {
    struct TrieNode *node = (struct TrieNode *)malloc(sizeof(struct TrieNode));
    if (node) {
        node->isEndOfWord = false;
        node->count = 0;
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            node->children[i] = NULL; 
        }
    }
    return node;
}


// Inserts a word 
void insert(struct TrieNode *root, char *word) {
    struct TrieNode *curr = root;
    for (int i = 0; word[i] != '\0'; i++) {
        int index = word[i] - 'a';
        if (!curr->children[index]) {
            curr->children[index] = createTrieNode();
        }
        curr = curr->children[index];
    }
    // Mark the end of the word
    curr->isEndOfWord = true;
    // Increment the count
    curr->count++;
}

// Searches for a word
int search(struct TrieNode *root, char *word) {
    struct TrieNode *curr = root;
    for (int i = 0; word[i] != '\0'; i++) {
        int index = word[i] - 'a';
        if (!curr->children[index]) {
            return 0; // Word not found
        }
        curr = curr->children[index];
    }
    if (curr != NULL && curr->isEndOfWord) {
        return curr->count;
    }
    return 0; // Word not found
}

// Deallocates memory for the Trie
void deallocateTrie(struct TrieNode *root) {
    if (root == NULL) return;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (root->children[i] != NULL) {
            deallocateTrie(root->children[i]);
        }
    }
    free(root);
}

// Reads the dictionary file
int readDictionary(char *filename, char **pInWords) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file %s\n", filename);
        exit(1);
    }
    int numWords = 0;
    char word[100];
    while (fscanf(file, "%s", word) != EOF) {
        pInWords[numWords] = _strdup(word); 
        numWords++;
    }
    fclose(file);
    return numWords;
}

int main(void) {
    char *inWords[256];
    // Read the number of words in the dictionary
    int numWords = readDictionary("dictionary.txt", inWords);
    for (int i = 0; i < numWords; ++i) {
        printf("%s\n", inWords[i]);
    }
    // Initialize 
    struct TrieNode *root = createTrieNode();
    // Insert words
    for (int i = 0; i < numWords; i++) {
        insert(root, inWords[i]);
    }
    // Words
    char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    // Search and print occurrence
    for (int i = 0; i < 5; i++) {
        printf("\t%s : %d\n", pWords[i], search(root, pWords[i]));
    }
    // Deallocate 
    deallocateTrie(root);
    printf("Trie deallocated.\n");
    return 0;
}
