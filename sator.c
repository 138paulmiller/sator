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
	int depth; //also length of word
	struct trie_t * parent; 
	struct trie_t * children[CN]; 
	//add weights ? 
	int term;//terminates word
}trie;

#define trie_foreach_child(t_, c_)\
	trie * c_;\
	for(int ix =0; ix<26; ix++)\
		if((c_ = t_->children[ix]))


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
		trie_foreach_child(t, c)
			trie_free(c);
		free(t);
	}
}
void trie_add(trie * t, char * word)
{
	int len = strlen(word);
	int i = 0;
	while(i < len)
	{
		//find or create
		int idx = word[i] - 'a';
		if(t->children[idx] == 0)
			t->children[idx] = trie_new(t, word[i]);
		t = t->children[idx];
		t->depth = i;
		i++;
	}
	t->term = 1;

}
trie * trie_find(trie * t, char * prefix)
{
	trie * c;
	int i = 0;
	int len = strlen(prefix);
	while(t && i < len)
	{
		if(!(c = t->children[prefix[i++] - 'a']))
			return t;
		t=c;
	}
	return t;
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

//print backwards from leaf
void print_word_upwards(trie * from)
{
	if(!from->term)return;
	int d = from->depth+2;
	char* word = malloc(d);
	memset(word,0, d);
	word[--d] = '\0';
	while(from && from->parent )
	{
		word[--d] = from->val;
		from = from->parent;
	}
	printf("%s\n", word);
	free(word);
}

//print all suffixes at a given node
void print_suffixes(trie * from)
{
	//
	if(!from) return;
	print_word_upwards(from);
	trie_foreach_child(from, c)
	{
		print_suffixes(c);
	}
}

//build all word squares
void print_sators(trie * t, char * word)
{
	int i = 0;
	int len = strlen(word);
	char * prefix = malloc(len+1);
	while(i <= len)
	{
		memcpy(prefix, word, i+1);
		trie * f = trie_find(t, prefix);
		print_suffixes(f);
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
	while(printf(">") && fgets(buf,WN, stdin) && (word = strtok(buf, "\n")))
	{
		const int len = strlen(word); 
		trie * t = trie_find(word_db[len], word);
		if(t && len)
		{		
			if(t->term)
				printf("Found [%s]\n", word);
			else
				printf("Invalid word [%s]\n", word);
			//debug, for all words greater than current length
			for(int i = len; i < WN; i++)
			{
				trie * f = trie_find(word_db[i], word);
				if(f && len-1 == f->depth)
					print_suffixes(f);
			}
		}

		//print_sators(t, word);
	}
	
	for(int i = 0; i < WN; i++)
	{
		trie_free(word_db[i]);
		word_db[i] = 0;	
	}
	printf("Goodbye!\n");
}