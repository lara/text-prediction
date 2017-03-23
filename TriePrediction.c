/*
Dilara Aydin
TriePrediction 
Assignment #4
8/1/2016
*/

#include "TriePrediction.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void maxWord(TrieNode *root, char *buffer, int k, int *count, char *mostFreqWord)
{
	int i;

	if (root == NULL)
		return;

	if (root->count > *count) {
		strcpy(mostFreqWord, buffer);
		*count = root->count;
	}

	buffer[k + 1] = '\0';

	for (i = 0; i < 26; i++)
	{
		buffer[k] = 'a' + i;

		maxWord(root->children[i], buffer, k + 1, count, mostFreqWord);
	}

	buffer[k] = '\0';
}

//Function to searches the trie for the most frequently occurring word and copies it into the
//string variable passed to the function
void mostFrequentWord(TrieNode *root, char *str) {
	char buffer[1024];
	int mostFreqCount = 0; 
	
	maxWord(root, buffer, 0, &mostFreqCount, str);
	
	return;
}

//Function to dynamically allocate space for a new TrieNode struct. 
//Initializes all the struct members appropriately.
TrieNode *createTrieNode(void){
	
    TrieNode *node;
    
    node = calloc(1, sizeof(TrieNode));
	    
    if(node == NULL)	
    
		return NULL;

    return node;
    
}

//Function to search the trie for the specified string passed in.
TrieNode *getNode(TrieNode *root, char *str){
	
	int c;

    if(str[0] == '\0')
    
        return root;

    c = tolower(str[0]) - 'a';
    
    if(root->children[c] == NULL)
    
        return NULL;

    return getNode(root->children[c], ++str);
    
}

//Inserts the string str into the trie.
void insertString(TrieNode *root, char *str){
	
    int c, i = 0, len = strlen(str);
    
    TrieNode *node = root;
    
    while(i < len){
    	
    	c = tolower(str[i]) - 'a';
    	
    	if(node->children[c] == NULL)
    	
    		node->children[c] = createTrieNode();
    	
		node = node->children[c];	
    		
    	i++;    	
    	
	}
	
    node->count++;
    
	return;
        
}

//Function to take a string, and remove all punctuation as well as lowering the characters of the string
void optimizeString(char *str){
	
	int i = 0, j = 0, len = strlen(str);
	
	char *temp;
	
	while(i < len){
		
		if(isalpha(str[i])){
		
			str[j] = tolower(str[i]);
			j++;
			
		}
			
		i++;
		
	}
	
	str[j] = '\0';	
	
}

//Function to determine if the string is the last word in the file by checking its last character
//Returns 1 if it's the last word and 0 otherwise.
int isLast(char *str){
	
	int len = strlen(str);
	    	
	if(str[len - 1] == '.' || str[len - 1] == '!' || str[len - 1] == '?')
	
		return 1;
		
	return 0;	
	
}

//Function to open the corpus file and create a trie and its according subtries if needed.
TrieNode *buildTrie(char *filename){
	
	FILE *fp;
	
	    TrieNode *root, *node;
	    
	    //flags to indicate first and last letters throughout the while loop
	    int first = 1, last; 
	    
	    char *word = malloc(sizeof(char) * (MAX_CHARACTERS_PER_WORD + 1));
	
	    fp = fopen(filename, "r");
	
	    root = createTrieNode();
	
	    while(fscanf(fp, " %s ", word) != EOF){
	    	
	    	last = isLast(word);	
	    	
			//Optimizing the string and inserting it into the root applies for all words in the text file.	
	    	optimizeString(word);
	    		
	    	insertString(root, word);	    	
	    	
	    	//if the string dealt with is only one word
	    	if(first == 1 && last == 1)
	    		
	    		last = 0;
	    	
	    	//if the string dealt with is only the last word
	    	else if(first == 0 && last == 1){
	    		
		        if(node->subtrie == NULL)
				            
		        	node->subtrie = createTrieNode();
		                        
		    	insertString(node->subtrie, word);
		    	
		    	first = 1;	
	    		
			}
	    	
	    	//if the string dealt with is only the first word
	    	else if(first == 1 && last == 0){

				node = getNode(root, word);
		    	
		    	first = 0;
	    		
			}
			
			//if the string dealt with is a middle word
			else{
	    		
		        if(node->subtrie == NULL)
				            
		        	node->subtrie = createTrieNode();
		                        
		    	insertString(node->subtrie, word);
		    	
		    	node = getNode(root, word);
				
			}
            
		}
		
		free(word);
		
		word = NULL;
		
		fclose(fp);
		
		fp = NULL;
		
		return root;
	
}

