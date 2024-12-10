/* COP 3502C Assignment 5
This program is written by: Donte Ojeda */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define STR_LEN 2000001
typedef struct trie trie;
struct trie{
    int frequency;
    int sum_prefix_freq;
    int max_child_freq;
    struct trie *next[26];
};

trie *init();
void printAll(struct trie* tree, char cur[]);
void insert(trie *curr_ptr, char *word, int k, int f);
void query(trie *tree, char *output, char *prefix, int k);
void freeDictionary(struct trie* tree);


int main(){
    int num_cmd;
    char str[STR_LEN];
    trie *myTrie = init();

    scanf("%d", &num_cmd);

    for (int i = 0; i < num_cmd; i++){
        int cmd;
        scanf("%d", &cmd);

        //handle insertion cmd
        if (cmd == 1){
            int frequency = 0;

            //run insert cmd
            scanf("%s%d", str, &frequency);
            insert(myTrie, str, 0, frequency);
        }
        //handle query cmd
        else if(cmd == 2){
            char output[STR_LEN];
            output[0] = '\0';

            //run query cmd
            scanf("%s", str);
            query(myTrie, output, str, 0);
        }

    }

    // char word[STR_LEN];
    // word[0] = '\0';
    // printAll(myTrie, word);

    freeDictionary(myTrie);
    return 0;
}

//initializes trie struct
trie *init(){
    
    struct trie* myTree = malloc(sizeof(struct trie));
    myTree->frequency = 0;
    myTree->sum_prefix_freq = 0;
    myTree->max_child_freq = 0;
    
    int i;
    for (i=0; i<26; i++)
        myTree->next[i] = NULL;

    
    return myTree;
}

// Prints all words stored in the trie in alphabetical order.
void printAll(struct trie* tree, char cur[]){

    // Stop!
    if (tree == NULL) return;

    // Print this node, if it's a word.
    if (tree->frequency > 0)
        printf("%s\n", cur);

    // Safer if we store this.
    int len = strlen(cur);

    // Recursively print all words in each subtree,
    // in alpha order.
    int i;
    for (i=0; i<26; i++) {
        cur[len] = (char)('a'+i);
        cur[len+1] = '\0';
        printAll(tree->next[i], cur);
    }
}

//inserts new word intro trie or updates its frequency if it already exists
//also updates the sums of prefix frequency and maximum child frequency variables as it travels through the trie
void insert(trie *curr_ptr, char *word, int k, int f){

    //first check if we are at the end of the inserted word and if we are: update its variables accordingly
    if (word[k] == '\0'){
        curr_ptr->frequency += f;
        curr_ptr->sum_prefix_freq += f;
        for (int i = 0; i < 26; i++){
            if (curr_ptr->next[i] != NULL){
                if (curr_ptr->next[i]->sum_prefix_freq > curr_ptr->max_child_freq){
                    curr_ptr->max_child_freq = curr_ptr->next[i]->sum_prefix_freq;
                }
            }
        }
        return;
    }

    //update current pointer's sum_prefix_freq by adding the passed in frequency to it
    curr_ptr->sum_prefix_freq += f;

    //create the next pointer if it doesn't exist
    int next_index = word[k] - 'a';
    if (curr_ptr->next[next_index] == NULL){
        curr_ptr->next[next_index] = init();
    }

    //update the current pointer's max_child_freq if we must 
    int next_pfx_sum = curr_ptr->next[next_index]->sum_prefix_freq;
    if (next_pfx_sum + f > curr_ptr->max_child_freq)
        curr_ptr->max_child_freq = next_pfx_sum + f;

    //recursively call the function with the next pointer
    insert(curr_ptr->next[next_index], word, k+1, f);

}

//searches for most common next characters based on prefix then prints them out
void query(trie *tree, char *output, char *prefix, int k){
    
    //if we have yet to reach the end of the prefix string 
    //recursively call this function if the next letter exists as a prefix
    if (prefix[k] != '\0'){
        int next_index = prefix[k] - 'a';
        if (tree->next[next_index] == NULL){
            printf("unrecognized prefix\n");
            return;
        }

        query(tree->next[next_index], output, prefix, k+1);
        return;
    }

    //build the output string
    int index = 0;
    for (int i = 0; i < 26; i++){
        if(tree->next[i] != NULL){
            if (tree->next[i]->sum_prefix_freq == tree->max_child_freq){
                output[index] = (char)('a' + i);
                output[index + 1] = '\0';
                index++;
            }
        }
    }

    //print the output string if there is one
    if (output[0] != '\0'){
        printf("%s\n", output);
        return;
    }
    else{
        printf("unrecognized prefix\n");
        return;
    }

}

//free function
void freeDictionary(struct trie* tree){

    int i;
    for (i=0; i<26; i++)
        if (tree->next[i] != NULL)
            freeDictionary(tree->next[i]);

    free(tree);
}