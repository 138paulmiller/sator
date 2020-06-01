#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

//number chars in alphabet
#define CN 26
//number chars in longest word
#define WN 23
#define INPUT "./dict/words.txt"

// -------------------------------------- Trie ----------------------------
typedef struct trie_t
{
	char val; 
	struct trie_t * parent; 
	struct trie_t * children[CN]; 
	int term;//terminates word
}trie;

trie * trie_new(trie * p, char val)
{
	trie * t = malloc(sizeof(trie));
	memset(t, 0, sizeof(trie));
	t->val = val;
	t->parent = p;
	return t;
}
void trie_free(trie * t)
{
	if(t)
	{
		for(int i = 0; i < CN; i++)
			trie_free(t->children[i]);
		free(t);
	}
}
void trie_add(trie * t, char * word)
{
	int len = strlen(word);
	trie * par = 0;
	int i = 0;
	while(i < len)
	{
		par = t;
		//find or create
		int idx = word[i] - 'a';
		if(t->children[idx] == 0)
			t->children[idx] = trie_new(par, word[i]);
		t = t->children[idx];
		if(++i == len)
			t->term = 1;
	}
}
trie * trie_find(trie * t, char * prefix)
{
	int i = 0;
	int len = strlen(prefix);
	while(i < len)
	{
		if(!(t = t->children[prefix[i++] - 'a']))
			return 0;
	}
	if(t && t->term)
		return t;
	return 0;
}
// -------------------------------------- End Trie ----------------------------


/*
   Sator - keeps an array of terminal trie nodes? 
*/

//each trie is indexed by word length
trie * word_db[WN] ;

int make_word_db()
{
	char * word;
	char buf[WN+1] ;
	memset(word_db, 0, WN*sizeof(trie));
	FILE * f = fopen(INPUT, "r");
	while(f && fgets(buf,WN+1, f) && (word = strtok(buf, "\n")))
	{
		int len = strlen(word);
		if(word_db[len] == 0)
			word_db[len] = trie_new(0, '-');
		trie_add(word_db[len], word);
	}
	return f ? !fclose(f) : 0;
}

//build all word squares
void print_sators(trie * t, char * word)
{
	return;
	int i = 0;
	int len = strlen(word);
	char * prefix = malloc(len+1);
	while(i <= len)
	{
		memcpy(prefix, word, i);
		trie * c = trie_find(t, prefix);
		//using this child, get all possible words. 
		i++;
	}

	free(prefix);
}

int main()
{

	printf("Building Word DB...\n");
	if(make_word_db() == 0 )
	{
		printf("Failed: Could not open %s\n", INPUT);
		exit(0);
	}
	printf("Finished!\n");

	for(int i = 0; i < WN; i++)
	{
		trie * root =word_db[i]; 
		printf("\n%d:", i);
		for(int j = 0; j < CN; j++)
			printf("%c", (root && root->children[j]) ?  root->children[j]->val : '-' );
	}
	printf("\n");

	char buf[WN];
	char * word;
	while(printf(">") && fgets(buf,WN, stdin)&& (word = strtok(buf, "\n")))
	{
		trie * t = word_db[strlen(word)]; 		
		if((trie_find(t, word) != 0)) 
			// print_sators(t, word);
			printf("Found [%s]\n", word);
		else
			printf("Invalid word [%s], please try again\n", word);
	}
	
	for(int i = 0; i < WN; i++)
	{
		trie_free(word_db[i]);
		word_db[i] = 0;	
	}
	printf("Goodbye!\n");

}