#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

/*

		[ val [] ]
		/  
	[ val   ]

*/
//number chars in longest word
#define WN 23
#define INPUT "words.txt"


typedef struct trie_t
{
	char val; 
	struct trie_t * sibling; 
	struct trie_t * child; 
}trie;

//each trie is indexed by word length
trie * word_db[WN] = { 0 } ;

trie * new_trie(char val)
{
	trie * t = malloc(sizeof(trie));
	t->val = val;
	t->sibling = t->child = 0;
	return t;
}
void free_trie(trie * t)
{
	if(t)
	{
		trie* s = t->sibling;
		while(s)
		{
			free_trie(s);
			s = s->sibling;
		}
		free(t->child);
		free(t);
	}
}

void add_word(char * word, int len)
{
	trie * p = 0;
	trie * t = word_db[len];
	if(t == 0)
	{
		t = word_db[len] = new_trie(*word);
	}
	int i = 0;
	while(++i < len)
	{
		//find or create
		while(t && t->val != word[i])
		{
			p = t;
			t = t->sibling;
		}
		if(t == 0)
		{
			t= (p->sibling = new_trie(word[i]));
		}
		if(t->child)
		{
			t = t->child;
		}
		else if(i+1 < len)
		{
			t = (t->child = new_trie(word[i+1]));
		}
	}
}

int make_word_db()
{
	for(int i = 0; i < WN; i++)
		word_db[i] = 0;
	FILE * f = fopen(INPUT, "r");
	int len;
	char buf[WN] ;
	if(f)
	{
		while(fgets(buf,WN, f) )
		{
			len = strlen(buf);
			add_word(buf, len);
		}
		fclose(f);
		return 1;
	}
	return 0;
}
int check_word_db(char * word)
{
	int i = 0;
	int len = strlen(word);

	trie * t = word_db[len];	
	while(++i < len)
	{
		//find or create
		while(t && t->val != word[i])
		{
			t = t->sibling;
		}
		if(t == 0)
		{
			printf("Could not find %s", word);
			return 0 ;
		}
		else
			t = t->child;		
	}
	return 1;
}
//build all word squares
void print_word_dbquares(char * word)
{

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


	char buf[WN] ;
	while(printf(">") && fgets(buf,WN, stdin) )
	{
		int success = check_word_db(buf);
		if(success)
			print_word_dbquares(buf);
		else
			printf("Invalid word [%s], please try again\n", buf);

	}
	
	for(int i = 0; i < WN; i++)
		free_trie(word_db[i]);	
	printf("Goodbye!\n");

}