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
trie * word_db[WN] ;

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
		free_trie(t->child);
		t->child = 0;
		trie* s = t->sibling;
		t->sibling = 0;
		while(s)
		{
			free_trie(s);
			s = s->sibling;
		}
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
	memset(word_db, 0, WN*sizeof(trie));
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

//visit all children after prefix
trie * get_child(trie * t, char * prefix)
{
	int i = 0;
	const int len = strlen(prefix);
	while(++i < len)
	{
		while(t && t->val != prefix[i])
		{
			t = t->sibling;
		}
		if(t == 0)
		{
			return 0 ;
		}
		else
			t = t->child;		
	}
	//
	return t;
}
//build all word squares
void print_sators(trie * t, char * word)
{
	int i = 0;
	const int len = strlen(word);
	char * prefix = malloc(len+1);
	while(i < len)
	{
		memcpy(prefix, word, i);
		trie * c = get_child(t, prefix);
		//using this child, create word iterator 
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


	char buf[WN] ;
	while(printf(">") && fgets(buf,WN, stdin) )
	{
		int len = strlen(buf);
		if(buf[len-1]=='\n')
		{
			buf[len-1] = '\0';	
			len--;
		}
		
		trie * t = word_db[len]; 
		int success = get_child(t, buf) == 0;
		if(success)
		{
			printf("Building Sator for %s...\n", buf);
			print_sators(t, buf);
		}
		else
			printf("Invalid word [%s], please try again\n", buf);
	}
	
	for(int i = 0; i < WN; i++)
	{
		free_trie(word_db[i]);
		word_db[i] = 0;	
	}
	printf("Goodbye!\n");

}