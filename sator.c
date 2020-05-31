#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

/*
	Example : 

	at

	 ____            _____
	|    | sibling  |     |
	| a  |--------> |     |
	|____|
		| child
		V
*/
//number chars in longest word
#define WN 23
#define INPUT "words.txt"


typedef struct trie_t
{
	char val; 
	int term; //if terminates then valid word
	struct trie_t * parent; 
	struct trie_t * sibling; 
	struct trie_t * child; 
}trie;

/*
   Sator - keeps an array of trie terminators. 
*/

//each trie is indexed by word length
trie * word_db[WN] ;

trie * trie_new(trie * p, char val)
{
	trie * t = malloc(sizeof(trie));
	t->val = val;
	t->term = 0 ;
	t->sibling = 0;
	t->child = 0;
	t->parent = p;
	return t;
}
void trie_free(trie * t)
{
	if(t)
	{
		trie_free(t->child);
		t->child = 0;
		trie* s = t->sibling;
		t->sibling = 0;
		while(s)
		{
			trie_free(s);
			s = s->sibling;
		}
		free(t);
	}
}
void trie_add(trie * t, char * word)
{
	int len = strlen(word);
	trie * par = t;
	int i = 0;
	while(i < len)
	{
		trie * prev = t;
		//find or create
		char c = word[i];
		while(t && t->val != word[i])
		{
			prev = t;
			t = t->sibling;
		}
		if(t == 0)
		{
			t= (prev->sibling = trie_new(par, word[i]));
		}	
		par = t;
		if(t->child)
			t = t->child;
		else if(i+1 <= len)
		{
			t = (t->child = trie_new(par, word[i+1]));
			if(i + 1 == len)
				t->term = 1;
		}
		i++;
	}
}


//visit all children after prefix
trie * trie_find(trie * t, char * prefix)
{
	int i = 0;
	int len = strlen(prefix);
	while(i < len)
	{
		trie * _t = t;
		printf("At : %c\n", prefix[i]);
		printf("list :");
		while(_t)
		{
			printf(" %c ", _t->val);
			_t = _t->sibling;
		}

		while(t && t->val != prefix[i])
		{
			printf(" %c ", t->val);
			t = t->sibling;
		}
		if(t == 0) printf("Not Found");
		getchar();
		if(t == 0)
			return 0 ;
		else
			t = t->child;		
		i++;
	}
	printf("Found %s %c : %d\n", prefix, t ? t->val : '-', t ? t->term : -1);
	return t;
}

//pretty print somehow
void trie_print(trie * t)
{
	if(!t) return;
	printf(" [ %c ", t->val);

	trie * _t = t;
	printf("list :");
	while(_t = _t->sibling)
	{
		trie_print(_t);
	}
	trie_print(t->child);		
	printf(" ]\n");

}

int make_word_db()
{
	for(int i = 0; i < WN; i++)
	memset(word_db, 0, WN*sizeof(trie));
	FILE * f = fopen(INPUT, "r");
	char buf[WN] ;
	char * word;
	//strtok removes newline
	while(f && fgets(buf,WN, f) && (word = strtok(buf, "\n")) )
	{
		int len = strlen(word);
		if(word_db[len] == 0)
			word_db[len] = trie_new(0, word[0]);

		trie_add(word_db[len], word);
		// trie_print(word_db[len]);
		// getchar();

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

	char buf[WN];
	char * word;
	while(printf(">") && fgets(buf,WN, stdin) )
	{
		word = strtok(buf, "\n");
		trie * t = word_db[strlen(word)]; 
		int success = (trie_find(t, word) != 0);
		
		if(success) 
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