// Helper function called by printTrie()
void printTrieHelper(TrieNode *root, char *buffer, int k)
{
	int i;

	if (root == NULL)
		return;

	if (root->count > 0)
		printf("%s (%d)\n", buffer, root->count);

	buffer[k + 1] = '\0';

	for (i = 0; i < 26; i++)
	{
		buffer[k] = 'a' + i;

		printTrieHelper(root->children[i], buffer, k + 1);
	}

	buffer[k] = '\0';
}

// If printing a subtrie, the second parameter should be 1; otherwise, if
// printing the main trie, the second parameter should be 0.
void printTrie(TrieNode *root, int useSubtrieFormatting)
{
	char buffer[1026];

	if (useSubtrieFormatting)
	{
		strcpy(buffer, "- ");
		printTrieHelper(root, buffer, 2);
	}
	else
	{
		strcpy(buffer, "");
		printTrieHelper(root, buffer, 0);
	}
}

//Function to open the input file and read in the comments
void commands(TrieNode *root, char* filename){
	
    FILE *fp;
    char *word;
    int i, n;
    
    TrieNode *node, *temp;
    
    word = malloc(sizeof(char) * (MAX_CHARACTERS_PER_WORD + 1));
    
    fp = fopen(filename, "r");

    while(fscanf(fp, " %s ", word) != EOF){
        
        //print the trie using the output format shown in the sample outputs for this program when '!' is
        //encountered in the input file
        if(word[0] == '!')
        
            printTrie(root, 0);
            
        //text prediction    
        else if(word[0] == '@'){
        	fscanf(fp, "%s", word);
        	fscanf(fp, "%d", &n);
        	
        	printf("%s", word);
        	
        	for (i = 0; i < n; i++) {
        		optimizeString(word);
        		temp = getNode(root, word);
        		
        		if (temp == NULL)
        			break;
        		else if(temp->subtrie == NULL)
					break;
				else {
					mostFrequentWord(temp->subtrie, word);
					printf(" %s", word);
				}
			}
			
			printf("\n");      	
		}   
            
        else{
        	
            printf("%s\n", word);
            
            node = getNode(root, word);
                         
            if(node == NULL)
            
                printf("(INVALID STRING)\n");
                         
            else if(node->subtrie == NULL)
            
                printf("(EMPTY)\n");
                            
            else
            
                printTrie(node->subtrie, 1);
        
		}
		
    }

	free(word);
	
	word = NULL;
	
    fclose(fp);
    
    fp = NULL;
    
}

//Function to free all dynamically allocated memory associated with the trie.
TrieNode *destroyTrie(TrieNode *root){
	
	int i;
	
	TrieNode *temp;
	
   if(root == NULL)	
   
		return NULL;
	
	while(root != NULL){
		
		for(i = 0; i < 26; i++){
		
			root->children[i] = destroyTrie(root->children[i]);
						
			free(root->children[i]);			
		}
	
				
		temp = root->subtrie;
			
		free(root);	
		
		root = temp;
		
	}		
		
	free(root);	
	
	return NULL;
    
}

//Yay, a main function!
int main(int argc, char **argv){
	
    TrieNode* root;
    
    root = buildTrie(argv[1]);
    
    commands(root, argv[2]);
    
    root = destroyTrie(root);

    return 0;
}


